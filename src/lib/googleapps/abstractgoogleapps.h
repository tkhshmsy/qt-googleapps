#ifndef ABSTRACTGOOGLEAPPS_H
#define ABSTRACTGOOGLEAPPS_H

#include "googleapps_global.h"
#include <QtCore/QObject>
#include <QtCore/QVariantMap>
class QJsonObject;
class QIODevice;
class QHttpMultiPart;
class QByteArray;
class QNetworkRequest;
class QNetworkReply;
class GoogleOAuth2;

#include "googleoauth2.h"

class GOOGLEAPPSSHARED_EXPORT AbstractGoogleApps : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGoogleApps(GoogleOAuth2 *googleOAuth2, QObject *parent = 0);
    ~AbstractGoogleApps();

    static const QString scope();
    static QByteArray waitReplyRAW(QNetworkReply *reply);
    static QJsonObject waitReplyJson(QNetworkReply *reply, bool *ok = Q_NULLPTR);

public Q_SLOTS:

Q_SIGNALS:
    void error();

protected:
    QNetworkRequest createRequest(const QUrl &url, const QVariantMap &parameters = QVariantMap());

    QNetworkReply *head(const QNetworkRequest &request);
    QNetworkReply *get(const QNetworkRequest &request);
    QNetworkReply *post(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *post(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *put(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *deleteResource(const QNetworkRequest &request);
    //TODO: PATCH i/f

    QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QIODevice *data = Q_NULLPTR);
    QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data);
    QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QHttpMultiPart *multiPart);

private:
    class Private;
    Private *d;
};

#endif // ABSTRACTGOOGLEAPPS_H
