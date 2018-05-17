#ifndef GOOGLECACHEMAPWINDOW_H
#define GOOGLECACHEMAPWINDOW_H

#include <QWidget>

namespace Ui {
class GoogleCacheMapWindow;
}

class GoogleCacheMapWindow : public QWidget
{
	Q_OBJECT

private:
	Ui::GoogleCacheMapWindow *ui;

	QString getRuText( const QString &text );

public:
	explicit GoogleCacheMapWindow(QWidget *parent = 0);
	~GoogleCacheMapWindow();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnAddTrack();
};

#endif // GOOGLECACHEMAPWINDOW_H
