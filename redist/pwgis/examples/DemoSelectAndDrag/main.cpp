#include <QApplication>
#include "SelectAndDragWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SelectAndDragWindow window;
	window.show();

	return a.exec();
}
