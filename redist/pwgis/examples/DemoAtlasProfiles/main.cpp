#include <QApplication>
#include "DemoAtlasProfiles.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoAtlasProfiles window;
	window.show();

	return a.exec();
}
