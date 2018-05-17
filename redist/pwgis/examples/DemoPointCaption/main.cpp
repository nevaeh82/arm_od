#include <QApplication>
#include "PointCaptionWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	PointCaptionWindow window;
	window.show();

	return a.exec();
}
