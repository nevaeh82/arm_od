#ifndef POSTTRACKINGWINDOW_H
#define POSTTRACKINGWINDOW_H

#include <QWidget>
#include <QDialog>
#include <PwGis/pwgiswidget.h>
#include <PwGis/TacticMap/styles/PostStyle.h>

class QTimer;
class Post;
class ITacticMap;
class IIdGenerator;
class IMapProvider;

namespace Ui {
class PostTrackingWindow;
}

class PostTrackingWindow : public QWidget
{
	Q_OBJECT

private:
	PwGisLonLat* createNextPoint(QObject* obj);

public:
	explicit PostTrackingWindow(QWidget *parent = 0);
	~PostTrackingWindow();

public slots:
	void onUpdateObjects();
	void onMapReady();

	void onBtnOpenMap();
	void onBtnStartTracking();
	void onBtnStopTracking();

	void onChangeStyle();

	void onSetPost();
	void onRotatePost(int angle);

private:
	Ui::PostTrackingWindow *ui;

private:
	ITacticMap*   _tacticMap;
	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;
	PwGisLonLat* _center;
	double _angle;
	double _mapHeight;

	//style
	PostStyle* _style;

	//demo
	QTimer* _timer ;
	Post* _moveablePost;

	//course ratio params
	int _course_c1;
	int _course_c2;
	int _course_c3;
};

#endif // POSTTRACKINGWINDOW_H
