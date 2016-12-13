#include "SolverSetupWidget.h"
#include "ui_SolverSetupWidget.h"

#include <QDateTime>

SolverSetupWidget::SolverSetupWidget(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SolverSetupWidget),
	m_workAreaSetCounter(0)
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

	connect(ui->pbAddStation, SIGNAL(clicked(bool)), this, SLOT(slotAddStation()));
	connect(ui->pbRemoveStation, SIGNAL(clicked(bool)), this, SLOT(slotRemoveStation()));

	ui->pbSetAll->setEnabled(false);
	ui->pbGetAll->setEnabled(true);
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

	if(data.has_areaofresponsibility()) {
		addTableItem(ui->twWorkRect, 0, 0, (data.areaofresponsibility().mincoordinates().lon()), true);
		addTableItem(ui->twWorkRect, 0, 1, (data.areaofresponsibility().mincoordinates().lat()), true);

		addTableItem(ui->twWorkRect, 1, 0, (data.areaofresponsibility().maxcoordinates().lon()), true);
		addTableItem(ui->twWorkRect, 1, 1, (data.areaofresponsibility().maxcoordinates().lat()), true);
	}

	if(data.has_sourcecoordsforcorrecting()) {
		SolverProtocol::Coordinates coord = data.sourcecoordsforcorrecting().coords();
		addTableItem(ui->twIri, 0, 0, (coord.lon()), true);
		addTableItem(ui->twIri, 0, 1, (coord.lat()), true);
		addTableItem(ui->twIri, 0, 2, (coord.alt()), true);
	}

	if(data.has_mindelayscountforsinglemarks()) {
		ui->sbMinDelays->setValue(data.mindelayscountforsinglemarks());
	}

	if(data.has_settingsofmergingmeasurements()) {
		ui->sbMergeTime->setValue(data.settingsofmergingmeasurements().time_window());
		ui->cbIsMerge->setChecked( data.settingsofmergingmeasurements().mergemeasurements() );
	}

	if(data.has_updatemode()) {
		ui->cbUpdateMode->setCurrentIndex( (int)data.updatemode()-1 );
	}

	if(data.has_timetype()) {
		ui->cbTimeMode->setCurrentIndex( (int)data.timetype()-1 );
	}

	if(data.has_multitargettrackingsettings()) {
		const SolverProtocol::MultitargetTrackingSettings multiSettings = data.multitargettrackingsettings();

		ui->sbAlpha->setValue( multiSettings.alpha() );
		ui->sbBeta->setValue( multiSettings.beta() );
		ui->sbParts->setValue( multiSettings.particlescount() );
		ui->sbActiveParts->setValue( multiSettings.visibleparticlescount() );
		ui->sbProbabilityDensity->setValue( multiSettings.clutterdencity() );
		ui->sbProbabilityNoize->setValue( multiSettings.clutterprior() );
		ui->sbAimProbability->setValue( multiSettings.probabilityofbirth() );
		ui->cbCorrectManualHeight->setChecked( multiSettings.correctaltitudeinmanualmode() );
		ui->cbManyAimsOnFreq->setChecked( multiSettings.manytargetsatonefrequency() );
		ui->cbManyFreqOnAim->setChecked(multiSettings.manyfrequenciesatonetarget());

		ui->pbSetAll->setEnabled(true); //TODO For all!
	}

	if(data.has_coorrections()) {

		ui->twWorkCorrections->setRowCount( data.coorrections().correction_size() );
		ui->twWorkCorrections->setColumnCount(1);

		QStringList headers;
		for(int i = 0; i<data.coorrections().correction_size(); i++ ) {
			SolverProtocol::Corrections_Correction corr = data.coorrections().correction(i);
			headers.append(QString("%1-%2").arg(corr.first_detector_index())
						   .arg(corr.second_detector_index()));
//			ui->twCurrentCorrections->verticalHeaderItem(i)->setText( QString("%1-%2").arg(corr.first_detector_index())
//																					  .arg(corr.second_detector_index())
//																	  );
		}
		ui->twWorkCorrections->setVerticalHeaderLabels(headers);

		for(int i = 0; i<data.coorrections().correction_size(); i++ ) {
			SolverProtocol::Corrections_Correction corr = data.coorrections().correction(i);
			addTableItem( ui->twWorkCorrections, i, 0, QString::number(corr.correction()*1000000), false );

		}
	}
}

SolverProtocol::Packet SolverSetupWidget::getSolverParams()
{
	SolverProtocol::Packet pkt;

	SolverProtocol::Packet_DataFromClient_SetRequest* sPkt = pkt.mutable_datafromclient()->mutable_setrequest();

	sPkt->mutable_setdefaultmanualaltitude()->set_defaultmanualaltitude( ui->sbAimHeight->value() );

	sPkt->mutable_settrajectoriesmaxlength()->set_trajectoriesmaxlength(ui->sbTrajectoryPoints->value());

	sPkt->mutable_settrajectoriesmaxtimelength()->set_trajectoriesmaxtimelength(ui->sbTrajectoryLen->value());

	sPkt->mutable_setmaxallowablelocationerror()->set_maxallowablelocationerror( ui->sbPositionError->value() );
	sPkt->mutable_setmaxallowablevelocity()->set_maxallowablevelocity( ui->sbSpeed->value() );
	sPkt->mutable_setmaxallowablerangessdv()->set_maxallowablerangessdv( ui->sbSKO->value() );

	sPkt->mutable_setmindelayscountforsinglemarks()->set_mindelayscountforsinglemarks(ui->sbMinDelays->value());

	sPkt->mutable_setsettingsofmergingmeasurements()->mutable_settingsofmergingmeasurements()->set_time_window(ui->sbMergeTime->value());

	sPkt->mutable_setsettingsofmergingmeasurements()->mutable_settingsofmergingmeasurements()->set_mergemeasurements(ui->cbIsMerge->isChecked());

	//Additional
	sPkt->mutable_setupdatemodetype()->set_updatemode( SolverProtocol::UpdateModeType(ui->cbUpdateMode->currentIndex()+1) );
	sPkt->mutable_settimetype()->set_timetype( SolverProtocol::TimeType(ui->cbTimeMode->currentIndex()+1) );

	SolverProtocol::MultitargetTrackingSettings* multiSettings =
			sPkt->mutable_setmultitargettrackingsettings()->mutable_multitargettrackingsettings();
	multiSettings->set_alpha( ui->sbAlpha->value() );
	multiSettings->set_beta( ui->sbBeta->value() );
	multiSettings->set_particlescount( ui->sbParts->value() );
	multiSettings->set_visibleparticlescount( ui->sbActiveParts->value() );
	multiSettings->set_clutterdencity( ui->sbProbabilityDensity->value() );
	multiSettings->set_clutterprior( ui->sbProbabilityNoize->value() );
	multiSettings->set_probabilityofbirth( ui->sbAimProbability->value() );
	multiSettings->set_correctaltitudeinmanualmode( ui->cbCorrectManualHeight->isChecked() );
	multiSettings->set_manytargetsatonefrequency( ui->cbManyAimsOnFreq->isChecked() );
	multiSettings->set_manyfrequenciesatonetarget( ui->cbManyFreqOnAim->isChecked() );

	//Work Area
	SolverProtocol::Coordinates* coord = sPkt->mutable_setareaofresponsibility()->mutable_areaofresponsibility()->mutable_mincoordinates();
	coord->set_lon( dynamic_cast<QDoubleSpinBox*>(ui->twWorkRect->cellWidget(0, 0))->value() );
	coord->set_lat( dynamic_cast<QDoubleSpinBox*>(ui->twWorkRect->cellWidget(0, 1))->value() );
	coord->set_alt( 0 );

	coord = sPkt->mutable_setareaofresponsibility()->mutable_areaofresponsibility()->mutable_maxcoordinates();
	coord->set_lon( dynamic_cast<QDoubleSpinBox*>(ui->twWorkRect->cellWidget(1, 0))->value() );
	coord->set_lat( dynamic_cast<QDoubleSpinBox*>(ui->twWorkRect->cellWidget(1, 1))->value() );
	coord->set_alt( 0 );

	//IRI
	SolverProtocol::CoordinatesInTime* tcoord = sPkt->mutable_setsourcecoordinates()->mutable_sourcecoordsforcorrecting();
	tcoord->set_setting_date_time( QDateTime::currentMSecsSinceEpoch() );
	coord = tcoord->mutable_coords();
	coord->set_lon( dynamic_cast<QDoubleSpinBox*>(ui->twIri->cellWidget(0, 0))->value() );
	coord->set_lat( dynamic_cast<QDoubleSpinBox*>(ui->twIri->cellWidget(0, 1))->value() );
	coord->set_alt( dynamic_cast<QDoubleSpinBox*>(ui->twIri->cellWidget(0, 2))->value() );

	//Detectors
	SolverProtocol::Detectors* detectors = sPkt->mutable_setdetectors()->mutable_detectors();
	int rows = ui->twStations->rowCount();
	for( int i = 0; i < rows; i++ ) {
		SolverProtocol::Detectors_Detector* det = detectors->add_detector();
		det->set_detector_name( dynamic_cast<QLineEdit*>(ui->twStations->cellWidget(i, 0))->text().toStdString() );
		det->mutable_coords()->set_lon(dynamic_cast<QDoubleSpinBox*>(ui->twStations->cellWidget(i, 1))->value());
		det->mutable_coords()->set_lat(dynamic_cast<QDoubleSpinBox*>(ui->twStations->cellWidget(i, 2))->value());
		det->mutable_coords()->set_alt(dynamic_cast<QDoubleSpinBox*>(ui->twStations->cellWidget(i, 3))->value());
	}

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

void SolverSetupWidget::setMapClick(double lon, double lat)
{
	if( ui->cbWorkAreaFromMap->isChecked() ) {
		if(m_workAreaSetCounter == 0) {
			addTableItem( ui->twWorkRect, 0, 0, lon, true);
			addTableItem( ui->twWorkRect, 0, 1, lat, true);
			m_workAreaSetCounter++;
		} else {
			addTableItem( ui->twWorkRect, 1, 0, lon, true);
			addTableItem( ui->twWorkRect, 1, 1, lat, true);
			m_workAreaSetCounter = 0;
			ui->cbWorkAreaFromMap->setChecked(false);
		}
	}

	if(ui->cbIriFromMap->isChecked()) {
		addTableItem( ui->twIri, 0, 0, lon, true );
		addTableItem( ui->twIri, 0, 1, lat, true );

		ui->cbIriFromMap->setChecked(false);
	}

	if(ui->cbStationOnMap->isChecked()) {
		if( !ui->twStations->selectedRanges().isEmpty() ) {
			int row = ui->twStations->selectedRanges().first().topRow();
			addTableItem( ui->twStations, row, 1, lon, true );
			addTableItem( ui->twStations, row, 2, lat, true );
		}

		ui->cbStationOnMap->setChecked(false);
	}
}

void SolverSetupWidget::slotAddStation()
{
	int row = ui->twStations->rowCount();
	ui->twStations->insertRow( row );
	addTableItem( ui->twStations, row, 0, "newStation", false );
	addTableItem( ui->twStations, row, 1, 0, true );
	addTableItem( ui->twStations, row, 2, 0, true );
	addTableItem( ui->twStations, row, 3, 0, true );
}

void SolverSetupWidget::slotRemoveStation()
{
	while (!ui->twStations->selectedRanges().isEmpty()) {
		int row = ui->twStations->selectedRanges().first().topRow();
		ui->twStations->removeRow( row );
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
	bool isSet = false;
	if(isNum) {
		QDoubleSpinBox* sb = dynamic_cast<QDoubleSpinBox*>(widget->cellWidget(r, c));
		if(!sb) {
			sb = new QDoubleSpinBox(widget);
			isSet = true;
		}
		sb->setMinimum(-180);
		sb->setMaximum(180);
		sb->setValue( value.toDouble() );

		sb->setSuffix( " °" );
		if(isSet) {
			widget->setCellWidget( r, c, sb );
		}
	} else {
		QLineEdit* le =  dynamic_cast<QLineEdit*>(widget->cellWidget(r, c));
		if(!le) {
			le = new QLineEdit(widget);
			isSet = true;
		}

		le->setText(value.toString());
		if(isSet) {
			widget->setCellWidget( r, c, le );
		}
	}
}

//Table view work
void SolverSetupWidget::ClearTableItem(QTableWidget* widget)
{
	widget->clear();
}
