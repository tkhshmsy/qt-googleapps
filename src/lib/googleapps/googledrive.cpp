#include "googledrive.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeType>
#include <QtCore/QMimeDatabase>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>

#include "googleoauth2.h"

const QString ContentTypeHeaderJSON = QStringLiteral("application/json;charset=UTF-8");

namespace API {
const QString scope                    = QStringLiteral("https://www.googleapis.com/auth/drive");
const QString baseUrl                  = QStringLiteral("https://www.googleapis.com/drive/v3");
const QString AboutGet                 = baseUrl + QStringLiteral("/about");

const QString ChangesGetStartPageToken = baseUrl + QStringLiteral("/changes/startPageToken");
const QString ChangesList              = baseUrl + QStringLiteral("/changes");
const QString ChangesWatch             = baseUrl + QStringLiteral("/changes/watch");
const QString ChannelsStop             = baseUrl + QStringLiteral("/channels/Stop");

const QString CommentsCreate           = baseUrl + QStringLiteral("/files/%1/comments");
const QString CommentsDelete           = baseUrl + QStringLiteral("/files/%1/comments/%2");
const QString CommentsGet              = baseUrl + QStringLiteral("/files/%1/comments/%2");
const QString CommentsList             = baseUrl + QStringLiteral("/files/%1/comments");
const QString CommentsUpdate           = baseUrl + QStringLiteral("/files/%1/comments/%2");

const QString FilesCopy                = baseUrl + QStringLiteral("/files/%1/copy");
const QString FilesCreate              = baseUrl + QStringLiteral("/files");
const QString FilesCreateMedia         = QStringLiteral("https://www.googleapis.com/upload/drive/v3/files");
const QString FilesDelete              = baseUrl + QStringLiteral("/files/%1");
const QString FilesEmptyTrash          = baseUrl + QStringLiteral("/files/trash");
const QString FilesExport              = baseUrl + QStringLiteral("/files/%1/export");
const QString FilesGenerateIds         = baseUrl + QStringLiteral("/files/generateIds");
const QString FilesGet                 = baseUrl + QStringLiteral("/files/%1");
const QString FilesList                = baseUrl + QStringLiteral("/files");
const QString FilesUpdate              = baseUrl + QStringLiteral("/files/%1");
const QString FilesUpdateMedia         = QStringLiteral("https://www.googleapis.com/upload/drive/v3/files/%1");
const QString FilesWatch               = baseUrl + QStringLiteral("/files/%1/watch");

const QString PermissionsCreate        = baseUrl + QStringLiteral("/files/%1/permissions");
const QString PermissionsDelete        = baseUrl + QStringLiteral("/files/%1/permissions/%2");
const QString PermissionsGet           = baseUrl + QStringLiteral("/files/%1/permissions/%2");
const QString PermissionsList          = baseUrl + QStringLiteral("/files/%1/permissions");
const QString PermissionsUpdate        = baseUrl + QStringLiteral("/files/%1/permissions/%2");

const QString RepliesCreate            = baseUrl + QStringLiteral("/files/%1/comments/%2/replies");
const QString RepliesDelete            = baseUrl + QStringLiteral("/files/%1/comments/%2/replies/%3");
const QString RepliesGet               = baseUrl + QStringLiteral("/files/%1/comments/%2/replies/%3");
const QString RepliesList              = baseUrl + QStringLiteral("/files/%1/comments/%2/replies");
const QString RepliesUpdate            = baseUrl + QStringLiteral("/files/%1/comments/%2/replies/%3");

const QString RevisionsDelete          = baseUrl + QStringLiteral("/files/%1/revisions/%2");
const QString RevisionsGet             = baseUrl + QStringLiteral("/files/%1/revisions/%2");
const QString RevisionsList            = baseUrl + QStringLiteral("/files/%1/revisions");
const QString RevisionsUpdate          = baseUrl + QStringLiteral("/files/%1/revisions/%2");
}

GoogleDrive::GoogleDrive(GoogleOAuth2 *googleOAuth2, QObject *parent)
    : AbstractGoogleApps(googleOAuth2, parent)
{}

GoogleDrive::~GoogleDrive()
{}

const QString GoogleDrive::scope()
{
    return API::scope;
}

QNetworkReply *GoogleDrive::AboutGet(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::AboutGet), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::ChangesGetStartPageToken(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::ChangesGetStartPageToken), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::ChangesList(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::ChangesList), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::ChangesWatch(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    if (!parameters.contains(QStringLiteral("pageToken"))) {
        qWarning() << Q_FUNC_INFO << "missing required parameter: pageToken";
    }
    QNetworkRequest request = this->createRequest(QUrl(API::ChangesWatch), parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::ChannelsStop(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::ChannelsStop), parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::CommentsCreate(const QString &fileId, const QJsonObject &commentsResource, const QVariantMap &parameters)
{
    if (!commentsResource.contains(QStringLiteral("content"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: content";
    }
    QUrl url(QString(API::CommentsCreate).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(commentsResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::CommentsDelete(const QString &fileId, const QString &commentId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CommentsDelete).arg(fileId).arg(commentId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::CommentsGet(const QString &fileId, const QString &commentId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CommentsGet).arg(fileId).arg(commentId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::CommentsList(const QString &fileId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CommentsList).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::CommentsUpdate(const QString &fileId, const QString &commentId, const QJsonObject &commentsResource, const QVariantMap &parameters)
{
    if (!commentsResource.contains(QStringLiteral("content"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: content";
    }
    QUrl url(QString(API::CommentsUpdate).arg(fileId).arg(commentId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(commentsResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::filesCopy(const QString &fileId, const QJsonObject &fileResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesCopy).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(fileResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::filesCreate(const QJsonObject &fileResource, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesCreate), parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(fileResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::filesCreateMedia(const QJsonObject &fileResource, const QByteArray &content, const QVariantMap &parameters)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);
    multiPart->setBoundary(QByteArray("###boundary###"));
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForData(content);

    QHttpPart resourcePart;
    {
        resourcePart.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);
        resourcePart.setBody(QByteArray(QJsonDocument(fileResource).toJson()));
    }
    QHttpPart contentPart;
    {
        contentPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
        contentPart.setBody(content);
    }
    multiPart->append(resourcePart);
    multiPart->append(contentPart);

    QVariantMap p(parameters);
    p.insert(QStringLiteral("uploadType"), QStringLiteral("multipart"));

    QNetworkRequest request = this->createRequest(QUrl(API::FilesCreateMedia), p);
    QNetworkReply *reply = this->post(request, multiPart);
    multiPart->setParent(reply); //delete with reply
    return reply;
}

QNetworkReply *GoogleDrive::filesCreateMedia(const QJsonObject &fileResource, const QString &filename, const QVariantMap &parameters)
{
    QFile *file = new QFile(filename);
    if (!file->exists()) {
        qWarning() << Q_FUNC_INFO << "file not found:" << filename;
        delete file;
        return Q_NULLPTR;
    }
    if (!file->open(QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "failed to open:" << filename;
        delete file;
        return Q_NULLPTR;
    }
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);
    multiPart->setBoundary(QByteArray("###boundary###"));
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(filename);

    QHttpPart resourcePart;
    {
        resourcePart.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);
        if (!fileResource.isEmpty()) {
            resourcePart.setBody(QByteArray(QJsonDocument(fileResource).toJson()));
        } else {
            QJsonObject metadata;
            QFileInfo fileInfo(*file);
            metadata.insert(QStringLiteral("name"), fileInfo.fileName());
            metadata.insert(QStringLiteral("mimeType"), mimeType.name());
            metadata.insert(QStringLiteral("description"), QStringLiteral("upload by qt-googleapps"));
            resourcePart.setBody(QByteArray(QJsonDocument(metadata).toJson()));
        }
    }
    QHttpPart contentPart;
    {
        contentPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
        contentPart.setBodyDevice(file);
        file->setParent(multiPart); //delete with multiPart
    }
    multiPart->append(resourcePart);
    multiPart->append(contentPart);

    QVariantMap p(parameters);
    p.insert(QStringLiteral("uploadType"), QStringLiteral("multipart"));

    QNetworkRequest request = this->createRequest(QUrl(API::FilesCreateMedia), p);
    QNetworkReply *reply = this->post(request, multiPart);
    multiPart->setParent(reply); //delete with reply
    return reply;
}

QNetworkReply *GoogleDrive::filesDelete(const QString &fileId, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesDelete).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesEmptyTrash(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesEmptyTrash), parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesExport(const QString &fileId, const QVariantMap &parameters)
{
    if (!parameters.contains(QStringLiteral("mimeType"))) {
        qWarning() << Q_FUNC_INFO << "missing required parameter: mimeType";
    }
    QUrl url(QString(API::FilesExport).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesGenerateIds(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesGenerateIds), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesGet(const QString &fileId, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesGet).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesGetMedia(const QString &fileId, const QVariantMap &parameters)
{
    QVariantMap p(parameters);
    p.insert(QStringLiteral("alt"), QStringLiteral("media"));
    return this->filesGet(fileId, p);
}

QNetworkReply *GoogleDrive::filesList(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesList), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesUpdate(const QString &fileId, const QJsonObject &fileResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesUpdate).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(fileResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::filesUpdateMedia(const QString &fileId, const QJsonObject &fileResource, const QByteArray &media, const QVariantMap &parameters)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);
    multiPart->setBoundary(QByteArray("###boundary###"));
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForData(media);

    QHttpPart resourcePart;
    {
        resourcePart.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);
        resourcePart.setBody(QByteArray(QJsonDocument(fileResource).toJson()));
    }
    QHttpPart mediaPart;
    {
        mediaPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
        mediaPart.setBody(media);
    }
    multiPart->append(resourcePart);
    multiPart->append(mediaPart);

    QVariantMap p(parameters);
    p.insert(QStringLiteral("uploadType"), QStringLiteral("multipart"));

    QUrl url(QString(API::FilesUpdateMedia).arg(fileId));
    QNetworkRequest request = this->createRequest(url, p);
    QNetworkReply *reply = this->patch(request, multiPart);
    multiPart->setParent(reply); //delete with reply
    return reply;
}

QNetworkReply *GoogleDrive::filesUpdateMedia(const QString &fileId, const QJsonObject &fileResource, const QString &filename, const QVariantMap &parameters)
{
    QFile *file = new QFile(filename);
    if (!file->exists()) {
        qWarning() << Q_FUNC_INFO << "file not found:" << filename;
        delete file;
        return Q_NULLPTR;
    }
    if (!file->open(QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "failed to open:" << filename;
        delete file;
        return Q_NULLPTR;
    }
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);
    multiPart->setBoundary(QByteArray("###boundary###"));
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(filename);

    QHttpPart resourcePart;
    {
        resourcePart.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);
        if (!fileResource.isEmpty()) {
            resourcePart.setBody(QByteArray(QJsonDocument(fileResource).toJson()));
        } else {
            QJsonObject metadata;
            QFileInfo fileInfo(*file);
            metadata.insert(QStringLiteral("name"), fileInfo.fileName());
            metadata.insert(QStringLiteral("mimeType"), mimeType.name());
            metadata.insert(QStringLiteral("description"), QStringLiteral("upload by qt-googleapps"));
            resourcePart.setBody(QByteArray(QJsonDocument(metadata).toJson()));
        }
    }
    QHttpPart contentPart;
    {
        contentPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
        contentPart.setBodyDevice(file);
        file->setParent(multiPart); //delete with multiPart
    }
    multiPart->append(resourcePart);
    multiPart->append(contentPart);

    QVariantMap p(parameters);
    p.insert(QStringLiteral("uploadType"), QStringLiteral("multipart"));

    QUrl url(QString(API::FilesUpdateMedia).arg(fileId));
    QNetworkRequest request = this->createRequest(url, p);
    QNetworkReply *reply = this->patch(request, multiPart);
    multiPart->setParent(reply); //delete with reply
    return reply;
}

QNetworkReply *GoogleDrive::filesWatch(const QString &fileId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesWatch).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}


QNetworkReply *GoogleDrive::permissionsCreate(const QString &fileId, const QJsonObject &permissionsResource, const QVariantMap &parameters)
{
    if (!permissionsResource.contains(QStringLiteral("role"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: role";
    }
    if (!permissionsResource.contains(QStringLiteral("type"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: type";
    }
    QUrl url(QString(API::PermissionsCreate).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(permissionsResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::permissionsDelete(const QString &fileId, const QString &permissionId, const QVariantMap &parameters)
{
    QUrl url(QString(API::PermissionsDelete).arg(fileId).arg(permissionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::permissionsGet(const QString &fileId, const QString &permissionId, const QVariantMap &parameters)
{
    QUrl url(QString(API::PermissionsGet).arg(fileId).arg(permissionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::permissionsList(const QString &fileId, const QVariantMap &parameters)
{
    QUrl url(QString(API::PermissionsList).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::permissionsUpdate(const QString &fileId, const QString &permissionId, const QJsonObject &permissionsResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::PermissionsUpdate).arg(fileId).arg(permissionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(permissionsResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}


QNetworkReply *GoogleDrive::RepliesCreate(const QString &fileId, const QString &commentId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QUrl url(QString(API::RepliesCreate).arg(fileId).arg(commentId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::RepliesDelete(const QString &fileId, const QString &commentId, const QString &replyId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RepliesDelete).arg(fileId).arg(commentId).arg(replyId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::RepliesGet(const QString &fileId, const QString &commentId, const QString &replyId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RepliesGet).arg(fileId).arg(commentId).arg(replyId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::RepliesList(const QString &fileId, const QString &commentId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RepliesList).arg(fileId).arg(commentId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::RepliesUpdate(const QString &fileId, const QString &commentId, const QString &replyId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    if (!requestBody.contains(QStringLiteral("content"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: content";
    }
    QUrl url(QString(API::RepliesUpdate).arg(fileId).arg(commentId).arg(replyId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleDrive::RevisionsDelete(const QString &fileId, const QString &revisionId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RevisionsDelete).arg(fileId).arg(revisionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleDrive::RevisionsGet(const QString &fileId, const QString &revisionId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RevisionsGet).arg(fileId).arg(revisionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::RevisionsList(const QString &fileId, const QVariantMap &parameters)
{
    QUrl url(QString(API::RevisionsList).arg(fileId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::RevisionsUpdate(const QString &fileId, const QString &revisionId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QUrl url(QString(API::RevisionsUpdate).arg(fileId).arg(revisionId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}
