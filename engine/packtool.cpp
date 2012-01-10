/****************************************************************************
 *            packtool.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "packtool.hpp"
#include "abstractreadmemorybuffer.hpp"
#include "abstractwritememorybuffer.hpp"
#include "alignedvec4array.hpp"
#include "simd/simd.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		/**
		* Mask for upper 3 bits
		*/
		const Uint16 XSIGN_MASK = 0x8000;
		const Uint16 YSIGN_MASK = 0x4000;
		const Uint16 ZSIGN_MASK = 0x2000;

		/*
		* Mask for middle 6 bits - xbits
		*/
		const Uint16 TOP_MASK = 0x1F80;

		/*
		* Mask for lower 7 bits - ybits
		*/
		const Uint16 BOTTOM_MASK = 0x007F;

		template <typename T, bool normalize>
		static inline T convert_from_float(const float value)
		{
			float tmp, min, max;

			BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);

			tmp = value;

			if (value < 0.0f)
			{
				if (normalize)
				{
					tmp *= std::numeric_limits<T>::min();

					if (std::numeric_limits<T>::is_signed)
					{
						tmp = -tmp;
					}
				}

				tmp -= 0.5f;
			}
			else
			{
				if (normalize)
				{
					tmp *= std::numeric_limits<T>::max();
				}

				tmp += 0.5f;
			}

			if (normalize)
			{
				if (std::numeric_limits<T>::is_signed)
				{
					min = -1.0f;
				}
				else
				{
					min = 0.0f;
				}

				max = 1.0f;
			}
			else
			{
				min = std::numeric_limits<T>::min();
				max = std::numeric_limits<T>::max();
			}

			try
			{
				return boost::numeric_cast<T>(tmp);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_float_range_min(min);
				exception << errinfo_float_range_max(max);
				exception << errinfo_float_range_index(value);
				throw;
			}
			catch (const std::exception &exception)
			{
				EL_THROW_EXCEPTION(RangeErrorException()
					<< errinfo_float_range_min(min)
					<< errinfo_float_range_max(max)
					<< errinfo_float_range_index(value));
			}
		}

		template <typename T, Uint16 count, bool normalize>
		void convert_from_vec4(const glm::vec4 &value, T* ptr)
		{
			Uint32 i;

			BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);

			for (i = 0; i < count; ++i)
			{
				ptr[i] = convert_from_float<T, normalize>(
					value[i]);
			}
		}

		template <typename T, bool normalize>
		static inline float convert_to_float(const T value)
		{
			float tmp;

			BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);

			tmp = value;

			if (normalize)
			{
				if (value < static_cast<T>(0))
				{
					tmp = -tmp /
						std::numeric_limits<T>::min();
				}
				else
				{
					tmp /= std::numeric_limits<T>::max();
				}
			}

			return tmp;
		}

		template <typename T, Uint16 count, bool normalize>
		glm::vec4 convert_to_vec4(const T* ptr)
		{
			glm::vec4 result;
			Uint32 i;

			BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);

			for (i = 0; i < count; ++i)
			{
				result[i] = convert_to_float<T, normalize>(
					ptr[i]);
			}

			return result;
		}

		template <typename T, Uint16 x, Uint16 y, Uint16 z,
			Uint16 w, bool signed_value, bool normalize,
			bool reverse>
		glm::vec4 unpack_values(const T value)
		{
			glm::vec4 result;
			glm::uvec4 masks, bits, shifts;
			Sint32 tmp, max_value, min_value, total;
			Uint32 i, shift, mask, count;

			if (reverse)
			{
				shift = 0;
			}
			else
			{
				shift = x + y + z + w;
			}

			bits[0] = x;
			bits[1] = y;
			bits[2] = z;
			bits[3] = w;

			count = 0;

			for (i = 0; i < 4; ++i)
			{
				if (bits[i] > 0)
				{
					count++;
				}
			}

			if (reverse)
			{
				switch (count)
				{
					case 2:
						std::swap(bits.x, bits.y);
						break;
					case 3:
						std::swap(bits.x, bits.z);
						break;
					case 4:
						std::swap(bits.x, bits.w);
						std::swap(bits.y, bits.z);
						break;
				}
			}

			for (i = 0; i < 4; ++i)
			{
				if (!reverse)
				{
					shift -= bits[i];
				}

				mask = (1 << bits[i]) - 1;
				masks[i] = mask << shift;
				shifts[i] = shift;

				if (reverse)
				{
					shift += bits[i];
				}
			}

			for (i = 0; i < count; ++i)
			{
				tmp = ((value & masks[i]) >> shifts[i]);

				total = 1 << bits[i];

				if (signed_value)
				{
					max_value = (1 << (bits[i] - 1)) - 1;
					min_value = -(1 << (bits[i] - 1));
				}
				else
				{
					max_value = (1 << bits[i]) - 1;
					min_value = 0;
				}

				if (tmp > max_value)
				{
					tmp = tmp - total;
				}

				if (normalize)
				{
					if (tmp < 0.0f)
					{
						result[i] = static_cast<float>(
							tmp) / -min_value;
					}
					else
					{
						result[i] = static_cast<float>(
							tmp) / max_value;
					}
				}
				else
				{
					result[i] = tmp;
				}
			}

			return result;
		}

		template <typename T, Uint16 x, Uint16 y, Uint16 z,
			Uint16 w, bool signed_value, bool normalize,
			bool reverse>
		T pack_values(const glm::vec4 &value)
		{
			T result;
			glm::uvec4 masks, bits, shifts;
			float temp;
			Sint32 tmp, max_value, min_value, total;
			Uint32 i, shift, mask, count;

			if (reverse)
			{
				shift = 0;
			}
			else
			{
				shift = x + y + z + w;
			}

			bits[0] = x;
			bits[1] = y;
			bits[2] = z;
			bits[3] = w;

			count = 0;

			for (i = 0; i < 4; ++i)
			{
				if (bits[i] > 0)
				{
					count++;
				}
			}

			if (reverse)
			{
				switch (count)
				{
					case 2:
						std::swap(bits.x, bits.y);
						break;
					case 3:
						std::swap(bits.x, bits.z);
						break;
					case 4:
						std::swap(bits.x, bits.w);
						std::swap(bits.y, bits.z);
						break;
				}
			}

			for (i = 0; i < 4; ++i)
			{
				if (!reverse)
				{
					shift -= bits[i];
				}

				mask = (1 << bits[i]) - 1;
				masks[i] = mask << shift;
				shifts[i] = shift;

				if (reverse)
				{
					shift += bits[i];
				}
			}

			result = 0;

			for (i = 0; i < count; ++i)
			{
				temp = value[i];

				total = 1 << bits[i];

				if (signed_value)
				{
					max_value = (1 << (bits[i] - 1)) - 1;
					min_value = -(1 << (bits[i] - 1));
				}
				else
				{
					max_value = (1 << bits[i]) - 1;
					min_value = 0;
				}

				if (normalize)
				{
					if (signed_value)
					{
						temp = std::min(1.0f,
							std::max(-1.0f, temp));
					}
					else
					{
						temp = std::min(1.0f,
							std::max(0.0f, temp));
					}

					if (temp < 0.0f)
					{
						tmp = total;
						tmp -= static_cast<Sint32>(temp
							* min_value + 0.5f);
					}
					else
					{
						tmp = temp * max_value + 0.5f;
					}
				}
				else
				{
					temp = std::min(static_cast<float>(
						max_value), temp);
					temp = std::max(static_cast<float>(
						min_value), temp);

					if (temp < 0.0f)
					{
						tmp = total + static_cast<
							Sint32>(temp - 0.5f);
					}
					else
					{
						tmp = temp + 0.5f;
					}
				}

				result |= (tmp << shifts[i]) & masks[i];
			}

			return result;
		}

	}

	Uint16 PackTool::compress_normalized(const glm::vec3 &value)
	{
		glm::vec3 tmp;
		float w;
		Uint32 xbits, ybits;
		Uint16 result;

		result = 0;
		tmp = value;

		if (tmp[0] < 0.0f)
		{
			result |= XSIGN_MASK;
			tmp[0] = -tmp[0];
		}

		if (tmp[1] < 0.0f)
		{
			result |= YSIGN_MASK;
			tmp[1] = -tmp[1];
		}

		if (tmp[2] < 0.0f)
		{
			result |= ZSIGN_MASK;
			tmp[2] = -tmp[2];
		}

		// project the normal onto the plane that goes through
		// X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).

		// on that plane we choose an (projective!) coordinate system
		// such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

		// a little slower... old pack was 4 multiplies and 2 adds.
		// This is 2 multiplies, 2 adds, and a divide....
		w = 126.0f / (tmp[0] + tmp[1] + tmp[2]);
		xbits = static_cast<Uint32>(tmp[0] * w + 0.5f);
		ybits = static_cast<Uint32>(tmp[1] * w + 0.5f);

		// Now we can be sure that 0<=xp<=126, 0<=yp<=126, 0<=xp+yp<=126

		// however for the sampling we want to transform this triangle
		// into a rectangle.
		if (xbits >= 64)
		{
			xbits = 127 - xbits;
			ybits = 127 - ybits;
		}

		// now we that have xp in the range (0,127) and yp in the range (0,63),
		// we can pack all the bits together
		result |= xbits << 7;
		result |= ybits;

		return result;
	}

	glm::vec3 PackTool::uncompress_normalized(const Uint16 value)
	{
		glm::vec3 result;
		Uint32 x, y;

		/**
		 * if we do a straightforward backward transform
		 * we will get points on the plane X0,Y0,Z0
		 * however we need points on a sphere that goes through
		 * these points. Therefore we need to adjust x,y,z so
		 * that x^2+y^2+z^2=1 by normalizing the vector. We have
		 * already precalculated the amount by which we need to
		 * scale, so all we do is a table lookup and a
		 * multiplication
		 * get the x and y bits
		 */

		x = (value & TOP_MASK) >> 7;
		y = value & BOTTOM_MASK;

		// map the numbers back to the triangle (0,0)-(0,126)-(126,0)
		if ((x + y) >= 127)
		{
			x = 127 - x;
			y = 127 - y;
		}

		/**
		 * do the inverse transform and normalization
		 * costs 3 extra multiplies and 2 subtracts. No big deal.
		 */
		result[0] = x;
		result[1] = y;
		result[2] = 126 - x - y;

		// set all the sign bits
		if (value & XSIGN_MASK)
		{
			result[0] = -result[0];
		}

		if (value & YSIGN_MASK)
		{
			result[1] = -result[1];
		}

		if (value & ZSIGN_MASK)
		{
			result[2] = -result[2];
		}

		return glm::normalize(result);
	}

	glm::vec3 PackTool::unpack_uint_3_3_2(const bool normalize, const Uint8 value)
	{
		if (normalize)
		{
			return glm::vec3(unpack_values<Uint8, 3, 3, 2, 0, false, true, false>(value));
		}
		else
		{
			return glm::vec3(unpack_values<Uint8, 3, 3, 2, 0, false, false, false>(value));
		}
	}

	glm::vec3 PackTool::unpack_uint_2_3_3_rev(const bool normalize, const Uint8 value)
	{
		if (normalize)
		{
			return glm::vec3(unpack_values<Uint8, 2, 3, 3, 0, false, true, true>(value));
		}
		else
		{
			return glm::vec3(unpack_values<Uint8, 2, 3, 3, 0, false, false, true>(value));
		}
	}

	glm::vec3 PackTool::unpack_uint_5_6_5(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return glm::vec3(unpack_values<Uint16, 5, 6, 5, 0, false, true, false>(value));
		}
		else
		{
			return glm::vec3(unpack_values<Uint16, 5, 6, 5, 0, false, false, false>(value));
		}
	}

	glm::vec3 PackTool::unpack_uint_5_6_5_rev(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return glm::vec3(unpack_values<Uint16, 5, 6, 5, 0, false, true, true>(value));
		}
		else
		{
			return glm::vec3(unpack_values<Uint16, 5, 6, 5, 0, false, false, true>(value));
		}
	}

	glm::vec4 PackTool::unpack_uint_5_5_5_1(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return unpack_values<Uint16, 5, 5, 5, 1, false, true, false>(value);
		}
		else
		{
			return unpack_values<Uint16, 5, 5, 5, 1, false, false, false>(value);
		}
	}

	glm::vec4 PackTool::unpack_uint_1_5_5_5_rev(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return unpack_values<Uint16, 1, 5, 5, 5, false, true, true>(value);
		}
		else
		{
			return unpack_values<Uint16, 1, 5, 5, 5, false, false, true>(value);
		}
	}

	glm::vec4 PackTool::unpack_uint_4_4_4_4(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return unpack_values<Uint16, 4, 4, 4, 4, false, true, false>(value);
		}
		else
		{
			return unpack_values<Uint16, 4, 4, 4, 4, false, false, false>(value);
		}
	}

	glm::vec4 PackTool::unpack_uint_4_4_4_4_rev(const bool normalize, const Uint16 value)
	{
		if (normalize)
		{
			return unpack_values<Uint16, 4, 4, 4, 4, false, true, true>(value);
		}
		else
		{
			return unpack_values<Uint16, 4, 4, 4, 4, false, false, true>(value);
		}
	}

	glm::vec4 PackTool::unpack_uint_10_10_10_2(const bool normalize, const Uint32 value)
	{
		if (normalize)
		{
			return unpack_values<Uint32, 10, 10, 10, 2, false, true, false>(value);
		}
		else
		{
			return unpack_values<Uint32, 10, 10, 10, 2, false, false, false>(value);
		}
	}

	glm::vec4 PackTool::unpack_uint_2_10_10_10_rev(const bool normalize, const Uint32 value)
	{
		if (normalize)
		{
			return unpack_values<Uint32, 2, 10, 10, 10, false, true, true>(value);
		}
		else
		{
			return unpack_values<Uint32, 2, 10, 10, 10, false, false, true>(value);
		}
	}

	glm::vec4 PackTool::unpack_sint_10_10_10_2(const bool normalize, const Uint32 value)
	{
		if (normalize)
		{
			return unpack_values<Uint32, 10, 10, 10, 2, true, true, false>(value);
		}
		else
		{
			return unpack_values<Uint32, 10, 10, 10, 2, true, false, false>(value);
		}
	}

	glm::vec4 PackTool::unpack_sint_2_10_10_10_rev(const bool normalize, const Uint32 value)
	{
		if (normalize)
		{
			return unpack_values<Uint32, 2, 10, 10, 10, true, true, true>(value);
		}
		else
		{
			return unpack_values<Uint32, 2, 10, 10, 10, true, false, true>(value);
		}
	}

	Uint8 PackTool::pack_uint_3_3_2(const bool normalize, const glm::vec3 &value)
	{
		if (normalize)
		{
			return pack_values<Uint8, 3, 3, 2, 0, false, true, false>(glm::vec4(value, 0.0f));
		}
		else
		{
			return pack_values<Uint8, 3, 3, 2, 0, false, false, false>(glm::vec4(value, 0.0f));
		}
	}

	Uint8 PackTool::pack_uint_2_3_3_rev(const bool normalize, const glm::vec3 &value)
	{
		if (normalize)
		{
			return pack_values<Uint8, 2, 3, 3, 0, false, true, true>(glm::vec4(value, 0.0f));
		}
		else
		{
			return pack_values<Uint8, 2, 3, 3, 0, false, false, true>(glm::vec4(value, 0.0f));
		}
	}

	Uint16 PackTool::pack_uint_5_6_5(const bool normalize, const glm::vec3 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 5, 6, 5, 0, false, true, false>(glm::vec4(value, 0.0f));
		}
		else
		{
			return pack_values<Uint16, 5, 6, 5, 0, false, false, false>(glm::vec4(value, 0.0f));
		}
	}

	Uint16 PackTool::pack_uint_5_6_5_rev(const bool normalize, const glm::vec3 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 5, 6, 5, 0, false, true, true>(glm::vec4(value, 0.0f));
		}
		else
		{
			return pack_values<Uint16, 5, 6, 5, 0, false, false, true>(glm::vec4(value, 0.0f));
		}
	}

	Uint16 PackTool::pack_uint_5_5_5_1(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 5, 5, 5, 1, false, true, false>(value);
		}
		else
		{
			return pack_values<Uint16, 5, 5, 5, 1, false, false, false>(value);
		}
	}

	Uint16 PackTool::pack_uint_1_5_5_5_rev(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 1, 5, 5, 5, false, true, true>(value);
		}
		else
		{
			return pack_values<Uint16, 1, 5, 5, 5, false, false, true>(value);
		}
	}

	Uint16 PackTool::pack_uint_4_4_4_4(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 4, 4, 4, 4, false, true, false>(value);
		}
		else
		{
			return pack_values<Uint16, 4, 4, 4, 4, false, false, false>(value);
		}
	}

	Uint16 PackTool::pack_uint_4_4_4_4_rev(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint16, 4, 4, 4, 4, false, true, true>(value);
		}
		else
		{
			return pack_values<Uint16, 4, 4, 4, 4, false, false, true>(value);
		}
	}

	Uint32 PackTool::pack_uint_10_10_10_2(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint32, 10, 10, 10, 2, false, true, false>(value);
		}
		else
		{
			return pack_values<Uint32, 10, 10, 10, 2, false, false, false>(value);
		}
	}

	Uint32 PackTool::pack_uint_2_10_10_10_rev(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint32, 2, 10, 10, 10, false, true, true>(value);
		}
		else
		{
			return pack_values<Uint32, 2, 10, 10, 10, false, false, true>(value);
		}
	}

	Uint32 PackTool::pack_sint_10_10_10_2(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint32, 10, 10, 10, 2, true, true, false>(value);
		}
		else
		{
			return pack_values<Uint32, 10, 10, 10, 2, true, false, false>(value);
		}
	}

	Uint32 PackTool::pack_sint_2_10_10_10_rev(const bool normalize, const glm::vec4 &value)
	{
		if (normalize)
		{
			return pack_values<Uint32, 2, 10, 10, 10, true, true, true>(value);
		}
		else
		{
			return pack_values<Uint32, 2, 10, 10, 10, true, false, true>(value);
		}
	}

#define PACK_FORMAT(name, type, normalized, ptr)	\
	case pft_##name##_1:	\
		return convert_to_vec4<type, 1, normalized>(	\
			static_cast<const type*>(ptr));	\
	case pft_##name##_2:	\
		return convert_to_vec4<type, 2, normalized>(	\
			static_cast<const type*>(ptr));	\
	case pft_##name##_3:	\
		return convert_to_vec4<type, 3, normalized>(	\
			static_cast<const type*>(ptr));	\
	case pft_##name##_4:	\
		return convert_to_vec4<type, 4, normalized>(	\
			static_cast<const type*>(ptr));

#define PACK_FORMAT_INT(name, type, ptr)	\
	PACK_FORMAT(unsigned_##name, unsigned type, false, ptr)	\
	PACK_FORMAT(signed_##name, signed type, false, ptr)	\
	PACK_FORMAT(unsigned_normalized_##name, unsigned type, true, ptr)	\
	PACK_FORMAT(signed_normalized_##name, signed type, true, ptr)

#define PACK_FORMAT_PACKED_3(name, type, x, y, z, ptr)	\
	case pft_unsigned_##name##_##x##_##y##_##z:	\
		return unpack_values<type, x, y, z, 0, false, false, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_##name##_##x##_##y##_##z:	\
		return unpack_values<type, x, y, z, 0, true, false, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_normalized_##name##_##x##_##y##_##z:	\
		return unpack_values<type, x, y, z, 0, false, true, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_normalized_##name##_##x##_##y##_##z:	\
		return unpack_values<type, x, y, z, 0, true, true, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_##name##_rev_##z##_##y##_##x:	\
		return unpack_values<type, z, y, x, 0, false, false, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_##name##_rev_##z##_##y##_##x:	\
		return unpack_values<type, z, y, x, 0, true, false, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_normalized_##name##_rev_##z##_##y##_##x:	\
		return unpack_values<type, z, y, x, 0, false, true, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_normalized_##name##_rev_##z##_##y##_##x:	\
		return unpack_values<type, z, y, x, 0, true, true, true>(	\
			*static_cast<const type*>(ptr));

#define PACK_FORMAT_PACKED_4(name, type, x, y, z, w, ptr)	\
	case pft_unsigned_##name##_##x##_##y##_##z##_##w:	\
		return unpack_values<type, x, y, z, w, false, false, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_##name##_##x##_##y##_##z##_##w:	\
		return unpack_values<type, x, y, z, w, true, false, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_normalized_##name##_##x##_##y##_##z##_##w:	\
		return unpack_values<type, x, y, z, w, false, true, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_normalized_##name##_##x##_##y##_##z##_##w:	\
		return unpack_values<type, x, y, z, w, true, true, false>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_##name##_rev_##w##_##z##_##y##_##x:	\
		return unpack_values<type, w, z, y, x, false, false, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_##name##_rev_##w##_##z##_##y##_##x:	\
		return unpack_values<type, w, z, y, x, true, false, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_unsigned_normalized_##name##_rev_##w##_##z##_##y##_##x:	\
		return unpack_values<type, w, z, y, x, false, true, true>(	\
			*static_cast<const type*>(ptr));	\
	case pft_signed_normalized_##name##_rev_##w##_##z##_##y##_##x:	\
		return unpack_values<type, w, z, y, x, true, true, true>(	\
			*static_cast<const type*>(ptr));

	glm::vec4 PackTool::unpack_vec4(const AbstractReadMemoryBuffer &buffer,
		const Uint64 offset, const PackFormatType type)
	{
		glm::vec4 result;
		const void* ptr;

		ptr = static_cast<const Uint8*>(buffer.get_ptr()) + offset;

		switch (type)
		{
			PACK_FORMAT_INT(byte, char, ptr)
			PACK_FORMAT_INT(short, short, ptr)
			PACK_FORMAT_INT(int, int, ptr)
			PACK_FORMAT_PACKED_3(byte, Uint8, 3, 3, 2, ptr)
			PACK_FORMAT_PACKED_4(short, Uint16, 4, 4, 4, 4, ptr)
			PACK_FORMAT_PACKED_3(short, Uint16, 5, 6, 5, ptr)
			PACK_FORMAT_PACKED_4(short, Uint16, 5, 5, 5, 1, ptr)
			PACK_FORMAT_PACKED_4(int, Uint32, 10, 10, 10, 2, ptr)
			case pft_float_4:
				result[3] = static_cast<const float*>(ptr)[3];
			case pft_float_3:
				result[2] = static_cast<const float*>(ptr)[2];
			case pft_float_2:
				result[1] = static_cast<const float*>(ptr)[1];
			case pft_float_1:
				result[0] = static_cast<const float*>(ptr)[0];
				return result;
			case pft_half_4:
				result[3] = glm::detail::toFloat32(
					static_cast<const glm::detail::hdata*>(
						ptr)[3]);
			case pft_half_3:
				result[2] = glm::detail::toFloat32(
					static_cast<const glm::detail::hdata*>(
						ptr)[2]);
			case pft_half_2:
				result[1] = glm::detail::toFloat32(
					static_cast<const glm::detail::hdata*>(
						ptr)[1]);
			case pft_half_1:
				result[0] = glm::detail::toFloat32(
					static_cast<const glm::detail::hdata*>(
						ptr)[0]);
				return result;
		}

		return glm::vec4(0.0f);
	}

#undef	PACK_FORMAT
#undef	PACK_FORMAT_INT
#undef	PACK_FORMAT_PACKED_3
#undef	PACK_FORMAT_PACKED_4

#define PACK_FORMAT(name, type, normalized, data, ptr)	\
	case pft_##name##_1:	\
		convert_from_vec4<type, 1, normalized>(data,	\
			static_cast<type*>(ptr));	\
		return;	\
	case pft_##name##_2:	\
		convert_from_vec4<type, 2, normalized>(data,	\
			static_cast<type*>(ptr));	\
		return;	\
	case pft_##name##_3:	\
		convert_from_vec4<type, 3, normalized>(data,	\
			static_cast<type*>(ptr));	\
		return;	\
	case pft_##name##_4:	\
		convert_from_vec4<type, 4, normalized>(data,	\
			static_cast<type*>(ptr));	\
		return;

#define PACK_FORMAT_INT(name, type, data, ptr)	\
	PACK_FORMAT(unsigned_##name, unsigned type, false, data, ptr)	\
	PACK_FORMAT(signed_##name, signed type, false, data, ptr)	\
	PACK_FORMAT(unsigned_normalized_##name, unsigned type, true, data, ptr)	\
	PACK_FORMAT(signed_normalized_##name, signed type, true, data, ptr)

#define PACK_FORMAT_PACKED_3(name, type, x, y, z, data, ptr)	\
	case pft_unsigned_##name##_##x##_##y##_##z:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, 0, false,	\
			false, false>(data);	\
		return;	\
	case pft_signed_##name##_##x##_##y##_##z:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, 0, true,	\
			false, false>(data);	\
		return;	\
	case pft_unsigned_normalized_##name##_##x##_##y##_##z:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, 0, false,	\
			true, false>(data);	\
		return;	\
	case pft_signed_normalized_##name##_##x##_##y##_##z:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, 0, true,	\
			true, false>(data);	\
		return;	\
	case pft_unsigned_##name##_rev_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, z, y, x, 0, false,	\
			false, true>(data);	\
		return;	\
	case pft_signed_##name##_rev_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, z, y, x, 0, true,	\
			false, true>(data);	\
		return;	\
	case pft_unsigned_normalized_##name##_rev_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, z, y, x, 0, false,	\
			true, true>(data);	\
		return;	\
	case pft_signed_normalized_##name##_rev_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, z, y, x, 0, true,	\
			true, true>(data);	\
		return;

#define PACK_FORMAT_PACKED_4(name, type, x, y, z, w, data, ptr)	\
	case pft_unsigned_##name##_##x##_##y##_##z##_##w:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, w, false,	\
			false, false>(data);	\
		return;	\
	case pft_signed_##name##_##x##_##y##_##z##_##w:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, w, true,	\
			false, false>(data);	\
		return;	\
	case pft_unsigned_normalized_##name##_##x##_##y##_##z##_##w:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, w, false,	\
			true, false>(data);	\
		return;	\
	case pft_signed_normalized_##name##_##x##_##y##_##z##_##w:	\
		*static_cast<type*>(ptr) = pack_values<type, x, y, z, w, true,	\
			true, false>(data);	\
		return;	\
	case pft_unsigned_##name##_rev_##w##_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, w, z, y, x, false,	\
			false, true>(data);	\
		return;	\
	case pft_signed_##name##_rev_##w##_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, w, z, y, x, true,	\
			false, true>(data);	\
		return;	\
	case pft_unsigned_normalized_##name##_rev_##w##_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, w, z, y, x, false,	\
			true, true>(data);	\
		return;	\
	case pft_signed_normalized_##name##_rev_##w##_##z##_##y##_##x:	\
		*static_cast<type*>(ptr) = pack_values<type, w, z, y, x, true,	\
			true, true>(data);	\
		return;

	void PackTool::pack(const glm::vec4 &data, const Uint64 offset,
		const PackFormatType pack_format,
		AbstractWriteMemoryBuffer &buffer)
	{
		void* ptr;

		ptr = static_cast<Uint8*>(buffer.get_ptr()) + offset;

		try
		{
			switch (pack_format)
			{
				PACK_FORMAT_INT(byte, char, data, ptr)
				PACK_FORMAT_INT(short, short, data, ptr)
				PACK_FORMAT_INT(int, int, data, ptr)
				PACK_FORMAT_PACKED_3(byte, Uint8, 3, 3, 2,
					data, ptr)
				PACK_FORMAT_PACKED_4(short, Uint16, 4, 4, 4, 4,
					data, ptr)
				PACK_FORMAT_PACKED_3(short, Uint16, 5, 6, 5,
					data, ptr)
				PACK_FORMAT_PACKED_4(short, Uint16, 5, 5, 5, 1,
					data, ptr)
				PACK_FORMAT_PACKED_4(int, Uint32, 10, 10, 10, 2,
					data, ptr)
				case pft_float_4:
					static_cast<float*>(ptr)[3] = data[3];
				case pft_float_3:
					static_cast<float*>(ptr)[2] = data[2];
				case pft_float_2:
					static_cast<float*>(ptr)[1] = data[1];
				case pft_float_1:
					static_cast<float*>(ptr)[0] = data[0];
					return;
				case pft_half_4:
					static_cast<glm::detail::hdata*>(ptr)[3]
						=
						glm::detail::toFloat16(data[3]);
				case pft_half_3:
					static_cast<glm::detail::hdata*>(ptr)[2]
						=
						glm::detail::toFloat16(data[2]);
				case pft_half_2:
					static_cast<glm::detail::hdata*>(ptr)[1]
						=
						glm::detail::toFloat16(data[1]);
				case pft_half_1:
					static_cast<glm::detail::hdata*>(ptr)[0]
						=
						glm::detail::toFloat16(data[0]);
					return;
			}
		}
		catch (boost::exception &exception)
		{
			StringStream str;

			str << pack_format;

			exception << errinfo_type_name(str.str());
			throw;
		}
	}

#undef	PACK_FORMAT
#undef	PACK_FORMAT_INT
#undef	PACK_FORMAT_PACKED_3
#undef	PACK_FORMAT_PACKED_4

	void PackTool::pack(const Vec4Vector &data, const Uint64 offset,
		const Uint32 stride, const Uint32 count,
		const PackFormatType pack_format,
		AbstractWriteMemoryBuffer &buffer)
	{
		Uint32 i;

		for (i = 0; i < count; ++i)
		{
			pack(data[i], offset + i * stride, pack_format, buffer);
		}
	}


	void PackTool::pack(const AlignedVec4Array &data, const Uint64 offset,
		const Uint32 stride, const Uint32 count,
		const PackFormatType pack_format, const bool use_simd,
		AbstractWriteMemoryBuffer &buffer)
	{
		float* source;
		Uint8* ptr;
		Uint32 i;

		ptr = static_cast<Uint8*>(buffer.get_ptr()) + offset;

		if (!(SIMD::get_supported() && use_simd))
		{
			for (i = 0; i < count; ++i)
			{
				pack(data[i], offset + i * stride, pack_format,
					buffer);
			}

			return;
		}

		source = static_cast<float*>(data.get_ptr());

		assert((((size_t)source) & 0xF) == 0);

		if (pack_format == pft_half_4)
		{
			SIMD::float_to_half_4(source, count, stride, ptr);

			return;
		}

		if (pack_format == pft_half_2)
		{
			SIMD::float_to_half_2(source, count, stride, ptr);

			return;
		}

		if (pack_format == pft_unsigned_short_4)
		{
			SIMD::float_to_unsigned_short_4(source, count, stride,
				ptr);

			return;
		}

		if (pack_format == pft_unsigned_short_2)
		{
			SIMD::float_to_unsigned_short_2(source, count, stride,
				ptr);

			return;
		}

		if (pack_format == pft_unsigned_normalized_short_4)
		{
			SIMD::float_to_unsigned_normalized_short_4(source,
				count, stride, ptr);

			return;
		}

		if (pack_format == pft_unsigned_normalized_short_2)
		{
			SIMD::float_to_unsigned_normalized_short_2(source,
				count, stride, ptr);

			return;
		}

		if (pack_format == pft_signed_short_4)
		{
			SIMD::float_to_signed_short_4(source, count, stride,
				ptr);

			return;
		}

		if (pack_format == pft_signed_short_2)
		{
			SIMD::float_to_signed_short_2(source, count, stride,
				ptr);

			return;
		}

		if (pack_format == pft_signed_normalized_short_4)
		{
			SIMD::float_to_signed_normalized_short_4(source, count,
				stride, ptr);

			return;
		}

		if (pack_format == pft_signed_normalized_short_2)
		{
			SIMD::float_to_signed_normalized_short_2(source, count,
				stride, ptr);

			return;
		}

		if (pack_format == pft_unsigned_byte_4)
		{
			SIMD::float_to_unsigned_byte_4(source, count, stride,
				ptr);

			return;
		}

		if (pack_format == pft_unsigned_normalized_byte_4)
		{
			SIMD::float_to_unsigned_normalized_byte_4(source,
				count, stride, ptr);

			return;
		}

		for (i = 0; i < count; ++i)
		{
			pack(data[i], offset + i * stride, pack_format, buffer);
		}
	}

}
