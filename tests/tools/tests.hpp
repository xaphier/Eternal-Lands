/****************************************************************************
 *            tests.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7b5905fe_8b1b_427a_b1fc_62f373aec63a
#define	UUID_7b5905fe_8b1b_427a_b1fc_62f373aec63a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

template <typename T>
bool value_in_set(const std::set<T> &set, const T value)
{
	return set.count(value) > 0;
}

template <typename T>
bool value_not_in_set(const std::set<T> &set, const T value)
{
	return set.count(value) == 0;
}

namespace std
{

	template <typename T>
	std::ostream &operator<<(std::ostream &str, const std::set<T> &set)
	{
		typename std::set<T>::const_iterator it, end, begin;

		end = set.end();
		begin = set.begin();

		str << "{ ";

		for (it = begin; it != end; ++it)
		{
			if (it != begin)
			{
				str << ", ";
			}

			str << *it;
		}

		str << " }";

		return str;
	}

}

#endif	/* UUID_7b5905fe_8b1b_427a_b1fc_62f373aec63a */
