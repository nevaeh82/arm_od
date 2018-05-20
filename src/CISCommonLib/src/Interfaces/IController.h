#ifndef ICONTROLLER_H
#define ICONTROLLER_H

template <class T>
class IController
{
public:
	virtual void appendView(T *view) = 0;
};


#endif // ICONTROLLER_H
