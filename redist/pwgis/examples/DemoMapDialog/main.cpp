#include <QApplication>
#include "MapDialogWindow.h"

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	MapDialogWindow window;
	window.show();

	return a.exec();
}
