#ifndef ARM_OD_VIEW_H
#define ARM_OD_VIEW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QGridLayout>
#include <QPointF>
#include <QMenu>
#include <QAction>
#include <QTabWidget>
#include <QStackedWidget>

#include "Tabs/TabManager.h"

typedef QVector<QPointF>         rpc_send_points_vector;
typedef QPointF                  rpc_QPointF;
typedef QVector<DataFly>         rpc_data_fly;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* ui;

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

	QTabWidget* getMainTabWidget() const;
	QStackedWidget* getStackedWidget() const;

	QMenu* getViewMenu() const;

public slots:
	void mapOpened();
	void cancelMapOpen();

private:
	void init();
	void setStateMapAction( bool value );

signals:
	void openAtlasSignal();
	void openMapSignal();
	void setupKoordinatometriyaSignal();

private slots:
	void openAtlasAction();
	void openMapAction();


};
Q_DECLARE_METATYPE(rpc_send_points_vector)
Q_DECLARE_METATYPE(rpc_QPointF)
Q_DECLARE_METATYPE(rpc_data_fly)


#endif // ARM_OD_VIEW_H
