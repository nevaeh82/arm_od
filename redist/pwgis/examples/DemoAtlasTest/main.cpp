#include <QApplication>
#include "DemoAtlasTest.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	DemoAtlasTest window;
	window.show();

	return a.exec();
}
