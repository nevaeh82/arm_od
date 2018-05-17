#ifndef MAPFORMATINFO_H
#define MAPFORMATINFO_H
#include <QObject>

#include "PwGisClient.h"

class GISCLIENT_EXPORT MapFormatInfo : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("ClassID", "{A04D5CFD-499F-4626-B64A-8D159F76A11A}")
    Q_CLASSINFO("InterfaceID", "{C37B4AF3-66FD-4B2C-9FFB-67FA8055BDDE}")
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)
    Q_PROPERTY(QString extensions READ getExtensions WRITE setExtensions)
public:
    MapFormatInfo(QObject* parent = 0) : QObject(parent) {}
    MapFormatInfo(QString description, QString extensions) : QObject(), _description(description), _extensions(extensions) {}
public slots:
    QString getDescription() const { return _description; }
    void setDescription(QString arg) { _description = arg; }
    QString getExtensions() const { return _extensions; }
    void setExtensions(QString arg) { _extensions = arg; }

private:
    QString _description;
    QString _extensions;
};

#endif // MAPFORMATINFO_H
