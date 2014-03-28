#include "Solver.h"
#include "ui_Solver.h"

Solver::Solver(int id, ITabManager *tab_manager) :
    ui(new Ui::Solver)
{
    ui->setupUi(this);

    _id = id;
    _tab_manager = tab_manager;

    setWindowIconText(tr("Обнаружитель"));

    QVBoxLayout* v1 = new QVBoxLayout();

    _chb_auto = new QCheckBox();
    _chb_auto->setCheckState(Qt::Unchecked);
    _chb_auto->setText(tr("Автоматический режим"));
    v1->addWidget(_chb_auto);
    QLabel* lbl1 = new QLabel(tr("Высота"));
    _dsb_alt = new QDoubleSpinBox();
    _dsb_alt->setValue(0);
    _dsb_alt->setRange(0, 10000);

//    _sb_alt = new QSpinBox();
//    _sb_alt->setValue(66);

    v1->addWidget(lbl1, Qt::AlignTop);
    v1->addWidget(_dsb_alt, Qt::AlignTop);

    _sb_track_length = new QSpinBox();
    _sb_track_length->setRange(0, 1000);
    _sb_track_length->setValue(0);

//    connect(_sb_track_length, SIGNAL(valueChanged(int)), this, SLOT(_slot_count_track(int)));
    QLabel* lbl33 = new QLabel(tr("Длина трека"));
    v1->addWidget(lbl33);
    v1->addWidget(_sb_track_length);

    _sb_count_point_for_state = new QSpinBox();
    _sb_count_point_for_state->setRange(0, 1000);
    _sb_count_point_for_state->setValue(0);

//    connect

    QLabel* lbl2 = new QLabel(tr("Число отсчетов"));
    v1->addWidget(lbl2);
    v1->addWidget(_sb_count_point_for_state);

    _pb_clear = new QPushButton(tr("Очистить"));
    connect(_pb_clear, SIGNAL(clicked()), this, SLOT(_slot_clear()));
    v1->addWidget(_pb_clear);

    QHBoxLayout* h1 = new QHBoxLayout();
    _accept = new QPushButton(tr("Принять"));

    _cancel = new QPushButton(tr("Отклонить"));

    h1->addWidget(_accept);
    h1->addWidget(_cancel);

    connect(_accept, SIGNAL(clicked()), this, SLOT(_slot_accept()));
    connect(_cancel, SIGNAL(clicked()), this, SLOT(_slot_cancel()));

    connect(_chb_auto, SIGNAL(toggled(bool)), this, SLOT(_slot_auto(bool)));

    QVBoxLayout* v_f = new QVBoxLayout();
    v_f->addLayout(v1, Qt::AlignTop);
    v_f->addLayout(h1, Qt::AlignBottom);

    this->setLayout(v_f);
}

Solver::~Solver()
{
    delete ui;
}

void Solver::_slot_accept()
{
    QByteArray ba;// = new QByteArray();
    QDataStream ds(&ba, QIODevice::ReadWrite);

    ds << _id;
//    ds << _chb_auto->isChecked();
    ds << _sb_track_length->value();
    ds << _dsb_alt->value();

    CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER, ba);
    _tab_manager->send_data(0, msg);
//    this->close();
}

void Solver::_slot_cancel()
{
    this->hide();
}

void Solver::_slot_auto(bool state)
{
    QByteArray ba;// = new QByteArray();
    QDataStream ds(&ba, QIODevice::ReadWrite);

    ds << _chb_auto->isChecked();

    CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_AUTO, ba);
    _tab_manager->send_data(0, msg);

}

void Solver::_slot_clear()
{
    QByteArray ba;// = new QByteArray();
    QDataStream ds(&ba, QIODevice::ReadWrite);

    bool t =true;
    ds << _id;
    ds << t;

    CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_CLEAR, ba);
    _tab_manager->send_data(0, msg);
}

void Solver::_slot_count_track(int count)
{
//    QByteArray ba;// = new QByteArray();
//    QDataStream ds(&ba, QIODevice::ReadWrite);

//    ds << count;

//    CommandMessage* msg = new CommandMessage(COMMAND_SET_SOLVER_TRACK_SIZE, ba);
//    _tab_manager->send_data(0, msg);
}

void Solver::slot_show()
{
    this->show();
}
