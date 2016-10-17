#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <cassert>

namespace Framework
{
	template <typename T>
	class Singleton
	{
	private:
		static T*	ms_singleton;

	public:
		Singleton()
		{
			assert(!ms_singleton);
			ms_singleton = static_cast<T*>(this);
		}

		~Singleton()
		{
			assert(ms_singleton);
			ms_singleton = 0;
		}

		static T& GetSingleton()		{ return *ms_singleton; }
		static T* GetSingletonPtr()	{ return ms_singleton; }
	};

	template <typename T> T* Singleton<T>::ms_singleton = 0;
}

#endif // SINGLETON_H_
