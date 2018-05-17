#include <QApplication>
#include "BorderMapWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	BorderMapWindow window;
	window.show();

	return a.exec();
}
