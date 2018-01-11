#ifndef MAPTABWIDGET_H
#define MAPTABWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QDockWidget>

#include "Map/MapWidget.h"
#include "NIIPP/NiippWidget.h"
#include "ControlPanel/ControlPanel.h"
#include "TCP/TcpClientManager.h"

namespace Ui {
class MapTabWidget;
}

class MapTabWidget: public QWidget
{
	Q_OBJECT
public:
	MapTabWidget(QWidget* parent = NULL);
	~MapTabWidget();

	MapWidget* getMapWidget() const;
	NiippWidget* getNiippWidget(const int number) const;

	QTreeView* getBlaTreeView() const;
	QTreeView* getBplaTreeView() const;
	ControlPanel* getControlPanelWidget() const;

	QDockWidget* getBlaDockWidget() const;
	QDockWidget* getBplaDockWidget() const;
	QDockWidget* getNiipp1DockWidget() const;
	QDockWidget* getNiipp2DockWidget() const;

    void setTcpManager(TcpClientManager* manager) {m_tcpClient = manager;}
    TcpClientManager* getTcpClientManager() {return m_tcpClient;}

private:
	Ui::MapTabWidget* ui;
    TcpClientManager* m_tcpClient;
};

#endif // MAPTABWIDGET_H
