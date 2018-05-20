#include <QApplication>
#include "ReperWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	ReperWindow window;
	window.show();

	return a.exec();
}
