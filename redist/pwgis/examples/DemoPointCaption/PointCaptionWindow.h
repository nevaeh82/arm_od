#ifndef POINTCAPTIONWINDOW_H
#define POINTCAPTIONWINDOW_H

#include <QWidget>

namespace Ui {
class PointCaptionWindow;
}

class PointCaptionWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::PointCaptionWindow *ui;

public:
	explicit PointCaptionWindow(QWidget *parent = 0);
	~PointCaptionWindow();

public slots:
	void onBtnOpenMap();
	void onBtnAddPoints();
	void onBtnCloseMap();
};

#endif // POINTCAPTIONWINDOW_H
