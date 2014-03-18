#ifndef SIGLETON_IMPL_H
#define SIGLETON_IMPL_H
template <class T, class I> I* SingletonTemplate<T,I>::m_instance = 0;

template <class T, class I>
I* SingletonTemplate<T,I>::instance () {
	if (m_instance == 0) {
		m_instance =  new T;
	}
	return m_instance;
}


template <class T, class I>
I* SingletonTemplate<T,I>::setInstance(I* p)
{
	I* prevInstance = m_instance;
	m_instance = p;
	return prevInstance;
}

#endif
