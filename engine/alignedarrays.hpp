/****************************************************************************
 *            alignedarrays.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f
#define	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "aligned128bitarray.hpp"

/**
 * @file
 * @brief The @c class AlignedArrays.
 * This file contains the @c class AlignedArrays.
 */
namespace eternal_lands
{

	template<class T>
	class AlignedArrays: public Aligned128BitArray
	{
		public:
			typedef typename T::value_type value_type;

			BOOST_STATIC_ASSERT(sizeof(T) == 16);

			inline value_type* get_ptr() noexcept
			{
				return static_cast<value_type*>(
					get_typeless_ptr());
			}

			inline const value_type* get_ptr() const
				noexcept
			{
				return static_cast<const value_type*>(
					get_typeless_ptr());
			}

			inline value_type* get_ptr_at(const size_t index)
				noexcept
			{
				return static_cast<value_type*>(
					get_typeless_ptr_at(index));
			}

			inline const value_type* get_ptr_at(const size_t index)
				const noexcept
			{
				return static_cast<const value_type*>(
					get_typeless_ptr_at(index));
			}

			inline const T &at(const size_t index) const
			{
				return *static_cast<const T*>(
					get_typeless_ptr_at(index));
			}

			inline T &at(const size_t index)
			{
				return *static_cast<T*>(get_typeless_ptr_at(
					index));
			}

			inline const T &operator[](const size_t index)
				const
			{
				return *static_cast<const T*>(
					get_typeless_ptr_at(index));
			}

			inline T &operator[](const size_t index)
			{
				return *static_cast<T*>(get_typeless_ptr_at(
					index));
			}

			inline void push_back(const T &value)
			{
				prepare_push_back();

				at(size() - 1) = value;
			}

	};

	typedef AlignedArrays<glm::vec4> AlignedVec4Array;
	typedef AlignedArrays<glm::ivec4> AlignedIvec4Array;
	typedef AlignedArrays<glm::uvec4> AlignedUvec4Array;
	typedef AlignedArrays<Sint16Array8> AlignedSint16Vec8Array;
	typedef AlignedArrays<Uint16Array8> AlignedUint16Vec8Array;
	typedef AlignedArrays<Sint8Array16> AlignedSint8Vec16Array;
	typedef AlignedArrays<Uint8Array16> AlignedUint8Vec16Array;

}

#endif	/* UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f */
