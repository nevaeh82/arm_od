#ifndef DEMOATLASTEST_H
#define DEMOATLASTEST_H

#include <QWidget>
#include <PwGis/pwgiswidget.h>

namespace Ui {
class DemoAtlasTest;
}

namespace Pw { namespace Gis {
	class IMapManager;
}}

class DemoAtlasTest : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoAtlasTest *ui;
	int _mapReadyCounter;
	int _atlasReadyCounter;

	Pw::Gis::IMapManager* _mapManager;
	PwGisWidget *_externalWindget;

	void updateSignalView();
	void isOpenWidget();

public:
	explicit DemoAtlasTest(QWidget *parent = 0);
	~DemoAtlasTest();

public slots:
	void onOpenMap();
	void onCloseMap();
	void onOpenAtlas();
	void onCloseAtlas();

	void onMapReady();
	void onAtlasReady();

	void openMapFrom();
	void openAtlasFrom();

};

#endif // DEMOATLASTEST_H
