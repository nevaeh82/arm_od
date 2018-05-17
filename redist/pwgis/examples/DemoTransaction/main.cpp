#include <QApplication>
#include "DemoTransaction.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DemoTransaction w;
	w.show();

	return a.exec();
}
