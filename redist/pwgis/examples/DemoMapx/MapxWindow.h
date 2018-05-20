#ifndef MAPXWINDOW_H
#define MAPXWINDOW_H

#include <QWidget>

namespace Ui {
class MapxWindow;
}

class MapxWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::MapxWindow *ui;

public:
	explicit MapxWindow(QWidget *parent = 0);
	~MapxWindow();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
};

#endif // MAPXWINDOW_H
