#include <QtCore/QCoreApplication>

#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include <QtCore/QJsonObject>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>
#include <GoogleApps/GoogleOAuth2>
#include <GoogleApps/GoogleDrive>

#include "jsonfile.h"

enum class ExecMode : int {
    None,
    Upload,
    Download,
    Delete,
    Copy
};

bool upload(GoogleOAuth2 *oauth2, const QString &filename)
{
    GoogleDrive api(oauth2);
    bool ok = false;
    QJsonObject obj = GoogleDrive::waitReplyJson(api.filesCreateMedia(QJsonObject() ,filename), &ok);
    if (ok == true) {
        //success
        QString id = obj.value("id").toString();
        qInfo() << "id:" << id;
    }
    return ok;
}

bool download(GoogleOAuth2 *oauth2, const QString &id)
{
    GoogleDrive api(oauth2);
    QByteArray buffer = GoogleDrive::waitReplyRAW(api.filesGetMedia(id));
    if (buffer.size() > 0) {
        std::cout.write(buffer.data(), buffer.size());
        return true;
    }
    return false;
}

bool deleteResource(GoogleOAuth2 *oauth2, const QString &id)
{
    GoogleDrive api(oauth2);
    QJsonObject obj = GoogleDrive::waitReplyJson(api.filesDelete(id));
    //filesDelete returns NULL if success
    if (obj.isEmpty()) {
        return true;
    }
    return false;
}

bool copy(GoogleOAuth2 *oauth2, const QString &id)
{
    GoogleDrive api(oauth2);
    bool ok = false;
    QJsonObject resource;
    resource.insert(QStringLiteral("name"), QString("Copy of %1").arg(id));
    QJsonObject obj = GoogleDrive::waitReplyJson(api.filesCopy(id, resource), &ok);
    if (ok == true) {
        //success
        qInfo() << obj;
        QString id = obj.value("id").toString();
        qInfo() << "id:" << id;
    }
    return ok;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("googledrivectrl");
    app.setApplicationVersion("0.1");

    //commandline
    QCommandLineParser parser;
    parser.setApplicationDescription("GoogleDrive upload/download/delete tool");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption uploadOption(QStringList() << "u" << "upload", "upload <file>", "file");
    QCommandLineOption downloadOption(QStringList() << "d" << "download", "download <id>", "id");
    QCommandLineOption deleteOption(QStringList() << "delete", "delete <id>", "id");
    QCommandLineOption copyOption(QStringList() << "copy", "copy <id>", "id");
    parser.addOption(uploadOption);
    parser.addOption(downloadOption);
    parser.addOption(deleteOption);
    parser.addOption(copyOption);
    parser.process(app);

    ExecMode execMode = ExecMode::None;
    QString target;
    if (parser.isSet(uploadOption)) {
        execMode = ExecMode::Upload;
        target = parser.value(uploadOption);
    } else if (parser.isSet(downloadOption)) {
        execMode = ExecMode::Download;
        target = parser.value(downloadOption);
    } else if (parser.isSet(deleteOption)) {
        execMode = ExecMode::Delete;
        target = parser.value(deleteOption);
    } else if (parser.isSet(copyOption)) {
        execMode = ExecMode::Copy;
        target = parser.value(copyOption);
    } else {
        parser.showHelp(1);
    }

    qInfo() << "target:" << target;

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
    oauth2->setScope(GoogleDrive::scope());
    if (!settings.isEmpty()) {
        oauth2->setTokenJson(settings);
    }
    oauth2->grant();
    bool ok = oauth2->waitAuthorization();
    if (ok == true) {
        //write tokens
        settingsFile.write(oauth2->tokenJson());

        switch(execMode) {
        case ExecMode::Upload:
            ok = upload(oauth2, target);
            break;
        case ExecMode::Download:
            ok = download(oauth2, target);
            break;
        case ExecMode::Delete:
            ok = deleteResource(oauth2, target);
            break;
        case ExecMode::Copy:
            ok = copy(oauth2, target);
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
