#include <QApplication>
#include "DemoStyleWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoStyleWindow window;
	window.show();

	return a.exec();
}
