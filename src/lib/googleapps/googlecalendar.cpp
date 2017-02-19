#include "googlecalendar.h"

#include <QtCore/QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "googleoauth2.h"

const QString ContentTypeHeaderJSON = QStringLiteral("application/json;charset=UTF-8");

namespace API {
const QString scope              = QStringLiteral("https://www.googleapis.com/auth/calendar");
const QString baseUrl            = QStringLiteral("https://www.googleapis.com/calendar/v3");

const QString AclDelete          = baseUrl + QStringLiteral("/calendars/%1/acl/%2");
const QString AclGet             = baseUrl + QStringLiteral("/calendars/%1/acl/%2");
const QString AclInsert          = baseUrl + QStringLiteral("/calendars/%1/acl");
const QString AclList            = baseUrl + QStringLiteral("/calendars/%1/acl");
const QString AclPatch           = baseUrl + QStringLiteral("/calendars/%1/acl/%2");
const QString AclUpdate          = baseUrl + QStringLiteral("/calendars/%1/acl/%2");
const QString AclWatch           = baseUrl + QStringLiteral("/calendars/%1/acl/watch");

const QString CalendarListDelete = baseUrl + QStringLiteral("/calendarList/%1");
const QString CalendarListGet    = baseUrl + QStringLiteral("/users/me/calendarList/%1");
const QString CalendarListInsert = baseUrl + QStringLiteral("/users/me/calendarList");
const QString CalendarListList   = baseUrl + QStringLiteral("/users/me/calendarList");
const QString CalendarListPatch  = baseUrl + QStringLiteral("/users/me/calendarList/%1");
const QString CalendarListUpdate = baseUrl + QStringLiteral("/users/me/calendarList/%1");
const QString CalendarListWatch  = baseUrl + QStringLiteral("/users/me/calendarList/watch");

const QString CalendarsClear     = baseUrl + QStringLiteral("/calendars/%1/clear");
const QString CalendarsDelete    = baseUrl + QStringLiteral("/calendars/%1");
const QString CalendarsGet       = baseUrl + QStringLiteral("/calendars/%1");
const QString CalendarsInsert    = baseUrl + QStringLiteral("/calendars");
const QString CalendarsPatch     = baseUrl + QStringLiteral("/calendars/%1");
const QString CalendarsUpdate    = baseUrl + QStringLiteral("/calendars/%1");

const QString ChannelsStop       = baseUrl + QStringLiteral("/channels/stop");

const QString ColorsGet          = baseUrl + QStringLiteral("/colors");

const QString EventsDelete       = baseUrl + QStringLiteral("/calendars/%1/events/%2");
const QString EventsGet          = baseUrl + QStringLiteral("/calendars/%1/events/%2");
const QString EventsImport       = baseUrl + QStringLiteral("/calendars/%1/events/import");
const QString EventsInsert       = baseUrl + QStringLiteral("/calendars/%1/events");
const QString EventsInstances    = baseUrl + QStringLiteral("/calendars/%1/events/%2/instances");
const QString EventsList         = baseUrl + QStringLiteral("/calendars/%1/events");
const QString EventsMove         = baseUrl + QStringLiteral("/calendars/%1/events/%2/move");
const QString EventsPatch        = baseUrl + QStringLiteral("/calendars/%1/events/%2");
const QString EventsQuickAdd     = baseUrl + QStringLiteral("/calendars/%1/events/quickAdd");
const QString EventsUpdate       = baseUrl + QStringLiteral("/calendars/%1/events/%2");
const QString EventsWatch        = baseUrl + QStringLiteral("/calendars/%1/events/watch");

const QString FreebusyQuery      = baseUrl + QStringLiteral("/freebusy");

const QString SettingsGet        = baseUrl + QStringLiteral("/users/me/settings/%1");
const QString SettingsList       = baseUrl + QStringLiteral("/users/me/settings");
const QString SettingsWatch      = baseUrl + QStringLiteral("/users/me/settings/watch");
}

GoogleCalendar::GoogleCalendar(GoogleOAuth2 *googleOAuth2, QObject *parent)
    : AbstractGoogleApps(googleOAuth2, parent)
{}

GoogleCalendar::~GoogleCalendar()
{}

const QString GoogleCalendar::scope()
{
    return API::scope;
}

QNetworkReply *GoogleCalendar::AclDelete(const QString &calendarId, const QString &ruleId, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclDelete).arg(calendarId).arg(ruleId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleCalendar::AclGet(const QString &calendarId, const QString &ruleId, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclGet).arg(calendarId).arg(ruleId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::AclInsert(const QString &calendarId, const QJsonObject &aclResource, const QVariantMap &parameters)
{
//    if (!aclResource.contains(QStringLiteral("role"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: role";
//    }
//    if (!aclResource.contains(QStringLiteral("scope"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: scope";
//    }
//    if (!aclResource.contains(QStringLiteral("scope.type"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: scope.type";
//    }

    QUrl url(QString(API::AclInsert).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(aclResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::AclList(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclList).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::AclPatch(const QString &calendarId, const QString &ruleId, const QJsonObject &aclResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclPatch).arg(calendarId).arg(ruleId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(aclResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::AclUpdate(const QString &calendarId, const QString &ruleId, const QJsonObject &aclResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclUpdate).arg(calendarId).arg(ruleId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(aclResource).toJson());
    QNetworkReply *reply = this->put(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::AclWatch(const QString &calendarId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QUrl url(QString(API::AclWatch).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListDelete(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarListDelete).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListGet(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarListGet).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListInsert(const QJsonObject &calendarListResource, const QVariantMap &parameters)
{
//    if (!calendarListResource.contains(QStringLiteral("defaultReminders[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: defaultReminders[].method";
//    }
//    if (!calendarListResource.contains(QStringLiteral("defaultReminders[].minutes"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: defaultReminders[].minutes";
//    }
//    if (!calendarListResource.contains(QStringLiteral("notificationSettings.notifications[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: notificationSettings.notifications[].method";
//    }
//    if (!calendarListResource.contains(QStringLiteral("notificationSettings.notifications[].type"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: notificationSettings.notifications[].type";
//    }

    QNetworkRequest request = this->createRequest(QUrl(API::CalendarListInsert), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarListResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListList(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::CalendarListList), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListPatch(const QString &calendarId, const QJsonObject &calendarListResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarListPatch).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarListResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListUpdate(const QString &calendarId, const QJsonObject &calendarListResource, const QVariantMap &parameters)
{
//    if (!calendarListResource.contains(QStringLiteral("defaultReminders[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: defaultReminders[].method";
//    }
//    if (!calendarListResource.contains(QStringLiteral("defaultReminders[].minutes"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: defaultReminders[].minutes";
//    }
//    if (!calendarListResource.contains(QStringLiteral("notificationSettings.notifications[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: notificationSettings.notifications[].method";
//    }
//    if (!calendarListResource.contains(QStringLiteral("notificationSettings.notifications[].type"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: notificationSettings.notifications[].type";
//    }

    QUrl url(QString(API::CalendarListUpdate).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarListResource).toJson());
    QNetworkReply *reply = this->put(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarListWatch(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::CalendarListWatch), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsClear(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarsClear).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QNetworkReply *reply = this->post(request, QByteArray());
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsDelete(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarsDelete).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsGet(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarsGet).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsInsert(const QJsonObject &calendarsResource, const QVariantMap &parameters)
{
    if (!calendarsResource.contains(QStringLiteral("summary"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: summary";
    }
    QNetworkRequest request = this->createRequest(QUrl(API::CalendarsInsert), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarsResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsPatch(const QString &calendarId, const QJsonObject &calendarsResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarsPatch).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarsResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::CalendarsUpdate(const QString &calendarId, const QJsonObject &calendarsResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::CalendarsUpdate).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(calendarsResource).toJson());
    QNetworkReply *reply = this->put(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::ChannelsStop(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::ChannelsStop), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::ColorsGet(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::ColorsGet), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsDelete(const QString &calendarId, const QString &eventId, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsDelete).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->deleteResource(request);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsGet(const QString &calendarId, const QString &eventId, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsGet).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsImport(const QString &calendarId, const QJsonObject &eventsResource, const QVariantMap &parameters)
{
//    if (!eventsResource.contains(QStringLiteral("attachments[].fileUrl"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attachments[].fileUrl";
//    }
//    if (!eventsResource.contains(QStringLiteral("attendees[].email"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attendees[].email";
//    }
//    if (!eventsResource.contains(QStringLiteral("end"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: end";
//    }
//    if (!eventsResource.contains(QStringLiteral("iCalUID"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: iCalUID";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].method";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].minutes"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].minutes";
//    }
//    if (!eventsResource.contains(QStringLiteral("start"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: start";
//    }

    QUrl url(QString(API::EventsImport).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(eventsResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsInsert(const QString &calendarId, const QJsonObject &eventsResource, const QVariantMap &parameters)
{
//    if (!eventsResource.contains(QStringLiteral("attachments[].fileUrl"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attachments[].fileUrl";
//    }
//    if (!eventsResource.contains(QStringLiteral("attendees[].email"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attendees[].email";
//    }
    if (!eventsResource.contains(QStringLiteral("end"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: end";
    }
//    if (!eventsResource.contains(QStringLiteral("iCalUID"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: iCalUID";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].method";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].minutes"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].minutes";
//    }
    if (!eventsResource.contains(QStringLiteral("start"))) {
        qWarning() << Q_FUNC_INFO << "missing required property: start";
    }

    QUrl url(QString(API::EventsInsert).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(eventsResource).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsInstances(const QString &calendarId, const QString &eventId, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsInstances).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsList(const QString &calendarId, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsList).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsMove(const QString &calendarId, const QString &eventId, const QVariantMap &parameters)
{
    if (!parameters.contains(QStringLiteral("destination"))) {
        qWarning() << Q_FUNC_INFO << "missing required parameter: destination";
    }

    QUrl url(QString(API::EventsMove).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QNetworkReply *reply = this->post(request, QByteArray());
    return reply;
}

QNetworkReply *GoogleCalendar::EventsPatch(const QString &calendarId, const QString &eventId, const QJsonObject &eventsResource, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsPatch).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(eventsResource).toJson());
    QNetworkReply *reply = this->patch(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsQuickAdd(const QString &calendarId, const QVariantMap &parameters)
{
    if (!parameters.contains(QStringLiteral("text"))) {
        qWarning() << Q_FUNC_INFO << "missing required parameter: text";
    }

    QUrl url(QString(API::EventsQuickAdd).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QNetworkReply *reply = this->post(request, QByteArray());
    return reply;
}

QNetworkReply *GoogleCalendar::EventsUpdate(const QString &calendarId, const QString &eventId, const QJsonObject &eventsResource, const QVariantMap &parameters)
{
//    if (!eventsResource.contains(QStringLiteral("attachments[].fileUrl"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attachments[].fileUrl";
//    }
//    if (!eventsResource.contains(QStringLiteral("attendees[].email"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: attendees[].email";
//    }
//    if (!eventsResource.contains(QStringLiteral("end"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: end";
//    }
//    if (!eventsResource.contains(QStringLiteral("iCalUID"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: iCalUID";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].method"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].method";
//    }
//    if (!eventsResource.contains(QStringLiteral("reminders.overrides[].minutes"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: reminders.overrides[].minutes";
//    }
//    if (!eventsResource.contains(QStringLiteral("start"))) {
//        qWarning() << Q_FUNC_INFO << "missing required property: start";
//    }

    QUrl url(QString(API::EventsUpdate).arg(calendarId).arg(eventId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(eventsResource).toJson());
    QNetworkReply *reply = this->put(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::EventsWatch(const QString &calendarId, const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QUrl url(QString(API::EventsWatch).arg(calendarId));
    QNetworkRequest request = this->createRequest(url, parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::FreebusyQuery(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::FreebusyQuery), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}

QNetworkReply *GoogleCalendar::SettingsGet(const QString &setting, const QVariantMap &parameters)
{
    QUrl url(QString(API::SettingsGet).arg(setting));
    QNetworkRequest request = this->createRequest(url, parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::SettingsList(const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::SettingsList), parameters);
    QNetworkReply *reply = this->get(request);
    return reply;
}

QNetworkReply *GoogleCalendar::SettingsWatch(const QJsonObject &requestBody, const QVariantMap &parameters)
{
    QNetworkRequest request = this->createRequest(QUrl(API::SettingsWatch), parameters);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, ContentTypeHeaderJSON);

    QByteArray buffer(QJsonDocument(requestBody).toJson());
    QNetworkReply *reply = this->post(request, buffer);
    return reply;
}
