#include <QApplication>
#include "SatelliteSpotWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	SatelliteSpotWindow window;
	window.show();

	return a.exec();
}
