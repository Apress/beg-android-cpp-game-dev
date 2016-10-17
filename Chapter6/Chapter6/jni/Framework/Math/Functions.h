#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

namespace Framework
{
	template<class T>
	inline const T& MIN(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	inline const T& MAX(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	inline uint32_t UpperPow2(uint32_t v)
	{
		--v;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		return ++v;
	}
}

#endif // FUNCTIONS_H_
