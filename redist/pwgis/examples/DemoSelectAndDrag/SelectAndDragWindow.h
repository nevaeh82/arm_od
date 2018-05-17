#ifndef SELECTANDDRAGWINDOW_H
#define SELECTANDDRAGWINDOW_H

#include <pwgiswidget.h>
#include <PwGis/TacticMap/TacticMap.h>
#include <PwGis/TacticMap/ITacticMap.h>
#include <QDialog>
#include <PwGis/TacticMap/TracedObjectAbstract.h>

namespace Ui {
class SelectAndDragWindow;
}

class SelectAndDragWindow : public QDialog
{
	Q_OBJECT

public:
	explicit SelectAndDragWindow( QWidget *parent = 0 );
	~SelectAndDragWindow();

private:
	ITacticMap* _tacticMap;
	IIdGenerator* _idGenerator;
	IMapProvider* _mapProvider;
	Ui::SelectAndDragWindow *ui;

	QList<TracedObjectAbstract*> _mapObjectList;

	/** random number between low and high
	  */
	int randInt( int low, int high );
	/** creating point with random coordinates
	  */
	PwGisLonLat* createRandomPoint( QObject* obj );

	/** Get a map object by a unique number
	*/
	TracedObjectAbstract* getMapObject( const QString &id );

private slots:
	void onMapReady();

	void onBtnOpenMap();
	void onBtnOpenAtlas();

	void markerSelected( const QString& id, const QString& type );
	void markerUnselected( const QString& id, const QString& type );
	void featureSelected( const QString& id, const QString& type );
	void featureUnselected( const QString& id, const QString& type );
	void featureDragStart( const QString& id, const QString& type );
	void featureDrag( const QString& id, const QString& type );
	void featureDragFinish( const QString& id, const QString& type );
	void featureClicked( const QString& id, const QString& type );
	void featureAdded( const QString& id, const QString& type );
	void featureModified( const QString& id, const QString& type );
	void pathMeasured( const QVariantList& points, double measure, int state );
};

#endif // SELECTANDDRAGWINDOW_H
