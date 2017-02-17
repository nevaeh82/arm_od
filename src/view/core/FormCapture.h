#ifndef FORMCAPTURE_H
#define FORMCAPTURE_H

#include <QWidget>

namespace Ui {
class FormCapture;
}

class FormCapture : public QWidget
{
	Q_OBJECT

public:
	explicit FormCapture(QWidget *parent = 0);
	~FormCapture();

signals:
	void signalClear();
	void signalApply(int);

private slots:
	void slotApply();

private:
	Ui::FormCapture *ui;
};

#endif // FORMCAPTURE_H
