#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
	Q_OBJECT

private:
	Ui::ControlPanel *ui;

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

signals:
	void startPlayingHistorySignal();
	void stopPlayingHistorySignal();
};

#endif // CONTROLPANEL_H
