#include "googleoauth2.h"

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QPointer>
#include <QtCore/QVariantMap>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QEventLoop>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QAuthenticator>
#include <QtNetworkAuth/QAbstractOAuthReplyHandler>

namespace Key {
const QString accessToken =        QStringLiteral("access_token");
const QString apiKey =             QStringLiteral("api_key");
const QString clientIdentifier =   QStringLiteral("client_id");
const QString clientSharedSecret = QStringLiteral("client_secret");
const QString code =               QStringLiteral("code");
const QString error =              QStringLiteral("error");
const QString errorDescription =   QStringLiteral("error_description");
const QString errorUri =           QStringLiteral("error_uri");
const QString expiresIn =          QStringLiteral("expires_in");
const QString grantType =          QStringLiteral("grant_type");
const QString redirectUri =        QStringLiteral("redirect_uri");
const QString refreshToken =       QStringLiteral("refresh_token");
const QString responseType =       QStringLiteral("response_type");
const QString scope =              QStringLiteral("scope");
const QString state =              QStringLiteral("state");
const QString tokenType =          QStringLiteral("token_type");

const QString authUri =            QStringLiteral("auth_uri");
const QString tokenUri =           QStringLiteral("token_uri");
const QString expiresAt =           QStringLiteral("expires_at");
}

class GoogleOAuth2::Private : public QObject
{
public:
    Private(GoogleOAuth2 *parent);
    ~Private();
private:
    GoogleOAuth2 *q;

public:
    QUrl accessTokenUrl;
    QString refreshToken;
    QDateTime expirationAt;

    QString tokenType;
    QPointer<QNetworkReply> currentReply;

public Q_SLOTS:
    void handleCallback(const QVariantMap &data);
    void accessTokenRequestFinished(const QVariantMap &values);
    void authenticate(QNetworkReply *reply, QAuthenticator *authenticaor);
};

GoogleOAuth2::Private::Private(GoogleOAuth2 *parent)
    : QObject(parent)
    , q(parent)
    , accessTokenUrl()
    , refreshToken()
    , expirationAt()
    , tokenType()
    , currentReply()
{}

GoogleOAuth2::Private::~Private()
{}

void GoogleOAuth2::Private::handleCallback(const QVariantMap &data)
{
    if (q->status() != QAbstractOAuth::Status::NotAuthenticated) {
        qWarning() << Q_FUNC_INFO << "unexpected call";
        return;
    }
    const QString error = data.value(Key::error).toString();
    const QString code = data.value(Key::code).toString();
    const QString receivedState = data.value(Key::state).toString();
    if (error.size() > 0) {
        const QString uri = data.value(Key::errorUri).toString();
        const QString description = data.value(Key::errorDescription).toString();
        qWarning() << Q_FUNC_INFO << "ERROR:" << qPrintable(error) << qPrintable(uri) << qPrintable(description);
        emit q->error(error, description, uri);
        return;
    }
    if (code.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "no code";
        return;
    }
    if (receivedState.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "no state";
        return;
    }
    if (q->state() != receivedState) {
        qWarning() << Q_FUNC_INFO << "state mismatch";
        return;
    }

    q->setStatus(QAbstractOAuth::Status::TemporaryCredentialsReceived);
    q->requestAccessToken(code);
}

void GoogleOAuth2::Private::accessTokenRequestFinished(const QVariantMap &values)
{
    if (values.contains(Key::error)) {
        const QString error = values.value(Key::error).toString();
        qWarning() << Q_FUNC_INFO << "ERROR:" << qPrintable(error);
        return;
    }

    bool ok;
    const QString accessToken = values.value(Key::accessToken).toString();
    tokenType = values.value(Key::tokenType).toString();
    int expiresIn = values.value(Key::expiresIn).toInt(&ok);
    if (!ok) {
        expiresIn = -1;
    }
    if (accessToken.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "missing accessToken";
        return;
    }
    q->setToken(accessToken);
    if (values.contains(Key::refreshToken)) {
        q->setRefreshToken(values.value(Key::refreshToken).toString());
    }
    if (values.contains(Key::scope)) {
        q->setScope(values.value(Key::scope).toString());
    }
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    if (expiresIn > 0 && currentDateTime.secsTo(expirationAt) != expiresIn) {
        q->setExpirationAt(currentDateTime.addSecs(expiresIn));
    }

    q->setStatus(QAbstractOAuth::Status::Granted);
    emit q->granted();
}

void GoogleOAuth2::Private::authenticate(QNetworkReply *reply, QAuthenticator *authenticaor)
{
    if (reply == currentReply) {
        const auto url = reply->url();
        if (url == accessTokenUrl) {
            authenticaor->setUser(q->clientIdentifier());
            authenticaor->setPassword(q->clientIdentifierSharedKey());
        }
    }
}

GoogleOAuth2::GoogleOAuth2(QObject *parent)
    : QAbstractOAuth2(new QNetworkAccessManager, parent)
    , d(new Private(this))
{}

GoogleOAuth2::~GoogleOAuth2()
{
    delete d;
}

QString GoogleOAuth2::responseType() const
{
    return QStringLiteral("code");
}

QUrl GoogleOAuth2::accessTokenUrl() const
{
    return d->accessTokenUrl;
}

QString GoogleOAuth2::refreshToken() const
{
    return d->refreshToken;
}

QDateTime GoogleOAuth2::expirationAt() const
{
    return d->expirationAt;
}

bool GoogleOAuth2::isAuthorized() const
{
    if (status() == QAbstractOAuth::Status::Granted) {
        return true;
    }
    return false;
}

bool GoogleOAuth2::setClientIdJson(const QJsonObject &json)
{
    const QString keyInstalled(QStringLiteral("installed"));
    if (!json.contains(keyInstalled)){
        qWarning() << Q_FUNC_INFO << "invalid:" << json;
        return false;
    }
    QJsonObject installed = json.value(keyInstalled).toObject();
    QString client_id = installed.value(Key::clientIdentifier).toString();
    QString client_secret = installed.value(Key::clientSharedSecret).toString();
    QString auth_uri = installed.value(Key::authUri).toString();
    QString token_uri = installed.value(Key::tokenUri).toString();

    setAuthorizationUrl(QUrl(auth_uri));
    setAccessTokenUrl(QUrl(token_uri));
    setClientIdentifier(client_id);
    setClientIdentifierSharedKey(client_secret);
    return true;
}

bool GoogleOAuth2::setClientIdJson(const QString &filename)
{
    QFile file(filename);
    if (!file.exists()) {
        qWarning() << Q_FUNC_INFO << "no file:" << filename;
        return false;
    }
    if (!file.open(QFile::Text|QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "failed to open:" << filename;
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject json = doc.object();
    return this-setClientIdJson(json);
}

bool GoogleOAuth2::setTokenJson(const QJsonObject &json)
{
    QString access_token = json.value(Key::accessToken).toString();
    QString refresh_token = json.value(Key::refreshToken).toString();
    QDateTime expires_at = QDateTime::fromString(json.value(Key::expiresAt).toString());

    setToken(access_token);
    setRefreshToken(refresh_token);
    setExpirationAt(expires_at);
    return true;
}

bool GoogleOAuth2::setTokenJson(const QString &filename)
{
    QFile file(filename);
    if (file.exists()) {
        qWarning() << Q_FUNC_INFO << "no file:" << filename;
        return false;
    }
    if (!file.open(QFile::Text|QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "failed to open:" << filename;
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject json = doc.object();
    return this->setTokenJson(json);
}

QJsonObject GoogleOAuth2::tokenJson()
{
    QJsonObject json;
    json.insert(Key::accessToken, this->token());
    json.insert(Key::refreshToken, this->refreshToken());
    json.insert(Key::expiresAt, this->expirationAt().toString());

    return json;
}

bool GoogleOAuth2::waitAuthorization()
{
    bool result = false;
    QEventLoop eventLoop;

    auto grantedConnection = QObject::connect(this, &GoogleOAuth2::granted,
                                              [&](){
        result = true;
        eventLoop.quit();
    });
    auto errorConnection = QObject::connect(this, &GoogleOAuth2::error,
                                            [&](const QString &error, const QString &errorDescription, const QUrl &uri){
        qDebug() << "ERROR:" << error << ":" << errorDescription << " @ " << uri.toString();
        eventLoop.quit();
    });

    eventLoop.exec();

    QObject::disconnect(grantedConnection);
    QObject::disconnect(errorConnection);
    return result;
}

bool GoogleOAuth2::checkAndUpdateTokens()
{
    bool ret = false;
    if (d->expirationAt < QDateTime::currentDateTime()) {
        refreshAccessToken();
        ret = waitAuthorization();
    } else {
        ret = true;
    }
    return ret;
}

QNetworkRequest GoogleOAuth2::createRequest(const QUrl &url, const QVariantMap &parameters)
{
    QUrlQuery query(url.query());
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        QString key = QString::fromLatin1(QUrl::toPercentEncoding(it.key()));
        QString value = QString::fromLatin1(QUrl::toPercentEncoding(it.value().toString()));
        query.addQueryItem(key, value);
    }

    QUrl u(url);
    u.setQuery(query);

    QNetworkRequest request(u);
    request.setHeader(QNetworkRequest::UserAgentHeader, this->userAgent());
    const QString bearerFormat = QStringLiteral("Bearer %1");
    const QString bearer = bearerFormat.arg(this->token());
    request.setRawHeader("Authorization", bearer.toUtf8());
    return request;
}

void GoogleOAuth2::setAccessTokenUrl(const QUrl &accessTokenUrl)
{
    if (d->accessTokenUrl != accessTokenUrl) {
        d->accessTokenUrl = accessTokenUrl;
        emit accessTokenUrlChanged(accessTokenUrl);
    }
}

void GoogleOAuth2::setRefreshToken(const QString refreshToken)
{
    if (d->refreshToken != refreshToken) {
        d->refreshToken = refreshToken;
        emit refreshTokenChanged(refreshToken);
    }
}

void GoogleOAuth2::setExpirationAt(const QDateTime &expirationAt)
{
    if (d->expirationAt != expirationAt) {
        d->expirationAt = expirationAt;
        emit expirationAtChanged(expirationAt);
    }
}

void GoogleOAuth2::grant()
{
    if (this->authorizationUrl().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "missing authorizationUrl";
        return;
    }
    if (d->accessTokenUrl.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "missing accessTokenUrl";
        return;
    }

    bool canStartFromRefresh = false;
    if (!refreshToken().isEmpty()
            && expirationAt().isValid()
            && (status() != QAbstractOAuth::Status::Granted)) {
        canStartFromRefresh = true;
    }
    if (canStartFromRefresh) {
        this->refreshAccessToken();
    } else {
        this->resourceOwnerAuthorization(authorizationUrl());
    }
}

void GoogleOAuth2::refreshAccessToken()
{
    if (d->refreshToken.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "missing refreshToken";
        return;
    }
    if (this->status() == Status::RefreshingToken) {
        qWarning() << Q_FUNC_INFO << "in progress";
        return;
    }

    QVariantMap parameters;
    QNetworkRequest request(d->accessTokenUrl);
    QUrlQuery query;
    parameters.insert(Key::grantType, QStringLiteral("refresh_token"));
    parameters.insert(Key::refreshToken, d->refreshToken);
    parameters.insert(Key::clientIdentifier, QUrl::toPercentEncoding(this->clientIdentifier()));
    parameters.insert(Key::clientSharedSecret, QUrl::toPercentEncoding(this->clientIdentifierSharedKey()));
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/x-www-form-urlencoded"));
    const QString data = query.toString(QUrl::FullyEncoded);

    this->setToken(QString());
    this->setExpirationAt(QDateTime());

    d->currentReply = this->networkAccessManager()->post(request, data.toUtf8());
    this->setStatus(Status::RefreshingToken);
    QObject::connect(d->currentReply.data(), &QNetworkReply::finished,
            std::bind(&QAbstractOAuthReplyHandler::networkReplyFinished,
                      this->replyHandler(), d->currentReply.data()));
    QObject::connect(this->replyHandler(), &QAbstractOAuthReplyHandler::tokensReceived,
                     d, &GoogleOAuth2::Private::accessTokenRequestFinished, Qt::UniqueConnection);
    QObject::connect(d->currentReply.data(), &QNetworkReply::finished,
            d->currentReply.data(), &QNetworkReply::deleteLater);
    QObject::connect(this->networkAccessManager(), &QNetworkAccessManager::authenticationRequired,
            d, &GoogleOAuth2::Private::authenticate, Qt::UniqueConnection);
}

QUrl GoogleOAuth2::buildAuthenticateUrl(const QVariantMap &parameters)
{
    if (this->state().isEmpty()) {
        QString randomState(QString::fromLatin1(this->generateRandomString(8)));
        this->setState(randomState);
    }
    const QString state = this->state();

    QVariantMap p(parameters);
    QUrl url(authorizationUrl());
    p.insert(Key::responseType, this->responseType());
    p.insert(Key::clientIdentifier, this->clientIdentifier());
    p.insert(Key::redirectUri, this->callback());
    p.insert(Key::scope, this->scope());
    p.insert(Key::state, state);
    //    if (modifyParametersFunction() != Q_NULLPTR) {
    //        modifyParametersFunction(Stage::RequestingAuthorization, &p);
    //    }
    QUrlQuery query;
    for (auto it = p.begin(), end = p.end(); it != end; ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    url.setQuery(query);
    connect(this->replyHandler(), &QAbstractOAuthReplyHandler::callbackReceived,
            this, &GoogleOAuth2::authorizationCallbackReceived, Qt::UniqueConnection);
    setStatus(QAbstractOAuth::Status::NotAuthenticated);
    return url;
}

void GoogleOAuth2::requestAccessToken(const QString &code)
{
    QVariantMap parameters;
    QNetworkRequest request(d->accessTokenUrl);
    QUrlQuery query;
    parameters.insert(Key::grantType, QStringLiteral("authorization_code"));
    parameters.insert(Key::code, QUrl::toPercentEncoding(code));
    parameters.insert(Key::redirectUri, QUrl::toPercentEncoding(this->callback()));
    parameters.insert(Key::clientIdentifier, QUrl::toPercentEncoding(this->clientIdentifier()));
    parameters.insert(Key::clientSharedSecret, QUrl::toPercentEncoding(this->clientIdentifierSharedKey()));
    //    if (modifyParametersFunction() != Q_NULLPTR) {
    //        modifyParametersFunction(Stage::RequestingAccessToken, &p);
    //    }
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/x-www-form-urlencoded"));

    const QString data = query.toString(QUrl::FullyEncoded);
    d->currentReply = this->networkAccessManager()->post(request, data.toUtf8());
    QObject::connect(d->currentReply.data(), &QNetworkReply::finished,
                     std::bind(&QAbstractOAuthReplyHandler::networkReplyFinished,
                               this->replyHandler(), d->currentReply.data()));
    QObject::connect(this->replyHandler(), &QAbstractOAuthReplyHandler::tokensReceived,
                     d, &GoogleOAuth2::Private::accessTokenRequestFinished, Qt::UniqueConnection);
    QObject::connect(this->networkAccessManager(), &QNetworkAccessManager::authenticationRequired,
                     d, &GoogleOAuth2::Private::authenticate, Qt::UniqueConnection);
}

void GoogleOAuth2::resourceOwnerAuthorization(const QUrl &url,
                                              const QVariantMap &parameters)
{
    if (url != authorizationUrl()) {
        qWarning() << Q_FUNC_INFO << "invalid:" << url << authorizationUrl();
        return;
    }
    const QUrl u = buildAuthenticateUrl(parameters);
    connect(this, &GoogleOAuth2::authorizationCallbackReceived,
            d, &GoogleOAuth2::Private::handleCallback, Qt::UniqueConnection);
    emit authorizeWithBrowser(u);
}

#include "moc_googleoauth2.cpp"
