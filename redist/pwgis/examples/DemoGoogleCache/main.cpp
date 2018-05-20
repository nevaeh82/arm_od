#include <QApplication>
#include "GoogleCacheMapWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GoogleCacheMapWindow window;
	window.show();

	return a.exec();
}
