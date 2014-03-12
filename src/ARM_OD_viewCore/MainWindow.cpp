#include "MainWindow.h"
#include "ui_arm_od_view.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ARM_OD_view)
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

	QIcon icon("images/zaviruha.png");
	this->setWindowIcon(icon);

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

void MainWindow::init()
{
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionOpen_Atlas, SIGNAL(triggered()), this, SIGNAL(openAtlasSignal()));
	connect(ui->actionOpen_from_file, SIGNAL(triggered()), this, SIGNAL(openMapSignal()));
	connect(ui->actionKoordinatometriyaParameters, SIGNAL(triggered()), this, SIGNAL(setupKoordinatometriyaSignal()));
}


