#include <QApplication>
#include "IngitMapWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	IngitMapWindow window;
	window.show();

	return a.exec();
}
