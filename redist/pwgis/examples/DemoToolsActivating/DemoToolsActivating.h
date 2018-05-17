#ifndef TESTPROFILESDEMO_H
#define TESTPROFILESDEMO_H

#include <QDialog>
#include <PwGis/pwgiswidget.h>

namespace Ui {
class DemoToolsActivating;
}
namespace Pw { namespace Gis {
	class IMapManager;
	class IProfileManager;
}}

class DemoToolsActivating : public QDialog
{
	Q_OBJECT

private:
	Pw::Gis::IMapManager* _mapManager;
	Pw::Gis::IProfileManager * _profileManager;

	Ui::DemoToolsActivating *ui;

public:
	explicit DemoToolsActivating(QWidget *parent = 0);
	~DemoToolsActivating();

public slots:
	void onCloseMap();
	void onOpenMap();
	void onOpenAtlas();
	void onCloseAtlas();
	void onMapReady();


	void onRegisterProfile();
	void onUnregisterProfile();

	void on_actiavateTool_clicked();
	void on_actiavateTool_2_clicked();
	void on_actiavateTool_6_clicked();
	void on_actiavateTool_5_clicked();
	void on_actiavateTool_4_clicked();
	void on_actiavateTool_3_clicked();
	void on_actiavateTool_7_clicked();
	void on_actiavateTool_8_clicked();
	void on_checkTool_toggled(bool checked);
	void on_checkTool_2_toggled(bool checked);
	void on_checkTool_3_toggled(bool checked);
	void on_checkTool_4_toggled(bool checked);
	void on_checkTool_5_toggled(bool checked);
	void on_checkTool_6_toggled(bool checked);
	void on_checkTool_7_toggled(bool checked);
private slots:
	void on_checkTool_9_toggled(bool checked);
};

#endif // TESTPROFILESDEMO_H
