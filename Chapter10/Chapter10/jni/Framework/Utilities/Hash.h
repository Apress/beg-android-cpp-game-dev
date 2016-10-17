#ifndef HASH_H_
#define HASH_H_

#include <string>

namespace Framework
{
	// SDBM Hash
	inline unsigned int Hash(const std::string& key)
	{
		unsigned int result = 0;

		for (unsigned int i=0; i<key.length(); ++i)
		{
			int c = key[i];
            result = c + (result << 6) + (result << 16) - result;
		}

		return result;
	}
}

#endif // HASH_H_
