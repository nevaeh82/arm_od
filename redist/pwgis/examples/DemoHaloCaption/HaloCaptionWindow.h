#ifndef HALOCAPTIONWINDOW_H
#define HALOCAPTIONWINDOW_H

#include <QWidget>

namespace Ui {
class HaloCaptionWindow;
}

class HaloCaptionWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::HaloCaptionWindow *ui;

	void addMarker();
	void addPoint();
	void addLine();
	void addRectangle();
	void addCircle();
	void addPath();
	void addPolygon();
	void addClassicPolygon();
	void addSlice();

public:
	explicit HaloCaptionWindow( QWidget *parent = 0 );
	~HaloCaptionWindow();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnAddFeatures();
};

#endif // HALOCAPTIONWINDOW_H
