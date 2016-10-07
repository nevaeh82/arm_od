#pragma once

#include <QDateTime>
#include <QTimer>

#include "PwGis/pwgiswidget.h"
#include "Map/Features/FeatureAbstract.h"

namespace MapFeature {

class FeaturesFactory;

class PointKK : public QObject
{

    Q_OBJECT

public:
    PointKK(PwGisWidget *wgt, const QPointF& position, const QDateTime& createTime, QObject* parent);
    virtual ~PointKK();

private:
    QTimer m_timer;
    QDateTime m_createTime;

    PwGisWidget* m_widget;
    QString m_kkId;
    QPointF m_pos;

    float m_bright;

private slots:
    void onKKpointAction();
};

} // namespace MapFeature

