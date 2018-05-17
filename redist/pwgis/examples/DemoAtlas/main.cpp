#include <QApplication>
#include "AtlasWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	AtlasWindow window;
	window.show();

	return a.exec();
}
