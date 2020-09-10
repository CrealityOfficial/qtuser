#ifndef _QTUSER_CORE_SINGLETON_1590845087811_H
#define _QTUSER_CORE_SINGLETON_1590845087811_H
#include "qtusercore/qtusercoreexport.h"

template<class T>
class Singleton
{
public:
	Singleton()
	{

	}

	virtual ~Singleton()
	{
		release();
	}

	T* instance()
	{
		if (!single)
		{
			single = new T();
		}

		return single;
	}

	void release()
	{
		if (single)
		{
			delete single;
			single = nullptr;
		}
	}
protected:
	static T* single;
};

template<class T>
T* Singleton<T>::single = nullptr;

#define SINGLETON_DECLARE(x) protected:\
								friend class Singleton<x>;\
								x();

#define SINGLETON_IMPL(x) x* get##x()\
							{ \
								static Singleton<x> s;\
								return s.instance();\
							}

#define SINGLETON_EXPORT(api, x) api x* get##x();
#endif // _QTUSER_CORE_SINGLETON_1590845087811_H
