#ifndef DEMOPOLYGONSIZE_H
#define DEMOPOLYGONSIZE_H

#include <QMainWindow>
#include <QFileDialog>

#include <PwGis/pwgiswidget.h>


namespace Ui {
class DemoPolygonSize;
}

class DemoPolygonSize : public QMainWindow
{
	Q_OBJECT

public:
	explicit DemoPolygonSize(QWidget *parent = 0);
	~DemoPolygonSize();

private:
	Ui::DemoPolygonSize *ui;
	PwGisWidget *pwgiswidget;

private slots:
	void onOpenMap();
	void onCloseMap();

	void onAddPolygon(bool b);
	void onAddCircle(bool b);
	void onChangeSize(int i);
};

#endif // DEMOPOLYGONSIZE_H
