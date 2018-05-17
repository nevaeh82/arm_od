#include <QApplication>
#include "PostTrackingWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PostTrackingWindow window;
	window.show();

	return a.exec();
}
