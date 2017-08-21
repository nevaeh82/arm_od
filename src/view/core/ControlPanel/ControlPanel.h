#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QDateTime>
#include <QDockWidget>
#include <QTimer>

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
	Q_OBJECT

private:
	Ui::ControlPanel *ui;

	QPixmap redLbl;
	QPixmap greenLbl;
	QPixmap yellowLbl;

    QTimer* m_alarmTimer;

public:
	explicit ControlPanel(QWidget *parent = 0);
	~ControlPanel();

	QDateTime getStartDateTime()const;
	QDateTime getEndDateTime()const;

	void setStartDateTime(const QDateTime& date);
	void setEndDateTime(const QDateTime& date);
	void setCurrentDateTime(const QDateTime &value);
	void setPlayingEnabled(bool enabled);
	void setPause();

	void setDataBaseStatus(bool isOpened);

	QDockWidget* getDockWgt();

	void onSetDbLog(const QString& logTxt);

	void setARMRConnection(bool b);

	void setBdWriteState(int val);

signals:
	void startPlayingHistorySignal();
	void stopPlayingHistorySignal();
	void startExportToXls();

	void signalOnMux(int);

private slots:
    void onSetAlarm(QString, bool val);
    void closeAlarm();
};

#endif // CONTROLPANEL_H
