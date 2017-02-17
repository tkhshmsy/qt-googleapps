#ifndef GOOGLEOAUTH2_H
#define GOOGLEOAUTH2_H

#include "googleapps_global.h"
#include <QtNetworkAuth/QAbstractOAuth2>
#include <QtCore/QJsonObject>
#include <QtCore/QVariantMap>

class QUrl;
class QString;
class QDateTime;
class QNetworkAccessManager;
class QNetworkRequest;

class GOOGLEAPPSSHARED_EXPORT GoogleOAuth2 : public QAbstractOAuth2
{
    Q_OBJECT
    Q_PROPERTY(QUrl accessTokenUrl READ accessTokenUrl WRITE setAccessTokenUrl NOTIFY accessTokenUrlChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
public:
    explicit GoogleOAuth2(QObject *parent = 0);
    ~GoogleOAuth2();

    QString responseType() const override;
    QUrl accessTokenUrl() const;
    QString refreshToken() const;
    QDateTime expirationAt() const;

    //helper methods
    bool isAuthorized() const;
    bool setClientIdJson(const QJsonObject &json);
    bool setClientIdJson(const QString &filename);
    bool setTokenJson(const QJsonObject &json);
    bool setTokenJson(const QString &filename);
    QJsonObject tokenJson();
    bool waitAuthorization();
    bool checkAndUpdateTokens();
    QNetworkRequest createRequest(const QUrl &url, const QVariantMap &parameters = QVariantMap());

public Q_SLOTS:
    void setAccessTokenUrl(const QUrl &accessTokenUrl);
    void setRefreshToken(const QString refreshToken);
    void setExpirationAt(const QDateTime &expirationAt);

    //start authorization
    void grant() override;
    //start refresh
    void refreshAccessToken();

protected:
    QUrl buildAuthenticateUrl(const QVariantMap &parameters = QVariantMap());
    void requestAccessToken(const QString &code);
    void resourceOwnerAuthorization(const QUrl &url,
                                    const QVariantMap &parameters = QVariantMap()) override;

Q_SIGNALS:
    void accessTokenUrlChanged(const QUrl &accessTokenUrl);
    void refreshTokenChanged(const QString &refreshToken);
    void expirationAtChanged(const QDateTime &expirationAt);

private:
    Q_DISABLE_COPY(GoogleOAuth2)
    class Private;
    Private *d;
};

#endif // GOOGLEOAUTH2_H
