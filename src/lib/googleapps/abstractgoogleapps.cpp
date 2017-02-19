#include "abstractgoogleapps.h"

#include <QtCore/QDebug>
#include <QtCore/QEventLoop>
#include <QtCore/QIODevice>
#include <QtCore/QByteArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>

#include "googleoauth2.h"

class AbstractGoogleApps::Private
{
public:
    Private(GoogleOAuth2 *oauth2);
    ~Private();

    GoogleOAuth2 *oauth2;
};

AbstractGoogleApps::Private::Private(GoogleOAuth2 *oauth2)
    : oauth2(oauth2)
{
    if (!oauth2->isAuthorized()) {
        oauth2->grant();
        oauth2->waitAuthorization();
        if (!oauth2->isAuthorized()) {
            qWarning() << "failed to authorize";
        }
    }
}

AbstractGoogleApps::Private::~Private()
{}

AbstractGoogleApps::AbstractGoogleApps(GoogleOAuth2 *googleOAuth2, QObject *parent)
    : QObject(parent)
    , d(new Private(googleOAuth2))
{}

AbstractGoogleApps::~AbstractGoogleApps()
{
    delete d;
}

const QString AbstractGoogleApps::scope()
{
    return QString();
}

QByteArray AbstractGoogleApps::waitReplyRAW(QNetworkReply *reply)
{
    QByteArray ret;
    QEventLoop eventLoop;
    auto connection = QObject::connect(reply, &QNetworkReply::finished,
                                       [&](){
        eventLoop.quit();
        ret = reply->readAll();
        reply->deleteLater();
    });

    eventLoop.exec();

    QObject::disconnect(connection);
    return ret;
}

QJsonObject AbstractGoogleApps::waitReplyJson(QNetworkReply *reply, bool *ok)
{
    QJsonObject ret;
    QByteArray content = AbstractGoogleApps::waitReplyRAW(reply);
    if (content.size() > 0) {
        QJsonDocument doc = QJsonDocument::fromJson(content);
        if (!doc.isNull() && doc.isObject()) {
            ret = doc.object();
            if (!ret.contains(QStringLiteral("error"))) {
                if (ok != Q_NULLPTR) {
                    *ok = true;
                }
                return ret;
            }
        }
    }
    if (ok != Q_NULLPTR) {
        *ok = false;
    }
    return ret;
}

QNetworkRequest AbstractGoogleApps::createRequest(const QUrl &url, const QVariantMap &parameters)
{
    return d->oauth2->createRequest(url, parameters);
}

QNetworkReply *AbstractGoogleApps::head(const QNetworkRequest &request)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->head(request);
}

QNetworkReply *AbstractGoogleApps::get(const QNetworkRequest &request)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->get(request);
}

QNetworkReply *AbstractGoogleApps::post(const QNetworkRequest &request, QIODevice *data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->post(request, data);
}

QNetworkReply *AbstractGoogleApps::post(const QNetworkRequest &request, const QByteArray &data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->post(request, data);
}

QNetworkReply *AbstractGoogleApps::post(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->post(request, multiPart);
}

QNetworkReply *AbstractGoogleApps::put(const QNetworkRequest &request, QIODevice *data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->put(request, data);
}

QNetworkReply *AbstractGoogleApps::put(const QNetworkRequest &request, const QByteArray &data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->put(request, data);
}

QNetworkReply *AbstractGoogleApps::put(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->put(request, multiPart);
}

QNetworkReply *AbstractGoogleApps::deleteResource(const QNetworkRequest &request)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->deleteResource(request);
}

QNetworkReply *AbstractGoogleApps::patch(const QNetworkRequest &request, QIODevice *data)
{
    return this->sendCustomRequest(request, QByteArray("PATCH"), data);
}

QNetworkReply *AbstractGoogleApps::patch(const QNetworkRequest &request, const QByteArray &data)
{
    return this->sendCustomRequest(request, QByteArray("PATCH"), data);
}

QNetworkReply *AbstractGoogleApps::patch(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    return this->sendCustomRequest(request, QByteArray("PATCH"), multiPart);
}

QNetworkReply *AbstractGoogleApps::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QIODevice *data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->sendCustomRequest(request, verb, data);
}

QNetworkReply *AbstractGoogleApps::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->sendCustomRequest(request, verb, data);
}

QNetworkReply *AbstractGoogleApps::sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QHttpMultiPart *multiPart)
{
    if (!d->oauth2->checkAndUpdateTokens()) {
        qWarning() << "failed to update Tokens";
        return Q_NULLPTR;
    }
    return d->oauth2->networkAccessManager()->sendCustomRequest(request, verb, multiPart);
}
