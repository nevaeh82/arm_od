#include <QApplication>
#include "SimulatorWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SimulatorWindow window;
	window.show();

	return a.exec();
}
