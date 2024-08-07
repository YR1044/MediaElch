#include "globals/Manager.h"

#include <QApplication>
#include <QDesktopServices>
#include <QSqlQuery>

#include "globals/Globals.h"
#include "media_center/KodiXml.h"
#include "media_center/MediaCenterInterface.h"
#include "scrapers/image/FanartTv.h"
#include "scrapers/image/TmdbImages.h"
#include "scrapers/music/UniversalMusicScraper.h"
#include "scrapers/trailer/HdTrailers.h"

// TODO: Move manager to UI folder
#include "ui/export/csv_export/CsvExportModule.h"

Manager::Manager(QObject* parent) : QObject(parent)
{
    using namespace mediaelch::scraper;

    m_settings = Settings::instance();
    m_scraperManager = new mediaelch::ScraperManager(*m_settings, this);

    m_movieFileSearcher = new mediaelch::MovieFileSearcher(this);
    m_tvShowFileSearcher = new TvShowFileSearcher(this);
    m_concertFileSearcher = new ConcertFileSearcher(this);
    m_musicFileSearcher = new MusicFileSearcher(this);
    m_movieModel = new MovieModel(this);
    m_tvShowModel = new TvShowModel(this);
    m_concertModel = new ConcertModel(this);
    m_musicModel = new MusicModel(this);
    m_database = new Database(this);

    m_kodiSettings = new mediaelch::KodiSettings(*m_settings, this);
    m_kodiSettings->init();
    m_mediaCenters.append(new KodiXml(*m_kodiSettings, this));
    m_mediaCentersTvShow.append(new KodiXml(*m_kodiSettings, this));
    m_mediaCentersConcert.append(new KodiXml(*m_kodiSettings, this));

    m_trailerProviders.append(new HdTrailers(this));

    m_iconFont = new MyIconFont(this);
    m_iconFont->initFontAwesome();

    m_csvExportModule = new mediaelch::exporter::CsvExportModule(*Settings::instance());
    m_csvExportModule->onInit();
}

Manager* Manager::instance()
{
    static auto* s_instance = new Manager(QApplication::instance());
    return s_instance;
}

mediaelch::ScraperManager& Manager::scrapers()
{
    return *m_scraperManager;
}

/**
 * \brief Returns the active MediaCenterInterface
 * \return Instance of a MediaCenterInterface
 */
MediaCenterInterface* Manager::mediaCenterInterface()
{
    return m_mediaCenters.at(0);
}

/**
 * \brief Returns the active MediaCenterInterface for TV Shows
 * \return Instance of a MediaCenterInterface
 */
MediaCenterInterface* Manager::mediaCenterInterfaceTvShow()
{
    return m_mediaCentersTvShow.at(0);
}

/**
 * \brief Returns the active MediaCenterInterface for Concerts
 * \return Instance of a MediaCenterinterface
 */
MediaCenterInterface* Manager::mediaCenterInterfaceConcert()
{
    return m_mediaCentersConcert.at(0);
}

/**
 * \brief Returns an instance of the movie file searcher
 * \return Instance of movie searcher
 */
mediaelch::MovieFileSearcher* Manager::movieFileSearcher()
{
    return m_movieFileSearcher;
}

/**
 * \brief Returns an instance of the TV show file searcher
 * \return Instance of TV show file searcher
 */
TvShowFileSearcher* Manager::tvShowFileSearcher()
{
    return m_tvShowFileSearcher;
}

/**
 * \brief Returns an instance of the concert file searcher
 * \return Instance of TV show file searcher
 */
ConcertFileSearcher* Manager::concertFileSearcher()
{
    return m_concertFileSearcher;
}

MusicFileSearcher* Manager::musicFileSearcher()
{
    return m_musicFileSearcher;
}

/**
 * \brief Returns an instance of the MovieModel
 * \return Instance of the MovieModel
 */
MovieModel* Manager::movieModel()
{
    return m_movieModel;
}

/**
 * \brief Returns an instance of the TvShowModel
 * \return Instance of the TvShowModel
 */
TvShowModel* Manager::tvShowModel()
{
    return m_tvShowModel;
}

/**
 * \brief Returns an instance of the ConcertModel
 * \return Instance of the ConcertModel
 */
ConcertModel* Manager::concertModel()
{
    return m_concertModel;
}

MusicModel* Manager::musicModel()
{
    return m_musicModel;
}

mediaelch::scraper::FanartTv* Manager::fanartTv()
{
    using namespace mediaelch::scraper;
    auto* scraper = dynamic_cast<FanartTv*>(m_scraperManager->imageProvider(FanartTv::ID));
    MediaElch_Ensures(scraper != nullptr);
    return scraper;
}

Database* Manager::database()
{
    return m_database;
}

void Manager::setTvShowFilesWidget(TvShowFilesWidget* widget)
{
    m_tvShowFilesWidget = widget;
}

TvShowFilesWidget* Manager::tvShowFilesWidget()
{
    return m_tvShowFilesWidget;
}

void Manager::setMusicFilesWidget(MusicFilesWidget* widget)
{
    m_musicFilesWidget = widget;
}

MusicFilesWidget* Manager::musicFilesWidget()
{
    return m_musicFilesWidget;
}

FileScannerDialog* Manager::fileScannerDialog()
{
    return m_fileScannerDialog;
}

void Manager::setFileScannerDialog(FileScannerDialog* dialog)
{
    m_fileScannerDialog = dialog;
}

QVector<mediaelch::scraper::TrailerProvider*> Manager::trailerProviders()
{
    return m_trailerProviders;
}

MyIconFont* Manager::iconFont()
{
    return m_iconFont;
}

mediaelch::exporter::CsvExportModule& Manager::csvExportModule()
{
    MediaElch_Ensures(m_csvExportModule != nullptr);
    return *m_csvExportModule;
}

mediaelch::KodiSettings* Manager::kodiSettings()
{
    return m_kodiSettings;
}
