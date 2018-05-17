#include <QApplication>
#include "DemoStyleCombine.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoStyleCombine window;
	window.show();

	return a.exec();
}
