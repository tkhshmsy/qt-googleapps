#include "googledrive.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeType>
#include <QtCore/QMimeDatabase>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>

#include "googleoauth2.h"

namespace API {
const QString scope            = QStringLiteral("https://www.googleapis.com/auth/drive");

const QString FilesCopy        = QStringLiteral("https://www.googleapis.com/drive/v3/files/%1/copy");
const QString FilesCreate      = QStringLiteral("https://www.googleapis.com/upload/drive/v3/files");
const QString FilesDelete      = QStringLiteral("https://www.googleapis.com/drive/v3/files/%1");
const QString FilesEmptyTrash  = QStringLiteral("https://www.googleapis.com/drive/v3/files/trash");
const QString FilesExport      = QStringLiteral("https://www.googleapis.com/drive/v3/files/%1/export");
const QString FilesGenerateIds = QStringLiteral("https://www.googleapis.com/drive/v3/files/generateIds");
const QString FilesGet         = QStringLiteral("https://www.googleapis.com/drive/v3/files/%1");
const QString FilesList        = QStringLiteral("https://www.googleapis.com/drive/v3/files");
const QString FilesUpdate      = QStringLiteral("https://www.googleapis.com/upload/drive/v3/files/%1");
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

QNetworkReply *GoogleDrive::filesCreate(const QString &filename, const QVariantMap &parameters)
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

    QHttpPart metadataPart;
    {
        metadataPart.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json;charset=UTF-8"));
        QJsonObject metadata;
        QFileInfo fileInfo(*file);
        metadata.insert(QStringLiteral("name"), fileInfo.fileName());
        metadata.insert(QStringLiteral("mimeType"), mimeType.name());
        metadata.insert(QStringLiteral("description"), QStringLiteral("upload by GoogleDrive"));
        metadataPart.setBody(QByteArray(QJsonDocument(metadata).toJson()));
    }
    QHttpPart contentPart;
    {
        contentPart.setHeader(QNetworkRequest::ContentTypeHeader, mimeType.name());
        contentPart.setBodyDevice(file);
        file->setParent(multiPart); //delete with multiPart
    }
    multiPart->append(metadataPart);
    multiPart->append(contentPart);

    QVariantMap p(parameters);
    p.insert(QStringLiteral("uploadType"), QStringLiteral("multipart"));

    QNetworkRequest request = this->createRequest(QUrl(API::FilesCreate), p);
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

QNetworkReply *GoogleDrive::filesGenerateIds(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesGenerateIds), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesGet(const QString &fileId, const FilesGetTarget &target, const QVariantMap &parameters)
{
    QUrl url(QString(API::FilesGet).arg(fileId));
    QVariantMap p(parameters);
    if (target == FilesGetTarget::Content) {
            p.insert(QStringLiteral("alt"), QStringLiteral("media"));
    }
    QNetworkRequest request = this->createRequest(url, p);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleDrive::filesList(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FilesList), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}
