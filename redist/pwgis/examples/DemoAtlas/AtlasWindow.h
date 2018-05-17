#ifndef ATLASWINDOW_H
#define ATLASWINDOW_H

#include <QWidget>


namespace Ui {
class AtlasWindow;
}

class AtlasWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::AtlasWindow *ui;

public:
	explicit AtlasWindow( QWidget *parent = 0 );
	~AtlasWindow();

public slots:
	void onBtnOpenAtlas();
	void onBtnCloseAtlas();
	void onBtnClearLog();
	void onMapReady();
	void onAtlasReady();
};

#endif // ATLASWINDOW_H
