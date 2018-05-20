#include <QApplication>
#include "DemoBasicFeatures.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DemoBasicFeatures w;
	w.show();

	return a.exec();
}
