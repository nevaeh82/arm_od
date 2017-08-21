#ifndef MAPINFOWIDGET_H
#define MAPINFOWIDGET_H

#include <QWidget>

namespace Ui {
class MapInfoWidget;
}

class MapInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapInfoWidget(QWidget *parent = 0);
    ~MapInfoWidget();

    void setText(const QString& text);

private:
    Ui::MapInfoWidget *ui;
};

#endif // MAPINFOWIDGET_H
