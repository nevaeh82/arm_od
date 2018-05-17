#include <QApplication>
#include "BearingLifetimeWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	BearingLifetimeWindow window;
	window.show();

	return a.exec();
}
