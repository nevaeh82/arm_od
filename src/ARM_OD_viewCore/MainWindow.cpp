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

void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect( ui->actionOpen_Atlas, SIGNAL( triggered() ),
		this, SLOT( openAtlasAction() ) );
	connect( ui->actionOpen_from_file, SIGNAL( triggered() ),
		this, SLOT( openMapAction() ) );

	connect(ui->actionKoordinatometriyaParameters, SIGNAL(triggered()), this, SIGNAL(setupKoordinatometriyaSignal()));
}

void MainWindow::setStateMapAction( bool value )
{
	ui->actionOpen_Atlas->setEnabled( value );
	ui->actionOpen_from_file->setEnabled( value );
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

void MainWindow::mapOpened()
{
	this->setStateMapAction( true );
}

void MainWindow::cancelMapOpen()
{
	this->setStateMapAction( true );
}
