#ifndef REPERWINDOW_H
#define REPERWINDOW_H


#include <QWidget>

namespace Ui {
class ReperWindow;
}

class ReperWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::ReperWindow *ui;

public:
	explicit ReperWindow( QWidget *parent = 0 );
	~ReperWindow();

public slots:
	void onBtnOpenMapDialog();
	void onBtnAddOsmReper();
	void onBtnAddSatReper();
	void onBtnCloseMap();
};

#endif // REPERWINDOW_H
