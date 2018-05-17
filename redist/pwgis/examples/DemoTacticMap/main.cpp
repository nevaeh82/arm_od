#include <QApplication>
#include "TacticMapWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	TacticMapWindow window;
	window.show();

	return a.exec();
}
