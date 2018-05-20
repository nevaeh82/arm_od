//#ifndef TESTPROFILESDEMO_H
//#define TESTPROFILESDEMO_H

#ifndef DEMOATLASPROFILES_H
#define DEMOATLASPROFILES_H


#include <QDialog>

namespace Ui {
class DemoAtlasProfiles;
}
namespace Pw { namespace Gis {
	class IMapManager;
	class IProfileManager;
}}

class DemoAtlasProfiles : public QDialog
{
	Q_OBJECT

private:
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IProfileManager * _profileManager;

	Ui::DemoAtlasProfiles *ui;

public:
	explicit DemoAtlasProfiles(QWidget *parent = 0);
	~DemoAtlasProfiles();

public slots:
	void onOpenAtlas();
	void onCloseAtlas();
	void onMapReady();


	void onRegisterProfile();
	void onUnregisterProfile();


};

#endif // DEMOATLASPROFILES_H
