#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "Interfaces/IController.h"

template <class T>
class ViewController : public IController<T>
{
protected:
	T* m_view;

public:
	void appendView(T *view) { m_view = view; }
};

#endif // VIEWCONTROLLER_H
