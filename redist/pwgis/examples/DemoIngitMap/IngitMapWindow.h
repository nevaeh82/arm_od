#ifndef INGITMAPWINDOW_H
#define INGITMAPWINDOW_H

#include <QWidget>

namespace Ui {
class IngitMapWindow;
}

class IngitMapWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::IngitMapWindow *ui;
	int _trackCounter;
	void loadReadme();

public:
	explicit IngitMapWindow( QWidget *parent = 0 );
	~IngitMapWindow();

public slots:
	void onBtnOpenMapDialog();
	void onBtnAddPoints();
	void onBtnAddTrack();
	void onBtnCloseMap();
};

#endif // INGITMAPWINDOW_H
