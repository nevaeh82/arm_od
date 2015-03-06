#include "ARM_OD_Application.h"

#include <QDebug>

ARM_OD_Application::ARM_OD_Application(int& argc, char** argv)
	: QtSingleCoreApplication(argc, argv)
{
}


bool ARM_OD_Application::notify(QObject* receiver, QEvent* event) {
  bool done = true;
  try
  {
    done = QCoreApplication::notify(receiver, event);
  } catch (const std::exception& e)
  {
      qFatal("Error %s sending event %s to object %s (%s)",
                  e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
                  typeid(*receiver).name());

  } catch (...)
  {
      qDebug() << "catch2";
  }
  return done;
}
