#include <QApplication>
#include "UserLayersWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UserLayersWindow w;
	w.show();

	return a.exec();
}
