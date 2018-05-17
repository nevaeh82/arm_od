#ifndef MAPDIALOGWINDOW_H
#define MAPDIALOGWINDOW_H

#include <QWidget>

namespace Ui {
class MapDialogWindow;
}

class MapDialogWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::MapDialogWindow *ui;

public:
	explicit MapDialogWindow(QWidget *parent = 0);
	~MapDialogWindow();

public slots:
	void onBtnOpenMapDialog();
	void onBtnCloseMap();
};

#endif // MAPDIALOGWINDOW_H
