#ifndef NSTATION_H
#define NSTATION_H

#include <QPointF>
#include <QString>

class NStation
{
public:
    NStation(int id = 0);
    int getID() const {return m_id;}
    int getMode() const {return m_mode;}
    int getAzimuth() const {return m_azimuth;}
    int getRadius() const {return m_radius;}
    int getWidth() const {return m_width;}
    int getIsCircle() const {return m_isCircle;}
    int getIsEnable() const {return m_isEnable;}
    QPointF getPos() const {return m_pos;}
    QString getName() const {return m_name;}

    void setID(int id) {m_id = id;}
    void setMode(int mode) {m_mode = mode;}
    void setAzimuth(int azimuth) {m_azimuth = azimuth;}
    void setRadius(int radius) {m_radius = radius;}
    void setWidth(int width) {m_width = width;}
    void setIsCircle(bool isCircle) {m_isCircle = isCircle;}
    void setIsEnable(bool isEnable) {m_isEnable = isEnable;}
    void setName(const QString name) {m_name = name;}

    QPointF setPos(QPointF pos) {return m_pos = pos;}

private:
    int m_id;
    int m_mode;
    int m_azimuth;
    int m_radius;
    int m_width;
    bool m_isCircle;
    bool m_isEnable;
    QPointF m_pos;
    QString m_name;
};

#endif // NSTATION_H
