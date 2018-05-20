#include <QDir>
#include "TestFormats.h"
#include "ui_TestFormats.h"


TestFormats::TestFormats(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TestFormats)
{
	ui->setupUi(this);

	connect(ui->btnLoadGpxTrack, SIGNAL(clicked()), this, SLOT(loadGpxTrack()));
	connect(ui->btnLoadRzdTrack, SIGNAL(clicked()), this, SLOT(loadRzdTrack()));

	connect(ui->btnInvalidAbakan, SIGNAL(clicked()), this, SLOT(loadInvalidAbakanMap()));
	connect(ui->btnLoadNoMap, SIGNAL(clicked()), this, SLOT(loadNoMap()));
	//connect(ui->btnLoadIngitMap, SIGNAL(clicked()), this, SLOT(loadIngitMap()));
	connect(ui->btnLoadIngitWorldMap, SIGNAL(clicked()), this, SLOT(loadIngitWorldMap()));
	connect(ui->btnLoadIngitRussiaMap, SIGNAL(clicked()), this, SLOT(loadIngitRussiaMap()));

	connect(ui->btnLoadMapinfoSaintPetersburgMap, SIGNAL(clicked()), this, SLOT(loadMapinfoSaintPetersburgMap()));
	connect(ui->btnLoadMapinfoMoscowMap, SIGNAL(clicked()), this, SLOT(loadMapinfoMoscowMap()));
	connect(ui->btnLoadMapinfoNizhnyNovgorodMap, SIGNAL(clicked()), this, SLOT(loadMapinfoNizhnyNovgorodMap()));
	connect(ui->btnLoadMapinfoMoscowUtmWgs84Map, SIGNAL(clicked()), this, SLOT(loadMapinfoMoscowUtmWgs84Map()));
	connect(ui->btnLoadMapinfoWorldMap41001, SIGNAL(clicked()), this, SLOT(loadMapinfoWorldMap41001()));
	connect(ui->btnLoadMapinfoRussiaMap41001, SIGNAL(clicked()), this, SLOT(loadMapinfoRussiaMap41001()));
	connect(ui->btnLoadMapinfoWorldMapQuasi3857, SIGNAL(clicked()), this, SLOT(loadMapinfoWorldMapQuasi3857()));
	connect(ui->btnLoadMapinfoRussiaMapQuasi3857, SIGNAL(clicked()), this, SLOT(loadMapinfoRussiaMapQuasi3857()));
	connect(ui->btnLoadMapinfoTestM180P180, SIGNAL(clicked()), this, SLOT(loadMapinfoTestM180P180()));

	connect(ui->btnTestSxf, SIGNAL(clicked()), this, SLOT(testSxf()));
	connect(ui->btnTestSit, SIGNAL(clicked()), this, SLOT(testSit()));
	connect(ui->btnTestMapPanorama, SIGNAL(clicked()), this, SLOT(testMapPanorama()));
	connect(ui->btnKmlMap, SIGNAL(clicked()), this, SLOT(loadKml()));
}

TestFormats::~TestFormats()
{
	delete ui;
}

void TestFormats::loadIngitMap(){	            openMap("Ingit/abakan/abakan.chart");}
void TestFormats::loadIngitWorldMap(){	            openMap("Ingit/world/world.chart");}
void TestFormats::loadIngitRussiaMap(){	            openMap("Ingit/russia/russia.chart");}
void TestFormats::loadInvalidIngitMap(){	    openMap("Ingit/fake/fake.chart");}
void TestFormats::loadMapinfoSaintPetersburgMap(){	openMap("MapX/SaintPetersburgGK42/SaintPetersburgGK42.gst");}
void TestFormats::loadMapinfoMoscowMap(){	        openMap("MapX/MoscowBuildingsGK42/MoscowBuildingsGK42.gst");}
void TestFormats::loadMapinfoNizhnyNovgorodMap(){	openMap("MapX/NizhnyNovgorodGK42/NizhnyNovgorodGK42.gst");}
void TestFormats::loadMapinfoMoscowUtmWgs84Map(){	openMap("MapX/MoscowUtmWgs84/MoscowUtmWgs84.gst");}
void TestFormats::loadMapinfoWorldMap41001(){	    openMap("MapX/WorldMercator41001/WorldMercator41001.gst");}
void TestFormats::loadMapinfoRussiaMap41001(){	    openMap("MapX/RussiaMercator41001/RussiaMercator41001.gst");}
void TestFormats::loadMapinfoWorldMapQuasi3857(){	openMap("MapX/WorldQuasiGoogle3857/WorldQuasiGoogle3857.gst");}
void TestFormats::loadMapinfoRussiaMapQuasi3857(){	openMap("MapX/RussiaQuasiGoogle3857/RussiaQuasiGoogle3857.gst");}
void TestFormats::loadMapinfoTestM180P180(){	        openMap("MapX/TestRussia3857/TestRussia3857.gst");}
void TestFormats::loadKml(){	        openMap("Kml/spb.kml");}
void TestFormats::loadKmlWorldMap(){	openMap( "Kml/quasiWorld.kml" );}
void TestFormats::testSxf(){	openMap("Panorama/voronej/M3703.SXF");}
void TestFormats::testSit(){	openMap("Panorama/moscow/moskow.sit");}
void TestFormats::testMapPanorama(){	        openMap("Panorama/podolsk/Podolsk.map"); }
void TestFormats::loadInvalidAbakanMap(){ 	openMap("Ingit/fakeAbakan/ABAKAN.chart");}

void TestFormats::openMap( QString mapfile)
{
	ui->widget1->mapProvider()->mapManager()->openMap(
				QDir::toNativeSeparators(
					QApplication::applicationDirPath() + "../mapSamples/" + QDir::separator() +  mapfile));
}
