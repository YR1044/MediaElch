#ifndef HELPER_H
#define HELPER_H

#include <QImage>
#include <QString>

/**
 * @brief Some convenience functions are bundled here
 */
class Helper
{
public:
    static QString toLatin1PercentEncoding(QString str);
    static QString urlFromEncoded(QString str);
    static QString formatTrailerUrl(QString url);
    static bool isDvd(QString path);
    static bool isBluRay(QString path);
    static QImage &resizeBackdrop(QImage &image, bool &resized);
    static QByteArray &resizeBackdrop(QByteArray &image);
    static QString &sanitizeFileName(QString &fileName);
    static QString stackedBaseName(const QString &fileName);
    static QString appendArticle(const QString &text);
    static QString mapGenre(const QString &text);
    static QStringList mapGenre(const QStringList &genres);
};

#endif // HELPER_H
