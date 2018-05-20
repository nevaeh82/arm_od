#include <QApplication>
#include "MapxWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	MapxWindow window;
	window.show();

	return a.exec();
}
