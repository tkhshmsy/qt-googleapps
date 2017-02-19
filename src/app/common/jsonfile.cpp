#include "jsonfile.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QByteArray>

class JsonFile::Private
{
public:
    Private();

    QString fullPath() const;

    QString filename;
    QString path;
};

JsonFile::Private::Private()
    : filename()
    , path()
{}

QString JsonFile::Private::fullPath() const
{
    QString ret = QString("%1/%2").arg(path).arg(filename);
    return ret;
}

JsonFile::JsonFile(const QString &filename)
    : d(new Private)
{
    setFilename(filename);
}

JsonFile::~JsonFile()
{}

QString JsonFile::path() const
{
    return d->path;
}
QString JsonFile::filename() const
{
    return d->filename;
}

QString JsonFile::fullPath() const
{
    return d->fullPath();
}

bool JsonFile::setPath(const JsonFile::Location &location)
{
    switch(location) {
    case JsonFile::Location::Current:
        d->path = QDir::currentPath();
        break;
    case JsonFile::Location::Home:
        d->path = QDir::homePath();
        break;
    default:
        qWarning() << Q_FUNC_INFO << "invaid location";
        return false;
    }
    return true;
}

bool JsonFile::setPath(const QString &path)
{
    QFileInfo info(path);
    d->path = info.dir().absolutePath();
    return true;
}

bool JsonFile::setFilename(const QString &filename)
{
    if (filename.isNull()
            || filename.isEmpty()) {
        return false;
    }
    d->filename = filename;
    return true;
}

bool JsonFile::write(const QJsonObject &json)
{
    QFile file(d->fullPath());
    if (!file.open(QFile::Text|QFile::Truncate|QFile::WriteOnly)) {
        qWarning() << "failed to open:" << file.fileName();
        return false;
    }
    QJsonDocument doc(json);
    file.write(QByteArray(doc.toJson(QJsonDocument::JsonFormat::Indented)));
    file.close();
    return true;
}

QJsonObject JsonFile::read()
{
    QFile file(d->fullPath());
    if (!file.exists()) {
        qWarning() << "not found:" << file.fileName();
        return QJsonObject();
    }
    if (!file.open(QFile::Text|QFile::ReadOnly)) {
        qWarning() << "failed to open:" << file.fileName();
        return QJsonObject();
    }
    QJsonObject ret = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    return ret;
}
