#ifndef LINESTYLEGROUPBOX_H
#define LINESTYLEGROUPBOX_H

#include <QGroupBox>

class TracedObjectAbstract;
class IMapProvider;

namespace Ui {
class LineStyleGroupBox;
}

class LineStyleGroupBox : public QGroupBox
{
	Q_OBJECT

public:
	explicit LineStyleGroupBox(QWidget *parent = 0);
	~LineStyleGroupBox();

	void setItem(TracedObjectAbstract* item);
	void setProvider(IMapProvider* provider);

private:
	Ui::LineStyleGroupBox *ui;

	IMapProvider* _mapProvider;
	TracedObjectAbstract* _currentItem;

	void setButtonColor(QColor color);
	void selectDashStyle(int style);

public slots:
	void onColorBtnClicked();
	void onDashStyleChanged(int index);
	void onLineSizeChanged(QString size);
};

#endif // LINESTYLEGROUPBOX_H
