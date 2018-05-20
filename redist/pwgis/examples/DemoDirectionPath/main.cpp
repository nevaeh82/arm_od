#include <QApplication>
#include "DemoDirectionPath.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	DemoDirectionPath window;
	window.show();

	return a.exec();
}
