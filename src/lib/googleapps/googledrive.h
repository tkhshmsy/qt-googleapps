#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include "googleapps_global.h"
#include "abstractgoogleapps.h"

#include <QtCore/QVariantMap>
class QString;
class QNetworkReply;
class GoogleOAuth2;

class GOOGLEAPPSSHARED_EXPORT GoogleDrive : public AbstractGoogleApps
{
    Q_OBJECT
public:
    explicit GoogleDrive(GoogleOAuth2 *GoogleOAuth2, QObject *parent = 0);
    ~GoogleDrive();

    static const QString scope();

    //TODO: filesCopy
    QNetworkReply *filesCreate(const QString &filename, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesDelete(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesEmptyTrash(const QVariantMap &parameters = QVariantMap());
    //TODO: filesExport
    QNetworkReply *filesGenerateIds(const QVariantMap &parameters = QVariantMap());
    enum class FilesGetTarget : int {
        Resource,
        Content,
    };
    QNetworkReply *filesGet(const QString &fileId, const FilesGetTarget &target, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesList(const QVariantMap &parameters = QVariantMap());
    //TODO: filesUpdate
    //TODO: filesWatch
};

#endif // GOOGLEDRIVE_H
