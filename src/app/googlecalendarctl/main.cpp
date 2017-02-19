#include <QtCore/QCoreApplication>

#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>
#include <GoogleApps/GoogleOAuth2>
#include <GoogleApps/GoogleCalendar>

#include "jsonfile.h"

enum class ExecMode : int {
    None,
    CalendarList,
    EventsList,
    EventsQuickAdd,
    EventsInsert,
    EventsDelete,
};

bool calendarList(GoogleOAuth2 *oauth2)
{
    GoogleCalendar api(oauth2);
    bool ok = false;
    QJsonObject obj = GoogleCalendar::waitReplyJson(api.CalendarListList(), &ok);
    if (ok) {
        if (!obj.contains(QStringLiteral("items"))) {
            qWarning() << "ERROR: no calendar items";
            return false;
        }
        QJsonArray items = obj.value(QStringLiteral("items")).toArray();
        foreach (const QJsonValue &item, items) {
            if (!item.isObject()) {
                continue;
            }
            QJsonObject resource = item.toObject();
            QString id = resource.value(QStringLiteral("id")).toString();
            QString summary = resource.value(QStringLiteral("summary")).toString();
            bool isDeleted = resource.value(QStringLiteral("deleted")).toBool();
            if (id.isEmpty() || summary.isEmpty() || isDeleted) {
                continue;
            }
            qInfo() << summary << "-> id:" << id;
        }
        return true;
    }
    qInfo() << obj;
    return false;
}

bool eventsList(GoogleOAuth2 *oauth2, const QString &id, const QDateTime &dt)
{
    GoogleCalendar api(oauth2);
    QString dateFormat("yyyy-MM-dd");
    QString dateTimeFormat("yyyy-MM-ddThh:mm:ss+09:00"); //JST+9 "Asia/Tokyo"

    QDateTime minDT(QDateTime(dt.date()));
    QDateTime maxDT(minDT.addDays(3));

    qInfo() << "Show events"
            << "from:" << minDT.toString(dateTimeFormat)
            << "to:" << maxDT.toString(dateTimeFormat);

    QVariantMap parameters;
    parameters.insert(QStringLiteral("timeMin"), minDT.toString(dateTimeFormat));
    parameters.insert(QStringLiteral("timeMax"), maxDT.toString(dateTimeFormat));

    bool ok = false;
    QJsonObject obj = GoogleCalendar::waitReplyJson(api.EventsList(id, parameters), &ok);
    if (ok) {
        if (!obj.contains(QStringLiteral("items"))) {
            qWarning() << "ERROR: no event items";
            return false;
        }
        QJsonArray items = obj.value(QStringLiteral("items")).toArray();
        if (items.size() <= 1) {
            qInfo() << "no events";
            return true;
        }
        foreach (const QJsonValue &item, items) {
            if (!item.isObject()) {
                continue;
            }
            QJsonObject resource = item.toObject();
            QString summary = resource.value(QStringLiteral("summary")).toString();
            QString id = resource.value(QStringLiteral("id")).toString();
            QJsonObject start = resource.value(QStringLiteral("start")).toObject();
            QDate startD = QDate::fromString(start.value(QStringLiteral("date")).toString(), dateFormat);
            QDateTime startDT = QDateTime::fromString(start.value(QStringLiteral("dateTime")).toString(), dateTimeFormat);
            QJsonObject end = resource.value(QStringLiteral("end")).toObject();
            QDate endD = QDate::fromString(end.value(QStringLiteral("date")).toString(), dateFormat);
            QDateTime endDT = QDateTime::fromString(end.value(QStringLiteral("dateTime")).toString(), dateTimeFormat);

            if (summary.isEmpty()) {
                continue;
            }
            if (!startDT.isValid()) {
                qInfo() << summary
                        << "start:" << startD.toString(dateFormat)
                        << "end:" << endD.toString(dateFormat)
                        << "id:" << id;
            } else {
                qInfo() << summary
                        << "start:" << startDT.toString(dateTimeFormat)
                        << "end:" << endDT.toString(dateTimeFormat)
                        << "id:" << id;
            }
        }
        return true;
    }
    qInfo() << obj;
    return false;
}

bool eventsQuickAdd(GoogleOAuth2 *oauth2, const QString &id, const QString &text)
{
    GoogleCalendar api(oauth2);

    qInfo() << "QuickAdd event (into current datetime)"
            << "text:" << text;

    QVariantMap parameters;
    parameters.insert(QStringLiteral("text"), text);

    bool ok = false;
    QJsonObject obj = GoogleCalendar::waitReplyJson(api.EventsQuickAdd(id, parameters), &ok);
    if (ok) {
        if (!obj.contains(QStringLiteral("id"))) {
            qWarning() << "ERROR: no event";
            return false;
        }
        qInfo() << "id:" << obj.value(QStringLiteral("id")).toString();
        return true;
    }
    qInfo() << obj;
    return false;
}

bool eventsInsert(GoogleOAuth2 *oauth2, const QString &id, const QDateTime &dt, const QString &text)
{
    GoogleCalendar api(oauth2);
    QString dateTimeFormat("yyyy-MM-ddThh:mm:ss+09:00"); //JST+9 "Asia/Tokyo"

    qInfo() << "Add event"
            << "datetime:" << dt
            << "text:" << text;

    QDateTime startDT(dt);
    QDateTime endDT(startDT.addSecs(3600));
    QJsonObject eventsResource;
    eventsResource.insert(QStringLiteral("summary"), text);
    QJsonObject startObject;
    startObject.insert(QStringLiteral("dateTime"), startDT.toString(dateTimeFormat));
    eventsResource.insert(QStringLiteral("start"), startObject);
    QJsonObject endObject;
    endObject.insert(QStringLiteral("dateTime"), endDT.toString(dateTimeFormat));
    eventsResource.insert(QStringLiteral("end"), endObject);

    bool ok = false;
    QJsonObject obj = GoogleCalendar::waitReplyJson(api.EventsInsert(id, eventsResource), &ok);
    if (ok) {
        if (!obj.contains(QStringLiteral("id"))) {
            qWarning() << "ERROR: no event";
            return false;
        }
        qInfo() << "id:" << obj.value(QStringLiteral("id")).toString();
        return true;
    }
    qInfo() << obj;
    return false;
}

bool eventsDelete(GoogleOAuth2 *oauth2, const QString &id, const QString &eid)
{
    GoogleCalendar api(oauth2);
    qInfo() << "Delete event"
            << "event-id:" << eid;

    bool ok = false;
    QJsonObject obj = GoogleCalendar::waitReplyJson(api.EventsDelete(id, eid), &ok);
    //eventsDelete returns NULL if success
    if (obj.isEmpty()) {
        return true;
    }
    qInfo() << obj;
    return false;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("googlecalendarctrl");
    app.setApplicationVersion("0.1");

    //commandline
    QCommandLineParser parser;
    parser.setApplicationDescription("GoogleCalendar tool");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("command", "command <list|add|quick|show|delete>");
    parser.addPositionalArgument("calendar-id", "target <calender-id>");
    QCommandLineOption dateOption(QStringList() << "datetime", "at <yyyy-MM-ddThh:mm:ss>", "yyyy-MM-ddThh:mm:ss");
    QCommandLineOption textOption(QStringList() << "text", "event's title <text>", "text");
    QCommandLineOption eventIdOption(QStringList() << "eid", "event-id <id>", "id");
    parser.addOption(dateOption);
    parser.addOption(textOption);
    parser.addOption(eventIdOption);
    parser.process(app);

    ExecMode execMode = ExecMode::None;
    const QStringList args = parser.positionalArguments();
    QString calendarId;
    QDateTime dt(QDateTime::currentDateTime());
    QString text;
    QString eventId;
    if (args.size() > 0) {
        QString command = args.value(0);
        if (command.compare(QStringLiteral("list")) == 0) {
            execMode = ExecMode::CalendarList;
        } else if (command.compare(QStringLiteral("add")) == 0) {
            execMode = ExecMode::EventsInsert;
        } else if (command.compare(QStringLiteral("quick")) == 0) {
            execMode = ExecMode::EventsQuickAdd;
        } else if (command.compare(QStringLiteral("show")) == 0) {
            execMode = ExecMode::EventsList;
        } else if (command.compare(QStringLiteral("delete")) == 0) {
            execMode = ExecMode::EventsDelete;
        }
    }
    if (args.size() >= 2) {
        calendarId = args.value(1);
    }
    if (parser.isSet(dateOption)) {
        QString string = parser.value(dateOption);
        dt = QDateTime::fromString(string, "yyyy-MM-ddThh:mm:ss");
        if (!dt.isValid()) {
            qWarning() << "invalid datetime:" << string;
        }
    }
    if (parser.isSet(textOption)) {
        text = parser.value(textOption);
    }
    if (parser.isSet(eventIdOption)) {
        eventId = parser.value(eventIdOption);
    }
    if (execMode == ExecMode::None) {
        parser.showHelp(1);
    }

    //read tokens
    QString settingsFilename = QString(".%1").arg(app.applicationName());
    JsonFile settingsFile(settingsFilename);
    settingsFile.setPath(JsonFile::Location::Home);
    QJsonObject settings = settingsFile.read();

    //authorization
    GoogleOAuth2 *oauth2 = new GoogleOAuth2();
    QOAuthHttpServerReplyHandler *replyHandler = new QOAuthHttpServerReplyHandler(50000);
    oauth2->setReplyHandler(replyHandler);
    QObject::connect(oauth2, &GoogleOAuth2::authorizeWithBrowser,
                     [&](const QUrl &url){
        qInfo() << "authorize URL:" << url.toString();
    });

    oauth2->setClientIdJson(":/client_id.json");
    oauth2->setScope(GoogleCalendar::scope());
    if (!settings.isEmpty()) {
        oauth2->setTokenJson(settings);
    }
    oauth2->grant();
    bool ok = oauth2->waitAuthorization();
    if (ok == true) {
        //write tokens
        settingsFile.write(oauth2->tokenJson());

        switch(execMode) {
        case ExecMode::CalendarList:
            ok = calendarList(oauth2);
            break;
        case ExecMode::EventsList:
            if (calendarId.isEmpty()) {
                qWarning() << "ERROR: missing calendar id";
                break;
            }
            ok = eventsList(oauth2, calendarId, dt);
            break;
        case ExecMode::EventsQuickAdd:
            if (calendarId.isEmpty()) {
                qWarning() << "ERROR: missing calendar id";
                break;
            }
            if (text.isEmpty()) {
                qWarning() << "ERROR: missing text";
                break;
            }
            ok = eventsQuickAdd(oauth2, calendarId, text);
            break;
        case ExecMode::EventsInsert:
            if (calendarId.isEmpty()) {
                qWarning() << "ERROR: missing calendar id";
                break;
            }
            if (text.isEmpty()) {
                qWarning() << "ERROR: missing text";
                break;
            }
            ok = eventsInsert(oauth2, calendarId, dt, text);
            break;
        case ExecMode::EventsDelete:
            if (calendarId.isEmpty()) {
                qWarning() << "ERROR: missing calendar id";
                break;
            }
            if (eventId.isEmpty()) {
                qWarning() << "ERROR: missing event id";
                break;
            }
            ok = eventsDelete(oauth2, calendarId, eventId);
            break;
        default:
            ok = false;
        }
    }

    if (ok == false) {
        qInfo() << "FAILED";
        return 1;
    }
    qInfo() << "SUCCESS";
    return 0;
}
