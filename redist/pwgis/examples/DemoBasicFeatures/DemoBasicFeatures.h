#ifndef DemoBasicFeatures_H
#define DemoBasicFeatures_H

#include <QWidget>

namespace Ui {
class DemoBasicFeatures;
}

class DemoBasicFeatures : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoBasicFeatures *ui;

	void addMessageToLog( const QString &message, const QString &featureID = "" );
	QString getRuText( const QString &text );

public:
	explicit DemoBasicFeatures(QWidget *parent = 0);
	~DemoBasicFeatures();

public slots:
	void onBtnAddMarker();
	void onBtnAddPoint();
	void onBtnAddLine();
	void onBtnAddRectangle();
	void onBtnAddCircle();
	void onBtnAddPath();
	void onBtnAddPolygon();
	void onBtnAddClassicPolygon();
	void onBtnAddSlice();
	void onBtnGetGeometry();
	void onBtnDeleteFeature();
	void onBtnDeleteAllFeatures();

	void onOpenMap();
	void onCloseMap();

	void onAddRing();
	void onAddRingSegment();

	void onMapReady();
};

#endif // DemoBasicFeatures_H
