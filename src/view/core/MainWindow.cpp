#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	qRegisterMetaType<QVector<QPointF> >("rpc_send_points_vector");
	qRegisterMetaTypeStreamOperators<QVector<QPointF> >("QVector<QPointF>");
	qRegisterMetaType<rpc_QPointF> ("rpc_QPointF");
	qRegisterMetaType<QMap<int, QVector<QString> > > ("QMap<int, QVector<QString> >");

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	this->setWindowTitle(tr("Zaviruha"));

	this->setWindowIcon(QIcon(":/images/zaviruha.png"));

	this->showMaximized();

	init();

	connect(ui->actionCrash, SIGNAL(triggered(bool)), this, SLOT(crash(bool)));

	m_formCapture = new FormCapture(0);
	connect(m_formCapture, SIGNAL(signalApply(int)), m_formCapture, SLOT(hide()));
	connect(m_formCapture, SIGNAL(signalClear()), m_formCapture, SLOT(hide()));
	connect(m_formCapture, SIGNAL(signalApply(int)), this, SIGNAL(signalApply(int)));
	connect(m_formCapture, SIGNAL(signalClear()), this, SIGNAL(signalClear()));

	m_ktrAddressControl = new KtrAdressControl();
	connect(ui->actionKtr_Settings, SIGNAL(triggered(bool)), m_ktrAddressControl, SLOT(show()));

	QPointF point(34.171325, 45.918281);
	drawAim(point, 50);

	QString path = QApplication::applicationDirPath() + "/Client.ini";
	QSettings settings(path, QSettings::IniFormat);

	if( settings.value("Map/mapExtraBoardInfo").toBool() ) {
		ui->actionShow_extra_board_info->setChecked(true);
	}

	connect(ui->actionShow_extra_board_info, SIGNAL(triggered(bool)), this, SLOT(slotOnExtraBoardInfo(bool)));
}

QPointF MainWindow::drawAim(QPointF pos, int angle)
{
	double BB = 34.023650 - 34.153769;
	double A = (46.018133-45.819489)/BB;
	double B = 1;
	double C = (34.153769*45.819489 - 34.023650*46.018133)/BB;

	double A1 = -tan((double)angle);
	double B1 = 1;
	double C1 = tan((double)angle)*pos.x()-pos.y();

	QPointF res;

	res.setX(  -(  (C*B1-C1*B)/(A*B1-A1*B)  )   );
	double tmp;
	if(angle>180) {
		res.setX( res.x() - 0.2*(1-modf(angle/90, &tmp)) );
	} else {
		res.setX( res.x() + 0.2*(1-modf(angle/90, &tmp)) );
	}

	res.setY(  -(  (A*C1-A1*C)/(A*B1-A1*B)  )   );

	log_debug(QString("aimX: %1").arg(res.x()));
	log_debug(QString("aimY: %1").arg(res.y()));

	return res;
}

MainWindow::~MainWindow()
{
	delete ui;
}

QTabWidget *MainWindow::getMainTabWidget() const
{
	return ui->mainTabWidget;
}

QStackedWidget* MainWindow::getStackedWidget() const
{
	return ui->stackedWidget;
}

QMenu* MainWindow::getViewMenu() const
{
	return ui->menuView;
}

void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect( ui->actionOpen_Atlas, SIGNAL( triggered() ),
		this, SLOT( openAtlasAction() ) );
	connect( ui->actionOpen_from_file, SIGNAL( triggered() ),
		this, SLOT( openMapAction() ) );

	connect( ui->actionClear_map, SIGNAL( triggered() ),
		this, SIGNAL( signalClearMap() ) );
	connect( ui->actionLoad_objects, SIGNAL( triggered() ),
		this, SIGNAL( signalLoadObjects() ) );
	connect( ui->actionSave_objects, SIGNAL( triggered() ),
		this, SIGNAL( signalSaveObjects() ) );

	connect(ui->actionKoordinatometriyaParameters, SIGNAL(triggered()), this, SIGNAL(setupKoordinatometriyaSignal()));

	connect(ui->actionRestartServer, SIGNAL(triggered()), this, SIGNAL(signalResetServer()));

	connect( ui->actionListen_to_ADSB, SIGNAL(triggered(bool)), this, SIGNAL(signalEnableAdsb(bool)) );
	connect( ui->actionListen_to_online_ADSB, SIGNAL(triggered(bool)), this, SIGNAL(signalEnableOnlineAdsb(bool)) );

	connect( ui->actionSolver_Commands, SIGNAL(triggered(bool)), this, SIGNAL(signalSolverCommandsDialog()) );

	connect(ui->actionCount_capture, SIGNAL(triggered(bool)), this, SLOT(slotCaptureCount()));

	connect(ui->actionLoad_Base_stations, SIGNAL(triggered(bool)), this, SIGNAL(signalLoadBaseStations()));
}

void MainWindow::setStateMapAction( bool value )
{
	ui->actionOpen_Atlas->setEnabled( value );
	ui->actionOpen_from_file->setEnabled( value );
}

void MainWindow::slotCaptureCount()
{
	m_formCapture->show();
}

void MainWindow::slotOnExtraBoardInfo(bool val)
{
	emit signalOnExtraBoardInfo((int)val);

	QString path = QApplication::applicationDirPath() + "/Client.ini";
	QSettings settings(path, QSettings::IniFormat);

	settings.setValue("Map/mapExtraBoardInfo", val);
}

void MainWindow::openAtlasAction()
{
	this->setStateMapAction( false );
	emit openAtlasSignal();
}

void MainWindow::openMapAction()
{
	this->setStateMapAction( false );
    emit openMapSignal();
}

void StackOverflow(int depth)
{
    char blockdata[10000];
    printf("Overflow: %d\n", depth);
    StackOverflow(depth+1);
}

void MainWindow::crash(bool a)
{
    int* k = NULL;
    *k = 5;

    //StackOverflow(0);
}

void MainWindow::signalSs(int b)
{
    b = b+1;
}

void MainWindow::mapOpened()
{
	this->setStateMapAction( true );
}

void MainWindow::cancelMapOpen()
{
	this->setStateMapAction( true );
}
