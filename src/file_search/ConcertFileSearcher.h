#pragma once

#include "database/Database.h"
#include "globals/MediaDirectory.h"

#include <QDir>
#include <QString>
#include <QStringList>
#include <QVector>

class Concert;

class ConcertFileSearcher : public QObject
{
    Q_OBJECT
public:
    explicit ConcertFileSearcher(QObject* parent = nullptr);
    void setConcertDirectories(QVector<mediaelch::MediaDirectory> directories);

public slots:
    void reload(bool force);
    void abort();

signals:
    void searchStarted(QString);
    void progress(int, int, int);
    void concertsLoaded();
    void currentDir(QString);

private:
    QVector<mediaelch::MediaDirectory> m_directories;
    int m_progressMessageId;
    bool m_aborted = false;

private:
    void clearOldConcerts(bool forceClear);

    QVector<QStringList> loadContentsFromDiskIfRequired(bool forceReload);
    QVector<Concert*> loadConcertsFromDatabase();

    void storeContentsInDatabase(const QVector<QStringList>& contents);
    void setupDatabaseConcerts(const QVector<Concert*>& concerts);
    void addConcertsToGui(const QVector<Concert*>& concerts);

    void scanDir(QString startPath,
        QString path,
        QVector<QStringList>& contents,
        bool separateFolders = false,
        bool firstScan = false);

    QStringList getFiles(mediaelch::DirectoryPath path);
};
