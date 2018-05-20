#include <QApplication>
#include <QTranslator>
#include "MapWindow.h"


int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	Q_INIT_RESOURCE( demoMapTranslations );
	QTranslator ruTranslator;
	ruTranslator.load( ":/DemoMap_ru.qm" );
	a.installTranslator( &ruTranslator );

	MapWindow window;
	window.show();

	return a.exec();
}


//	translator.load(":/DemoMap_en.qm");
