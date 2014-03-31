#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

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

signals:
	void showBlaClicked();
	void showBplaClicked();
	void showNiippClicked();
};

#endif // CONTROLPANEL_H
