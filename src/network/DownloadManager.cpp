#include "network/DownloadManager.h"

#include "data/music/Album.h"
#include "data/music/Artist.h"
#include "data/tv_show/TvShow.h"
#include "log/Log.h"
#include "network/DownloadManagerElement.h"
#include "network/NetworkReplyWatcher.h"
#include "network/NetworkRequest.h"

#include <QFile>
#include <QTimer>

static constexpr char PROP_DOWNLOAD_ELEMENT[] = "downloadElement";

DownloadManager::DownloadManager(QObject* parent) : QObject(parent)
{
}

mediaelch::network::NetworkManager* DownloadManager::network()
{
    static auto* s_network = new mediaelch::network::NetworkManager();
    return s_network;
}

bool DownloadManager::isLocalFile(const QUrl& url)
{
    return url.toString().startsWith("//");
}

void DownloadManager::logCurrentDownloads() const
{
    qCDebug(generic) << "[DownloadManager] Start next download | Files left:" << m_queue.size()
                     << "| Running:" << m_currentReplies.size();
}

void DownloadManager::setDownloads(QVector<DownloadManagerElement> elements)
{
    if (!m_queue.isEmpty() || !m_currentReplies.isEmpty()) {
        abortDownloads();
    }

    for (const DownloadManagerElement& elem : elements) {
        addDownload(elem);
    }

    if (m_queue.isEmpty()) {
        logCurrentDownloads();
        QTimer::singleShot(0, this, &DownloadManager::allDownloadsFinished);
    }
}

void DownloadManager::addDownload(DownloadManagerElement elem)
{
    // TODO: Refactor
    //
    // I previously assumed that we had a threading issue.  But this is not
    // the case because connect() calls methods of DownloadManager only in one
    // thread, DownloadManager's thread.
    // But I forgot that signals (e.g. "emit sigFinished") call their slots
    // _immediately_ by default. This means we could end up in inconsistent
    // states because member variables are changed while we are still inside a
    // method that depends on them or has a previous state.  For example:
    //
    //  addDownload()
    //    startNextDownload();
    //     on finished: downloadFinished()
    //       emit sigDownloadFinished(elem);
    //         some slot calls addDownload, m_currentDownloadElement changes
    //       emit sigElemDownloaded(elem);
    //         may emit wrong element
    //
    // To avoid this, I've adapted all places where a connect() to this
    // download manager is used.  Those places now all used a queued connection.
    //
    // But to be more robust, we should refactor this class and get rid of
    // all member variables that may change state.
    // Also, this would allow parallel downloads.
    //
    // The mutexes have been removed but the code still needs to be refactored.
    //

    qCDebug(generic) << "[DownloadManager] Enqueue download at pos " << downloadQueueSize() << "|" << elem.url;

    m_queue.enqueue(elem);

    const bool shouldStartDownloading = m_currentReplies.size() <= numberOfParallelDownloads;
    if (shouldStartDownloading) {
        startNextDownload();
    }
}

template<class T>
bool DownloadManager::hasDownloadsLeft(T*& elementToCheck)
{
    // Note: This code looks similar to numberOfDownloadsLeft() but does not require
    // to run through all downloads.
    for (elch_ssize_t i = 0, n = m_queue.size(); i < n; ++i) {
        if (m_queue[i].getElement<T>() == elementToCheck) {
            return true;
        }
    }
    // There may be currently running requests for this movie/tv show/...
    for (auto& m_currentReplie : m_currentReplies) {
        // TODO: No copy!
        auto download = m_currentReplie->property(PROP_DOWNLOAD_ELEMENT).value<DownloadManagerElement>();
        if (download.getElement<T>() == elementToCheck) {
            return true;
        }
    }
    return false;
}

template<class T>
int DownloadManager::numberOfDownloadsLeft(T*& elementToCheck)
{
    int count = 0;
    for (elch_ssize_t i = 0, n = m_queue.size(); i < n; ++i) {
        if (m_queue[i].getElement<T>() == elementToCheck) {
            ++count;
        }
    }
    // There may be currently running requests for this movie/tv show/...
    for (auto& m_currentReplie : m_currentReplies) {
        // TODO: No copy!
        auto download = m_currentReplie->property(PROP_DOWNLOAD_ELEMENT).value<DownloadManagerElement>();
        if (download.getElement<T>() == elementToCheck) {
            ++count;
        }
    }
    return count;
}

void DownloadManager::abortDownloads()
{
    qCInfo(generic) << "[DownloadsManager] Abort Downloads";

    m_queue.clear();

    // Abort all currently running jobs. Disconnect the finished() signal first!
    for (auto* reply : asConst(m_currentReplies)) {
        // We know that the replies in m_currentReplies aren't finished, because
        // as soon as they are, they're removed from the list. Disconnect before deleteLater().
        disconnect(reply, &QNetworkReply::finished, this, &DownloadManager::downloadFinished);
        disconnect(reply, &QNetworkReply::downloadProgress, this, &DownloadManager::downloadProgress);

        reply->abort();
        reply->deleteLater();
    }
    m_currentReplies.clear();
}

void DownloadManager::startNextDownload()
{
    if (m_queue.isEmpty()) {
        if (m_currentReplies.isEmpty()) {
            qCInfo(generic) << "[DownloadManager] All downloads finished";
            emit allDownloadsFinished();
        } else {
            logCurrentDownloads();
        }
        return;
    }

    DownloadManagerElement download = m_queue.dequeue();

    if (download.imageType == ImageType::Actor || download.imageType == ImageType::TvShowEpisodeThumb) {
        if (download.movie != nullptr) {
            emit movieDownloadsLeft(numberOfDownloadsLeft<Movie>(download.movie), download);

        } else if (download.show != nullptr) {
            emit showDownloadsLeft(numberOfDownloadsLeft<TvShow>(download.show), download);

        } else {
            emit downloadsLeft(downloadQueueSize());
        }
    }
    logCurrentDownloads();

    if (DownloadManager::isLocalFile(download.url)) {
        QFile file(download.url.toString());
        QByteArray data;
        if (file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            file.close();
        }

        download.data = data;

        if (download.actor != nullptr && download.imageType == ImageType::Actor && (download.movie == nullptr)) {
            download.actor->image = data;

        } else if (download.imageType == ImageType::TvShowEpisodeThumb && !download.directDownload) {
            download.episode->setThumbnailImage(data);

        } else { // TODO: Why the "else"?
            emit sigDownloadFinished(download);
        }
        startNextDownload();
        // TODO: Also emit allXXXFinished() signal

    } else {
        QNetworkReply* reply = network()->getWithWatcher(mediaelch::network::requestWithDefaults(download.url));
        reply->setProperty(PROP_DOWNLOAD_ELEMENT, QVariant::fromValue(download));
        m_currentReplies.push_back(reply);

        connect(reply, &QNetworkReply::finished, this, &DownloadManager::downloadFinished);
        connect(reply, &QNetworkReply::downloadProgress, this, &DownloadManager::downloadProgress);
    }
}

void DownloadManager::downloadProgress(qint64 received, qint64 total)
{
    auto* reply = dynamic_cast<QNetworkReply*>(QObject::sender());
    if (reply == nullptr) {
        qCCritical(generic) << "[DownloadManager] dynamic_cast<QNetworkReply*> failed for downloadProgress!";
        return;
    }

    auto element = reply->property(PROP_DOWNLOAD_ELEMENT).value<DownloadManagerElement>();
    element.bytesReceived = received;
    element.bytesTotal = total;

    emit sigDownloadProgress(element);
}

void DownloadManager::restartDownloadAfterTimeout(QNetworkReply* reply)
{
    auto download = reply->property(PROP_DOWNLOAD_ELEMENT).value<DownloadManagerElement>();
    ++download.retries;
    reply->deleteLater();

    qCWarning(generic) << "[DownloadManager] Download timed out:" << download.url;

    if (download.retries < 3) {
        qCDebug(generic) << "[DownloadManager] Re-enqueuing the download, tries:" << download.retries << "/ 3";
        m_queue.prepend(download);

    } else {
        qCDebug(generic) << "[DownloadManager] Giving up on this file, tried 3 times";
    }

    // Should always be true because we're replacing the previous request
    // but just to be sure, we still check this.
    if (m_currentReplies.size() <= numberOfParallelDownloads) {
        startNextDownload();
    }
}

void DownloadManager::downloadFinished()
{
    auto* reply = dynamic_cast<QNetworkReply*>(QObject::sender());
    if (reply == nullptr) {
        qCCritical(generic) << "[DownloadManager] dynamic_cast<QNetworkReply*> failed for downloadFinished!";
        return;
    }

    bool wasRemoved = m_currentReplies.removeOne(reply);
    if (!wasRemoved) {
        qCCritical(generic) << "[DownloadManager] downloadFinished() called for reply which wasn't tracked";
    }

    QByteArray data;
    if (reply->error() != QNetworkReply::NoError) {
        if (reply->property(NetworkReplyWatcher::TIMEOUT_PROP).toBool()) {
            restartDownloadAfterTimeout(reply); // also deletes the reply
            return;
        }
        qCWarning(generic) << "[DownloadManager] Network Error:" << reply->errorString() << "|" << reply->url();

    } else {
        data = reply->readAll();
    }


    DownloadManagerElement downloadElelement = reply->property(PROP_DOWNLOAD_ELEMENT).value<DownloadManagerElement>();
    reply->deleteLater();

    downloadElelement.data = data;

    if (downloadElelement.actor != nullptr && downloadElelement.imageType == ImageType::Actor
        && downloadElelement.movie == nullptr) {
        downloadElelement.actor->image = data;

    } else if (downloadElelement.imageType == ImageType::TvShowEpisodeThumb && !downloadElelement.directDownload) {
        downloadElelement.episode->setThumbnailImage(data);

    } else { // TODO: Why the "else"?
        emit sigDownloadFinished(downloadElelement);
    }

    emit sigElemDownloaded(downloadElelement);

    if (downloadElelement.movie != nullptr && !hasDownloadsLeft<Movie>(downloadElelement.movie)) {
        emit allMovieDownloadsFinished(downloadElelement.movie);
    }
    if (downloadElelement.show != nullptr && !hasDownloadsLeft<TvShow>(downloadElelement.show)) {
        emit allTvShowDownloadsFinished(downloadElelement.show);
    }
    if (downloadElelement.concert != nullptr && !hasDownloadsLeft<Concert>(downloadElelement.concert)) {
        emit allConcertDownloadsFinished(downloadElelement.concert);
    }
    if (downloadElelement.artist != nullptr && !hasDownloadsLeft<Artist>(downloadElelement.artist)) {
        emit allArtistDownloadsFinished(downloadElelement.artist);
    }
    if (downloadElelement.album != nullptr && !hasDownloadsLeft<Album>(downloadElelement.album)) {
        emit allAlbumDownloadsFinished(downloadElelement.album);
    }

    startNextDownload();
}

bool DownloadManager::isDownloading() const
{
    return !m_queue.isEmpty() || !m_currentReplies.isEmpty();
}

int DownloadManager::downloadQueueSize()
{
    return qsizetype_to_int(m_queue.size() + m_currentReplies.size());
}

int DownloadManager::downloadsLeftForShow(TvShow* show)
{
    if (show == nullptr) {
        qCCritical(generic) << "[DownloadManager] Cannot count downloads left for nullptr show";
        return 0;
    }
    return numberOfDownloadsLeft<TvShow>(show);
}
