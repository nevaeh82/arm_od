#ifndef ICONSGENERATOR_H
#define ICONSGENERATOR_H

#include <QPushButton>
#include <QLabel>
#include <QPixmap>

class IconsGenerator
{
public:
    IconsGenerator();
    ~IconsGenerator();

public:
    QPushButton *set_centred();
    QPushButton *set_home();
    QPushButton *set_fix();
    QPushButton *set_spiral();
    QPushButton *set_tablet();
    QPushButton *set_web_mail();

private:
    QPushButton *_set_pb(QPixmap, QString);

};

#endif // ICONSGENERATOR_H
