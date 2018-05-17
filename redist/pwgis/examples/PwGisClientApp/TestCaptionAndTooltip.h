#ifndef TESTCAPTIONANDTOOLTIP_H
#define TESTCAPTIONANDTOOLTIP_H

#include <QWidget>

namespace Ui {
class TestCaptionAndTooltip;
}

class TestCaptionAndTooltip : public QWidget
{
	Q_OBJECT

private:
	Ui::TestCaptionAndTooltip *ui;

	QString getRuText( const QString &text );

public:
	explicit TestCaptionAndTooltip(QWidget *parent = 0);
	~TestCaptionAndTooltip();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnAddMarkers();
	void onBtnMoveMarkers();
	void onBtnAddPath();
};

#endif // TESTCAPTIONANDTOOLTIP_H
