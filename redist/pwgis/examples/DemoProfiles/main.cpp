#include <QApplication>
#include "DemoProfiles.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoProfiles window;
	window.show();

	return a.exec();
}
