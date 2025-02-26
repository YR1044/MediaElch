#pragma once

#include "media/FileFilter.h"

#include <QDir>
#include <QString>
#include <QStringList>
#include <QVector>

class Movie;
class RenamerDialog;


enum class RenameType : int8_t
{
    Movies,
    TvShows,
    Concerts,
    All
};

QString renamerTypeToString(RenameType type);

struct RenamerConfig
{
    QString filePattern;
    QString filePatternMulti;
    QString directoryPattern;
    QString delimiter;
    bool renameFiles = false;
    bool renameDirectories = false;
    bool replaceDelimiter = false;
    bool dryRun = false;
};

class Renamer
{
public:
    enum class RenameResult : int8_t
    {
        Failed,
        Success
    };
    enum class RenameOperation : int8_t
    {
        CreateDir,
        Move,
        Rename
    };
    enum class RenameError : int8_t
    {
        None, // Todo: Be more specific about what error occurred
        Error
    };

    Renamer(RenamerConfig config, RenamerDialog* dialog);

    static QString replace(QString& text, const QString& search, QString replacement);

    static bool rename(QDir& dir, QString newName);
    static bool rename(const QString& file, const QString& newName);

protected:
    void replaceDelimiter(QString& text) const;

protected:
    RenamerConfig m_config;
    RenamerDialog* m_dialog;
    const mediaelch::FileFilter& m_extraFiles;
};
