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
class ARM_OD_view;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
	Ui::ARM_OD_view*     ui;
    
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	QTabWidget* getMainTabWidget() const;
	QStackedWidget* getStackedWidget() const;
    
private:
	void init();

signals:
	void openAtlasSignal();
	void openMapSignal();
	void setupKoordinatometriyaSignal();

};
Q_DECLARE_METATYPE(rpc_send_points_vector)
Q_DECLARE_METATYPE(rpc_QPointF)
Q_DECLARE_METATYPE(rpc_data_fly)


#endif // ARM_OD_VIEW_H
