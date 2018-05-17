#ifndef SATELLITESPOTWINDOW_H
#define SATELLITESPOTWINDOW_H

#include <QWidget>
#include <QTimer>
#include "SatelliteMapWidget.h"
#include <PwGis/GeoMathematics/GeometricFunctions.h>


namespace Ui {
class SatelliteSpotWindow;
}


struct Satellite
{
	SatelliteMapHandle id;
	QString name;
	QList< SatellitePoint > points;
	int state;
};


class SatelliteSpotWindow : public QWidget
{
	Q_OBJECT

public:
	explicit SatelliteSpotWindow( QWidget *parent = 0 );
	~SatelliteSpotWindow();

public slots:
	void onMapReady();

private slots:
	void onUpdateSatellitePosition();
	void onBtnClearAll();
	void onBtnStartTimer();
	void onBtnStopTimer();
	void onBtnLon0Lat0();
	void onBtnLon0Lat35();
	void onBtnLon90Lat0();
	void onBtnLon0Lat40();
	void onBtnLon0LatM40();
	void onBtnLon170Lat0();
	void onBtnLon180Lat0();
	void onBtnLonM150Lat0();
	void onBtnLon180Lat6();
	void onBtnLonM180MLat6();
	void onBtnLon180Lat60();
	void onBtnLon170Lat60();
	void onBtnLonM180LatM60();
	void onBtnLonM170LatM60();
	void onBtnLon170Lat45();

	void onCheckBoxMoveSatellite( bool checked = false );
	void onSetSatelliteLongitude( int longitude );
	void onSetSatelliteLatitude( int latitude );
	void onSetSatelliteAltitude( int altitude );
	void onBtnSpotPoints();
	void onBtnSpotPolygon();

private:
	Ui::SatelliteSpotWindow *ui;

	QList< Satellite > _satelliteTrack;
	QTimer* _satelliteTimer;
	QList<SatelliteMapHandle> _selectedSatellites;
	QString _spotName;
	double _satelliteLongitude, _satelliteLatitude, _satelliteAltitude;

	bool loadSatelliteData( const QString& file, QString& name, QList<SatellitePoint>& path );

	/** Draw satellite spot
	 @param satelliteLongitude in degree
	 @param satelliteLatitude in degree
	 @param satelliteAltitude in kilometers
	*/
	void drawSpot( double satelliteLongitude, double satelliteLatitude, double satelliteAltitude );

	/** Move satellite spot
	*/
	void moveSpot();
};

#endif // SATELLITESPOTWINDOW_H
