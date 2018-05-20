#ifndef SINGLETONTEMPLATE_H
#define SINGLETONTEMPLATE_H

template <class T, class I>
class SingletonTemplate
{
public:
	static I* instance();
	static I* setInstance(I* p);
protected:
	SingletonTemplate(){}
	virtual ~SingletonTemplate(){}
	static I* m_instance ;
};

#include "SingletonTemplate.hpp"

#endif
