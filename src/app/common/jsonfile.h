#ifndef JSONFILE_H
#define JSONFILE_H

class QString;
class QJsonObject;

class JsonFile
{
public:
    JsonFile(const QString &filename);
    ~JsonFile();

    enum class Location : int {
        Current,
        Home,
//        UserLocalShare,
//        VarLib,
    };

    QString path() const;
    QString filename() const;
    QString fullPath() const;

    bool setPath(const JsonFile::Location &location);
    bool setPath(const QString &path);
    bool setFilename(const QString &filename);

    bool write(const QJsonObject &json);
    QJsonObject read();

private:
    class Private;
    Private *d;
};

#endif // JSONFILE_H
