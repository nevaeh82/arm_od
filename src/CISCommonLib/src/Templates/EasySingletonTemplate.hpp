#ifndef EASYSIGLETON_IMPL_H
#define EASYSIGLETON_IMPL_H

template <class T> T* EasySingletonTemplate<T>::m_instance = 0;

template <class T>
T* EasySingletonTemplate<T>::instance () {
	if (m_instance == 0) {
		m_instance =  new T;
	}
	return m_instance;
}


template <class T>
T* EasySingletonTemplate<T>::setInstance(T* p)
{
	T* prevInstance = m_instance;
	m_instance = p;
	return prevInstance;
}


#endif
