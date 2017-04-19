#ifndef BASEPARSERWIDGET_H
#define BASEPARSERWIDGET_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class BaseParserWidget;
}

struct MobileSession
{
	QString standart;
	QString datetime;
	QString network;
	QString basestation;
	QString lac;
	QString cid;
	QString frequency;
	QString duration;

	double lon;
	double lat;
	QString source;

	MobileSession::MobileSession()
	{
		standart = QString("");
		datetime = QString("");
		network = QString("");
		basestation = QString("");
		lac = QString("");
		cid = QString("");
		frequency = QString("");
		duration = QString("");

		lon = 0;
		lat = 0;
		source = QString("");
	}
};


class BaseParserWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BaseParserWidget(QWidget *parent = 0);
	~BaseParserWidget();

	void setTableData(const QList<MobileSession> &data);

private:
	Ui::BaseParserWidget *ui;

private slots:
	void slotOpenXmpPath();

signals:
	void signalPathSelected(QString);
};

#endif // BASEPARSERWIDGET_H
