#include <QApplication>
#include "PwGisWidgetVersionWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	PwGisWidgetVersionWindow window;
	window.show();

	return a.exec();
}
