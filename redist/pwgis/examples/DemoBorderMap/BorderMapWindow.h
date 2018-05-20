#ifndef BORDERMAPWINDOW_H
#define BORDERMAPWINDOW_H

#include <QWidget>

namespace Ui {
class BorderMapWindow;
}

class BorderMapWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::BorderMapWindow *ui;

public:
	explicit BorderMapWindow( QWidget *parent = 0 );
	~BorderMapWindow();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
};

#endif // BORDERMAPWINDOW_H
