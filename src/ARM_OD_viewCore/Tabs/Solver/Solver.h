#ifndef SOLVER_H
#define SOLVER_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QByteArray>
#include <QDataStream>
#include <QCheckBox>

#include "../ITabManager.h"
#include "../../Common/CommandMessage.h"

namespace Ui {
class Solver;
}

class Solver : public QWidget
{
    Q_OBJECT
    
public:
    explicit Solver(int id, ITabManager* tab_manager);
    ~Solver();
    
private:
    Ui::Solver *ui;

    ITabManager*        _tab_manager;
    int                 _id;

    QDoubleSpinBox*     _dsb_alt;
    QSpinBox*           _sb_track_length;
    QSpinBox*           _sb_count_point_for_state;
    QPushButton*        _accept;
    QPushButton*        _cancel;
    QPushButton*        _pb_clear;
    QCheckBox*          _chb_auto;

private slots:
    void _slot_accept();
    void _slot_cancel();
    void _slot_auto(bool state);

    void _slot_clear();
    void _slot_count_track(int count);

public slots:
    void slot_show();
};

#endif // SOLVER_H
