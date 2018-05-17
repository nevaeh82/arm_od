#ifndef EASYSINGLETONTEMPLATE_H
#define EASYSINGLETONTEMPLATE_H


template <class T>
class EasySingletonTemplate
{
public:
	static T* instance();
	static T* setInstance(T* p);
protected:
	EasySingletonTemplate(){}
	virtual ~EasySingletonTemplate(){}
	static T* m_instance ;
};

#include "EasySingletonTemplate.hpp"

#endif // EASYSINGLETONTEMPLATE_H
