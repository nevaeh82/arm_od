#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <PwGis/pwgiswidget.h>

namespace Ui {
	class MainWindow;
}

namespace Pw { namespace Gis {
	class IMapManager;
	class IMapBounds;
}}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Q_ENUMS(PwGisWidget::MeasureState)

private:
	template<typename T>
	void showTest();

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();



private slots:
	void loadGpxTrack();
	void loadRzdTrack();

	void loadMap();
	void loadNoMap();

	void onSelectFeature( const QString& id );
	void onClickFeature( const QString& id );
	void onAddFeature(const QString &type, const QString &id);
	void onUnselectFeature( const QString& id );
	void onMapReady();

	void onSetMarker();
	void onRemoveMarker();

	void onMapClick(double lon, double lat);
	void onMapMouseMove(double lon, double lat);
	void tooltipsAndCaptions();


	void drawCircle();
	void centerLat89();
	void centerLonM179();
	void addSlice();
	void removeSlice();
	void add2Markers();
	void remove2Markers();
	void addMarker();
	void updateMarker();
	void add4Objects();
	void move4Objects();
	void drawLongLine();
	void drawLongRectangle();
	void drawBrokenCircle();
	void addForkedObjects();
	void removeForkedObjects();


	void testMapFormats();
	void testCaptionAndTooltip();
	void demoUpdateFeatureParameters();

	void setCustomStyle();

	void on_pushButton_clicked();

	void onMeasure(const QVariantList &, double measure, int status);

	void on_removeGradFeature_clicked();

	void on_addGradFeature_clicked();

	void onGetFeatureProperties();

	void on_testEditFeatureButton_clicked();

	void on_testEditFeatureButton_2_clicked();

	void on_testEditFeatureButton_3_clicked();

	void on_testFormats_clicked();

	void on_btnCustomStyleMarker_clicked();

	void on_btnRemoveAllObjects_clicked();

private slots:
	void log(QString message);

private:
	Ui::MainWindow *ui;
	QString _mapFile;

	QString getFileNameJs();
	QString getFileNameRzdJs();
	QString readJs(const QString& fileNameJs);
	QString m_lastAddedFeature;
	PwGisPointList* m_lastPoints;
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IMapBounds* _mapBounds;

};

#endif // MAINWINDOW_H
