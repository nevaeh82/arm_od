#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QWidget>
#include <QApplication>
#include <QTranslator>
#include <QLocale>


namespace Ui {
class MapWindow;
}

class MapWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::MapWindow *ui;
	int _trackCounter;

	void retranslateStrings();
	QTranslator* _translator;

public:
	explicit MapWindow( QWidget *parent = 0 );
	~MapWindow();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnOpenAtlas();
	void onBtnCloseAtlas();
	void onBtnAddTrack();
	void onBtnClearLog();
	void onMapReady();
	void onAtlasReady();

private slots:
	void clickedRadioButton();
	void baseLayerSwitcherChanged( int state );
};

#endif // MAPWINDOW_H
