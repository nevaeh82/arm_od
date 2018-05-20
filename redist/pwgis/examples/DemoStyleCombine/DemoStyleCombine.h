#ifndef TESTPROFILESDEMO_H
#define TESTPROFILESDEMO_H

#include <QWidget>

namespace Ui {
class DemoStyleCombine;
}
namespace Pw { namespace Gis {
	class IMapManager;
	class IProfileManager;
}}

class DemoStyleCombine : public QWidget
{
	Q_OBJECT

private:
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IProfileManager * _profileManager;

	Ui::DemoStyleCombine *ui;

public:
	explicit DemoStyleCombine(QWidget *parent = 0);
	~DemoStyleCombine();

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
