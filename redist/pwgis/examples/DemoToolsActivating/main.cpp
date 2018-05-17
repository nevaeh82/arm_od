#include <QApplication>
#include "DemoToolsActivating.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoToolsActivating window;
	window.show();

	return a.exec();
}
