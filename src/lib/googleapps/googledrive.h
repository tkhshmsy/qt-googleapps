#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include "googleapps_global.h"
#include "abstractgoogleapps.h"

#include <QtCore/QVariantMap>
class QString;
class QByteArray;
class QJsonObject;
class QNetworkReply;
class GoogleOAuth2;

class GOOGLEAPPSSHARED_EXPORT GoogleDrive : public AbstractGoogleApps
{
    Q_OBJECT
public:
    explicit GoogleDrive(GoogleOAuth2 *GoogleOAuth2, QObject *parent = 0);
    ~GoogleDrive();

    static const QString scope();

    //About
    QNetworkReply *AboutGet(const QVariantMap &parameters = QVariantMap());
    //Changes
    QNetworkReply *ChangesGetStartPageToken(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *ChangesList(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *ChangesWatch(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Channels
    QNetworkReply *ChannelsStop(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Comments
    QNetworkReply *CommentsCreate(const QString &fileId, const QJsonObject &commentsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CommentsDelete(const QString &fileId, const QString &commentId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CommentsGet(const QString &fileId, const QString &commentId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CommentsList(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CommentsUpdate(const QString &fileId, const QString &commentId, const QJsonObject &commentsResource, const QVariantMap &parameters = QVariantMap());
    //Files
    QNetworkReply *filesCopy(const QString &fileId, const QJsonObject &fileResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesCreate(const QJsonObject &fileResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesCreateMedia(const QJsonObject &fileResource, const QByteArray &media, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesCreateMedia(const QJsonObject &fileResource, const QString &filename, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesDelete(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesEmptyTrash(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesExport(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesGenerateIds(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesGet(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesGetMedia(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesList(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesUpdate(const QString &fileId, const QJsonObject &fileResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesUpdateMedia(const QString &fileId, const QJsonObject &fileResource, const QByteArray &media, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesUpdateMedia(const QString &fileId, const QJsonObject &fileResource, const QString &filename, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *filesWatch(const QString &fileId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Permissions
    QNetworkReply *permissionsCreate(const QString &fileId, const QJsonObject &permissionsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *permissionsDelete(const QString &fileId, const QString &permissionId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *permissionsGet(const QString &fileId, const QString &permissionId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *permissionsList(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *permissionsUpdate(const QString &fileId, const QString &permissionId, const QJsonObject &permissionsResource, const QVariantMap &parameters = QVariantMap());
    //Replies
    QNetworkReply *RepliesCreate(const QString &fileId, const QString &commentId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RepliesDelete(const QString &fileId, const QString &commentId, const QString &replyId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RepliesGet(const QString &fileId, const QString &commentId, const QString &replyId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RepliesList(const QString &fileId, const QString &commentId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RepliesUpdate(const QString &fileId, const QString &commentId, const QString &replyId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Revisions
    QNetworkReply *RevisionsDelete(const QString &fileId, const QString &revisionId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RevisionsGet(const QString &fileId, const QString &revisionId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RevisionsList(const QString &fileId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *RevisionsUpdate(const QString &fileId, const QString &revisionId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
};

#endif // GOOGLEDRIVE_H
