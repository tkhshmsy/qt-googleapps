#ifndef GOOGLECALENDAR_H
#define GOOGLECALENDAR_H

#include "googleapps_global.h"
#include "abstractgoogleapps.h"

#include <QtCore/QVariantMap>
class QString;
class QJsonObject;
class QNetworkReply;
class GoogleOAuth2;

class GOOGLEAPPSSHARED_EXPORT GoogleCalendar : public AbstractGoogleApps
{
    Q_OBJECT
public:
    explicit GoogleCalendar(GoogleOAuth2 *googleOAuth2, QObject *parent = 0);
    ~GoogleCalendar();

    static const QString scope();

    //Acl
    QNetworkReply *AclDelete(const QString &calendarId, const QString &ruleId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclGet(const QString &calendarId, const QString &ruleId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclInsert(const QString &calendarId, const QJsonObject &aclResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclList(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclPatch(const QString &calendarId, const QString &ruleId, const QJsonObject &aclResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclUpdate(const QString &calendarId, const QString &ruleId, const QJsonObject &aclResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *AclWatch(const QString &calendarId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //calendarList
    QNetworkReply *CalendarListDelete(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListGet(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListInsert(const QJsonObject &calendarListResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListList(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListPatch(const QString &calendarId, const QJsonObject &calendarListResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListUpdate(const QString &calendarId, const QJsonObject &calendarListResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarListWatch(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //calendars
    QNetworkReply *CalendarsClear(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarsDelete(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarsGet(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarsInsert(const QJsonObject &calendarsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarsPatch(const QString &calendarId, const QJsonObject &calendarsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *CalendarsUpdate(const QString &calendarId, const QJsonObject &calendarsResource, const QVariantMap &parameters = QVariantMap());
    //Channels
    QNetworkReply *ChannelsStop(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Colors
    QNetworkReply *ColorsGet(const QVariantMap &parameters = QVariantMap());
    //Events
    QNetworkReply *EventsDelete(const QString &calendarId, const QString &eventId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsGet(const QString &calendarId, const QString &eventId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsImport(const QString &calendarId, const QJsonObject &eventsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsInsert(const QString &calendarId, const QJsonObject &eventsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsInstances(const QString &calendarId, const QString &eventId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsList(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsMove(const QString &calendarId, const QString &eventId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsPatch(const QString &calendarId, const QString &eventId, const QJsonObject &eventsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsQuickAdd(const QString &calendarId, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsUpdate(const QString &calendarId, const QString &eventId, const QJsonObject &eventsResource, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *EventsWatch(const QString &calendarId, const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Freebusy
    QNetworkReply *FreebusyQuery(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
    //Settings
    QNetworkReply *SettingsGet(const QString &setting, const QVariantMap &parameters = QVariantMap());
    QNetworkReply *SettingsList(const QVariantMap &parameters = QVariantMap());
    QNetworkReply *SettingsWatch(const QJsonObject &requestBody, const QVariantMap &parameters = QVariantMap());
};

#endif // GOOGLECALENDAR_H
