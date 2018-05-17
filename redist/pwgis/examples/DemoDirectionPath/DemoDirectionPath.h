#ifndef DEMODIRECTIONPATH_H
#define DEMODIRECTIONPATH_H

#include <QWidget>
#include <PwGis/PwGisPointList.h>
#include <PwGis/objects/PwGisStyle.h>

namespace Ui {
class DemoDirectionPath;
}

class DemoDirectionPath : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoDirectionPath *ui;

	PwGisPointList _trackPoints;
	PwGisStyle* _pathStyle;
	PwGisStyle* _windRoseStyle;
	QString _styleArrow;
	QString _currentStyle;

	double getRandomLon();
	double getRandomLat();

public:
	explicit DemoDirectionPath(QWidget *parent = 0);
	~DemoDirectionPath();

private slots:
	void onBtnOpenMap();
	void onMapReady();
	void onBtnCloseMap();

	void onBtnAddWindRose();
	void onBtnRemoveWindRose();

	void onBtnAddPath();
	void onBtnAppendPath();
	void onBtnRemovePath();
	void onBtnArrowStyle();
	void onBtnSimpleStyle();
};

#endif // DEMODIRECTIONPATH_H
