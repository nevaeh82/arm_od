#include "SolverSetupWidget.h"
#include "ui_SolverSetupWidget.h"

SolverSetupWidget::SolverSetupWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SolverSetupWidget)
{
	ui->setupUi(this);

	// Hide "?" button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);

	setWindowFlags(flags);

	this->setWindowTitle(tr("Solver setup"));
	setWindowIcon(QIcon(":/images/icons/ARM_R.png"));

	this->hide();

	connect(ui->pbStop, SIGNAL(clicked(bool)), this, SIGNAL(signalStopSolver()));
	connect(ui->pbRestart, SIGNAL(clicked(bool)), this, SIGNAL(signalRestartSolver()));
	connect(ui->pbClear, SIGNAL(clicked(bool)), this, SIGNAL(signalClearSolver()));
	connect(ui->pbCorStart, SIGNAL(clicked(bool)), this, SIGNAL(signalStartSolverCorrections()));
	connect(ui->pbCorStop, SIGNAL(clicked(bool)), this, SIGNAL(signalStopSolverCorrections()));
	connect(ui->pbCorReset, SIGNAL(clicked(bool)), this, SIGNAL(signalResetSolverCorrections()));
	connect(ui->pbCorSave, SIGNAL(clicked(bool)), this, SIGNAL(signalSaveSolverCorrections()));

	connect(ui->pbSetUser, SIGNAL(clicked(bool)), this, SIGNAL(signalSetUser()));
	connect(ui->pbGetAll, SIGNAL(clicked(bool)), this, SIGNAL(signalGetAll()));

    connect(ui->pbSetAll, SIGNAL(clicked(bool)), this, SIGNAL(signalSetParams()));
}

SolverSetupWidget::~SolverSetupWidget()
{
	delete ui;
}

void SolverSetupWidget::setSolverVersion(const QString &version)
{
	ui->lblSolverVersion->setText( version );
}

void SolverSetupWidget::setSolverSettings(SolverProtocol::Packet_DataFromSolver_SolverResponse& data)
{
	if( data.has_solverversion() ) {
		ui->lblSolverVersion->setText( QString::fromStdString(data.solverversion()) );
	}

	if( data.has_clienttype() ) {
		ui->cbClientType->setCurrentIndex( ((int)data.clienttype())-1 );
	}

	if( data.has_solutionconfiguration() ) {
		ui->cbManual->setChecked(false);
		ui->cbAuto->setChecked(false);
		for(int i = 0; i < data.solutionconfiguration().altitude_type_size(); i++) {
			if( data.solutionconfiguration().altitude_type(i) == SolverProtocol::manual_altitude ) {
				ui->cbManual->setChecked( true );
			}
			else if( data.solutionconfiguration().altitude_type(i) == SolverProtocol::automatic_altitude ) {
				ui->cbAuto->setChecked( true );
			}
		}

		ui->cbPositionLines->setChecked(false);
		ui->cbOneMarks->setChecked(false);
		ui->cbParticles->setChecked(false);
		ui->cbTrajectories->setChecked(false);
		for( int i = 0; i < data.solutionconfiguration().solution_type_size(); i++ ) {
			if( data.solutionconfiguration().solution_type(i) == SolverProtocol::linesOfPosition ) {
				ui->cbPositionLines->setChecked( true );
			}
			else if( data.solutionconfiguration().solution_type(i) == SolverProtocol::singleMarks ) {
				ui->cbOneMarks->setChecked( true );
			}
			else if( data.solutionconfiguration().solution_type(i) == SolverProtocol::particles ) {
				ui->cbParticles->setChecked( true );
			}
			else if( data.solutionconfiguration().solution_type(i) == SolverProtocol::trajectories ) {
				ui->cbTrajectories->setChecked( true );
			}
		}
	}

	if(data.has_trajectoriesmaxlength()) {
		ui->sbTrajectoryPoints->setValue( data.trajectoriesmaxlength() );
	}

	if(data.has_trajectoriesmaxtimelength()) {
		ui->sbTrajectoryLen->setValue( data.trajectoriesmaxtimelength() );
	}

	if(data.has_detectors()) {
		ClearTableItem(ui->twStations);
		ui->twStations->setRowCount( data.detectors().detector_size() );

		for(int i = 0; i < data.detectors().detector_size(); i++) {
			addTableItem(ui->twStations, i, 0, QString::fromStdString(data.detectors().detector(i).detector_name()), false);

            addTableItem(ui->twStations, i, 1, (data.detectors().detector(i).coords().lon()), true);
            addTableItem(ui->twStations, i, 2, (data.detectors().detector(i).coords().lat()), true);
            addTableItem(ui->twStations, i, 3, (data.detectors().detector(i).coords().alt()), true);
		}
    }

    if(data.has_mindelayscountforsinglemarks()) {
        ui->sbMinDelays->setValue(data.mindelayscountforsinglemarks());
    }

    if(data.has_settingsofmergingmeasurements()) {
        ui->sbMergeTime->setValue(data.settingsofmergingmeasurements().time_window());
        ui->cbIsMerge->setChecked( data.settingsofmergingmeasurements().mergemeasurements() );
    }
}

SolverProtocol::Packet SolverSetupWidget::getSolverParams()
{
    SolverProtocol::Packet pkt;

    SolverProtocol::Packet_DataFromClient_SetRequest* sPkt = pkt.mutable_datafromclient()->mutable_setrequest();

    sPkt->mutable_setdefaultmanualaltitude()->set_defaultmanualaltitude( ui->sbAimHeight->value() );

    sPkt->mutable_setmindelayscountforsinglemarks()->set_mindelayscountforsinglemarks( ui->sbMinDelays->value() );

    sPkt->mutable_settrajectoriesmaxlength()->set_trajectoriesmaxlength(ui->sbTrajectoryPoints->value());

    sPkt->mutable_settrajectoriesmaxtimelength()->set_trajectoriesmaxtimelength(ui->sbTrajectoryLen->value());


    sPkt->mutable_setmindelayscountforsinglemarks()->set_mindelayscountforsinglemarks(ui->sbMinDelays->value());

    sPkt->mutable_setsettingsofmergingmeasurements()->mutable_settingsofmergingmeasurements()->set_time_window(ui->sbMergeTime->value());

    sPkt->mutable_setsettingsofmergingmeasurements()->mutable_settingsofmergingmeasurements()->set_mergemeasurements(ui->cbIsMerge->isChecked());

    setSolutionConfiguration(sPkt);

    return pkt;
}

void SolverSetupWidget::setSolutionConfiguration(SolverProtocol::Packet_DataFromClient_SetRequest* sPkt)
{
	//setting solution configuration
	{
        SolverProtocol::SolutionConfiguration* conf = sPkt->mutable_setsolutionconfiguration()->mutable_solutionconfiguration();
		if(ui->cbAuto->isChecked()) {
			conf->add_altitude_type(SolverProtocol::automatic_altitude);
		}
		if(ui->cbManual->isChecked()) {
			conf->add_altitude_type(SolverProtocol::manual_altitude);
		}
		if(ui->cbPositionLines->isChecked()) {
			conf->add_solution_type( SolverProtocol::linesOfPosition );
		}
		if(ui->cbOneMarks->isChecked()) {
			conf->add_solution_type( SolverProtocol::singleMarks );
		}
		if(ui->cbParticles->isChecked()) {
			conf->add_solution_type( SolverProtocol::particles );
		}
		if(ui->cbTrajectories->isChecked()) {
			conf->add_solution_type( SolverProtocol::trajectories );
		}
	}
}

SolverProtocol::Packet_DataFromClient_SetRequest_SetClientType SolverSetupWidget::getClientParams()
{
	SolverProtocol::Packet_DataFromClient_SetRequest_SetClientType data;
	SolverProtocol::ClientType client = SolverProtocol::ClientType( ui->cbClientType->currentIndex() + 1 );
	data.set_clienttype( client );
	data.set_password( ui->lePassword->text().toStdString() );

	return data;
}

//Table view work
void SolverSetupWidget::addTableItem(QTableWidget* widget, int r, int c, QVariant value, bool isNum)
{
	if(isNum) {
		QDoubleSpinBox* sb = new QDoubleSpinBox(widget);
        sb->setMinimum(-1000);
        sb->setMaximum(1000);
		sb->setValue( value.toDouble() );
		widget->setCellWidget( r, c, sb );
	} else {
		QLineEdit* le = new QLineEdit(widget);
		le->setText(value.toString());
		widget->setCellWidget( r, c, le );
	}
}

//Table view work
void SolverSetupWidget::ClearTableItem(QTableWidget* widget)
{
	widget->clear();
}
