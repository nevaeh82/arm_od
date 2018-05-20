#include <QApplication>
#include "DemoMapBounds.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoMapBounds window;
	window.show();

	return a.exec();
}
