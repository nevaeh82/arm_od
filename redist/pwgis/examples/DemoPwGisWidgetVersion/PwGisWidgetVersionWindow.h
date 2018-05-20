#ifndef PWGISWIDGETVERSIONWINDOW_H
#define PWGISWIDGETVERSIONWINDOW_H

#include <QWidget>

namespace Ui {
class PwGisWidgetVersionWindow;
}

class PwGisWidgetVersionWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::PwGisWidgetVersionWindow *ui;

public:
	explicit PwGisWidgetVersionWindow( QWidget *parent = 0 );
	~PwGisWidgetVersionWindow();

public slots:
	void onBtnOpenMap();
	void onBtnGetVersion();
	void onBtnCloseMap();
};

#endif // PWGISWIDGETVERSIONWINDOW_H
