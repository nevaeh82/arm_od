#include <QApplication>
#include "DemoIconsReview.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoIconsReview window;
	window.show();

	return a.exec();
}
