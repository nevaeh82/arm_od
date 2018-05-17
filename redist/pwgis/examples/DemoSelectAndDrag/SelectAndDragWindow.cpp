#include <QFileDialog>
#include <QDebug>
#include <typeinfo>

#include <MapProvider.h>
#include <PwGis/IProfileManager.h>
#include <PwGis/IObjectsManager.h>
#include <PwGis/MapObjectEvents.h>
#include <PwGis/IMapManager.h>
#include <PwGis/MapManagerEvents.h>

#include "SelectAndDragWindow.h"
#include "ui_SelectAndDragWindow.h"

SelectAndDragWindow::SelectAndDragWindow( QWidget *parent ) :
	QDialog( parent ),
	ui( new Ui::SelectAndDragWindow )
{
	ui->setupUi(this);
	this->showMaximized();

	_mapProvider = ui->pwgiswidget->mapProvider();

	connect( ui->btnOpenMap, SIGNAL( clicked() ), SLOT( onBtnOpenMap() ) );
	connect( ui->btnOpenAtlas, SIGNAL( clicked() ), SLOT( onBtnOpenAtlas() ) );

	Pw::Gis::MapManagerEvents* mapEvents = &(ui->pwgiswidget->mapProvider()->mapManager()->events());
	connect(mapEvents, SIGNAL(mapReady()), this, SLOT(onMapReady()));

	Pw::Gis::MapObjectEvents* mapObjectEvents = &(_mapProvider->objectsManager()->events());

	connect( mapObjectEvents, SIGNAL( featureSelected(const QString&, const QString&)),
			 this, SLOT( featureSelected(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureUnselected(const QString&, const QString&)),
			 this, SLOT( featureUnselected(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureDragStart(const QString&, const QString&)),
			 this, SLOT( featureDragStart(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureDrag(const QString&, const QString&)),
			 this, SLOT( featureDrag(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureDragFinish(const QString&, const QString&)),
			 this, SLOT( featureDragFinish(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureClicked(const QString&, const QString&)),
			 this, SLOT( featureClicked(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureAdded(const QString&, const QString&)),
			 this, SLOT( featureAdded(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( featureModified(const QString&, const QString&)),
			 this, SLOT( featureModified(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( markerSelected(const QString&, const QString&)),
			 this, SLOT( markerSelected(const QString&, const QString&) ) );
	connect( mapObjectEvents, SIGNAL( markerUnselected(const QString&, const QString&)),
			 this, SLOT( markerUnselected(const QString&, const QString&) ) );
	connect( ui->pwgiswidget, SIGNAL( pathMeasured(const QVariantList&, double, int)),
			 this, SLOT( pathMeasured(const QVariantList&, double, int) ) );
	//ui->pwgiswidget->enableDebugger(true);

}

void SelectAndDragWindow::onMapReady()
{
	double lon1 = 30.309621;
	double lat1 = 59.936765;
	double lon2 = -7.20;
	double lat2 = 53.27;

	ui->pwgiswidget->addMarker("m1", lon1, lat1, "not selectable marker", "not selectable marker", 0, "");
	ui->pwgiswidget->addMarker("m1sel", lon2, lat2, "selectable marker", "selectable marker", 0, "behavour");
	ui->pwgiswidget->addRectangle("r1", lon1 + 0.02, lat1 + 0.02, lon1 + 0.12, lat1 + 0.12, "not selectable rectangle", "not selectable rectangle", "");
	ui->pwgiswidget->addRectangle("r1sel", lon2 + 0.13, lat2 + 0.13, lon2 + 0.23, lat2 + 0.23, "selectable rectangle", "selectable rectangle", "behaviour");

	ui->checkBoxMarkerSelected->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureSelected->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureUnselected->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureDragStart->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureDrag->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureDragFinish->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureClicked->setCheckState(Qt::Unchecked);
	ui->checkBoxFeatureAdded->setCheckState(Qt::Unchecked);
	ui->checkBoxMarkerSelected->setCheckState(Qt::Unchecked);
	ui->checkBoxMarkerUnselected->setCheckState(Qt::Unchecked);
}

SelectAndDragWindow::~SelectAndDragWindow()
{
	_mapProvider->mapManager()->closeMap();
	delete ui;
}

void SelectAndDragWindow::onBtnOpenMap()
{
	QString profileName = "DemoSelectAndDrag";
	ui->pwgiswidget->mapProvider()->profileManager()->registerProfile(profileName, ":/DemoSelectAndDrag/profile");
	_mapProvider->mapManager()->setProfile(profileName);

	QString mapPath = QFileDialog::getOpenFileName(this,tr( "Open map" ),
			QApplication::applicationDirPath(), tr( "Map files(*.*)" ) );

	if( mapPath.isEmpty() ){
		return;
	}

	_mapProvider->mapManager()->openMap(mapPath);
}

void SelectAndDragWindow::onBtnOpenAtlas()
{
	_mapProvider->mapManager()->openAtlas(9910);
}

void SelectAndDragWindow::featureSelected( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureSelected->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureUnselected( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureUnselected->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureDragStart( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureDragStart->setCheckState(Qt::Checked);
}

void SelectAndDragWindow:: featureDrag( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureDrag->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureDragFinish( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureDragFinish->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureClicked( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureClicked->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureAdded( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureAdded->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::featureModified(const QString& id, const QString& type)
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxFeatureModified->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::pathMeasured(const QVariantList& points, double measure, int state)
{
	QStringList pointsInString;
	for (int i=1; i < points.length(); i+=2) {
		pointsInString << QString("%1x%2").arg(points[i-1].toInt()).arg(points[i].toInt());
	}

	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("((%1), %2, %3)")
								 .arg(pointsInString.join("; ")).arg(measure).arg(state) ));
	ui->checkBoxMeasured->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::markerSelected( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxMarkerSelected->setCheckState(Qt::Checked);
}

void SelectAndDragWindow::markerUnselected( const QString &id, const QString& type )
{
	ui->txtLog->append( QString( QString(__FUNCTION__) + QString("(%1:%2)").arg(id).arg(type) ));
	ui->checkBoxMarkerUnselected->setCheckState(Qt::Checked);
}
