#ifndef TESTPROFILESDEMO_H
#define TESTPROFILESDEMO_H

#include <QDialog>

namespace Ui {
class DemoProfiles;
}
namespace Pw { namespace Gis {
	class IMapManager;
	class IProfileManager;
}}

class DemoProfiles : public QDialog
{
	Q_OBJECT

private:
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IProfileManager * _profileManager;

	Ui::DemoProfiles *ui;

public:
	explicit DemoProfiles(QWidget *parent = 0);
	~DemoProfiles();

public slots:
	void onCloseMap();
	void onOpenMap();
	void onOpenAtlas();
	void onCloseAtlas();
	void onMapReady();


	void onRegisterProfile();
	void onUnregisterProfile();


};

#endif // TESTPROFILESDEMO_H
