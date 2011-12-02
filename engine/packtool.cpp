/****************************************************************************
 *            packtool.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "packtool.hpp"

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

		template <const bool signed_value, Uint32 X, Uint32 Y, Uint32 Z,
			Uint32 W, typename T>
		glm::vec4 unpack_values(const bool normalize, const T value)
		{
			glm::vec4 result;
			Uint32Array4 mask;
			Sint32 tmp, max_value, min_value, total;
			Uint32 i, shift;

			mask[0] = X;
			mask[1] = Y;
			mask[2] = Z;
			mask[3] = W;

			for (i = 0; i < 4; i++)
			{
				if (mask[i] > 0)
				{
					shift = __builtin_ctz(mask[i]);
					tmp = ((value & mask[i]) >> shift);

					total = 1 << __builtin_popcount(mask[i]);

					if (signed_value)
					{
						max_value = (1 << (__builtin_popcount(mask[i]) - 1)) - 1;
						min_value = -(1 << (__builtin_popcount(mask[i]) - 1));
					}
					else
					{
						max_value = (1 << __builtin_popcount(mask[i])) - 1;
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
							result[i] = static_cast<float>(tmp) / -min_value;
						}
						else
						{
							result[i] = static_cast<float>(tmp) / max_value;
						}
					}
					else
					{
						result[i] = tmp;
					}
				}
			}

			return result;
		}

		template <typename T, bool signed_value, Uint32 X, Uint32 Y,
			Uint32 Z, Uint32 W>
		T pack_values(const bool normalize, const glm::vec4 &value)
		{
			T result;
			Uint32Array4 mask;
			float temp;
			Sint32 tmp, max_value, min_value, total;
			Uint32 i, shift;

			mask[0] = X;
			mask[1] = Y;
			mask[2] = Z;
			mask[3] = W;

			result = 0;

			for (i = 0; i < 4; i++)
			{
				if (mask[i] > 0)
				{
					shift = __builtin_ctz(mask[i]);
					temp = value[i];

					total = 1 << __builtin_popcount(mask[i]);

					if (signed_value)
					{
						max_value = (1 << (__builtin_popcount(mask[i]) - 1)) - 1;
						min_value = -(1 << (__builtin_popcount(mask[i]) - 1));
					}
					else
					{
						max_value = (1 << __builtin_popcount(mask[i])) - 1;
						min_value = 0;
					}

					if (normalize)
					{
						if (signed_value)
						{
							temp = std::min(1.0f, std::max(-1.0f, temp));
						}
						else
						{
							temp = std::min(1.0f, std::max(0.0f, temp));
						}

						if (temp < 0.0f)
						{
							tmp = total - static_cast<Sint32>(temp * min_value + 0.5f);
						}
						else
						{
							tmp = temp * max_value + 0.5f;
						}
					}
					else
					{
						temp = std::min(static_cast<float>(max_value), temp);
						temp = std::max(static_cast<float>(min_value), temp);

						if (temp < 0.0f)
						{
							tmp = total + static_cast<Sint32>(temp - 0.5f);
						}
						else
						{
							tmp = temp + 0.5f;
						}
					}

					result |= (tmp << shift) & mask[i];
				}
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
		return glm::vec3(unpack_values<false, 0xE0, 0x1C, 0x03, 0x00>(normalize, value));
	}

	glm::vec3 PackTool::unpack_uint_2_3_3_rev(const bool normalize, const Uint8 value)
	{
		return glm::vec3(unpack_values<false, 0x07, 0x38, 0xC0, 0x00>(normalize, value));
	}

	glm::vec3 PackTool::unpack_uint_5_6_5(const bool normalize, const Uint16 value)
	{
		return glm::vec3(unpack_values<false, 0x001F, 0x07E0, 0xF800, 0x0000>(normalize, value));
	}

	glm::vec3 PackTool::unpack_uint_5_6_5_rev(const bool normalize, const Uint16 value)
	{
		return glm::vec3(unpack_values<false, 0xF800, 0x07E0, 0x001F, 0x0000>(normalize, value));
	}

	glm::vec4 PackTool::unpack_uint_5_5_5_1(const bool normalize, const Uint16 value)
	{
		return unpack_values<false, 0xF800, 0x07C0, 0x003E, 0x0001>(normalize, value);
	}

	glm::vec4 PackTool::unpack_uint_1_5_5_5_rev(const bool normalize, const Uint16 value)
	{
		return unpack_values<false, 0x8000, 0x7C00, 0x03E0, 0x001F>(normalize, value);
	}

	glm::vec4 PackTool::unpack_uint_4_4_4_4(const bool normalize, const Uint16 value)
	{
		return unpack_values<false, 0xF000, 0x0F00, 0x00F0, 0x000F>(normalize, value);
	}

	glm::vec4 PackTool::unpack_uint_4_4_4_4_rev(const bool normalize, const Uint16 value)
	{
		return unpack_values<false, 0x000F, 0x00F0, 0x0F00, 0xF000>(normalize, value);
	}

	glm::vec4 PackTool::unpack_uint_10_10_10_2(const bool normalize, const Uint32 value)
	{
		return unpack_values<false, 0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003>(normalize, value);
	}

	glm::vec4 PackTool::unpack_uint_2_10_10_10_rev(const bool normalize, const Uint32 value)
	{
		return unpack_values<false, 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000>(normalize, value);
	}

	glm::vec4 PackTool::unpack_sint_10_10_10_2(const bool normalize, const Uint32 value)
	{
		return unpack_values<true, 0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003>(normalize, value);
	}

	glm::vec4 PackTool::unpack_sint_2_10_10_10_rev(const bool normalize, const Uint32 value)
	{
		return unpack_values<true, 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000>(normalize, value);
	}

	Uint8 PackTool::pack_uint_3_3_2(const bool normalize, const glm::vec3 &value)
	{
		return pack_values<Uint8, false, 0xE0, 0x1C, 0x03, 0x00>(normalize, glm::vec4(value, 0.0f));
	}

	Uint8 PackTool::pack_uint_2_3_3_rev(const bool normalize, const glm::vec3 &value)
	{
		return pack_values<Uint8, false, 0x07, 0x38, 0xC0, 0x00>(normalize, glm::vec4(value, 0.0f));
	}

	Uint16 PackTool::pack_uint_5_6_5(const bool normalize, const glm::vec3 &value)
	{
		return pack_values<Uint16, false, 0x001F, 0x07E0, 0xF800, 0x0000>(normalize, glm::vec4(value, 0.0f));
	}

	Uint16 PackTool::pack_uint_5_6_5_rev(const bool normalize, const glm::vec3 &value)
	{
		return pack_values<Uint16, false, 0xF800, 0x07E0, 0x001F, 0x0000>(normalize, glm::vec4(value, 0.0f));
	}

	Uint16 PackTool::pack_uint_5_5_5_1(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint16, false, 0xF800, 0x07C0, 0x003E, 0x0001>(normalize, value);
	}

	Uint16 PackTool::pack_uint_1_5_5_5_rev(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint16, false, 0x8000, 0x7C00, 0x03E0, 0x001F>(normalize, value);
	}

	Uint16 PackTool::pack_uint_4_4_4_4(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint16, false, 0xF000, 0x0F00, 0x00F0, 0x000F>(normalize, value);
	}

	Uint16 PackTool::pack_uint_4_4_4_4_rev(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint16, false, 0x000F, 0x00F0, 0x0F00, 0xF000>(normalize, value);
	}

	Uint32 PackTool::pack_uint_10_10_10_2(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint32, false, 0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003>(normalize, value);
	}

	Uint32 PackTool::pack_uint_2_10_10_10_rev(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint32, false, 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000>(normalize, value);
	}

	Uint32 PackTool::pack_sint_10_10_10_2(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint64, true, 0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003>(normalize, value);
	}

	Uint32 PackTool::pack_sint_2_10_10_10_rev(const bool normalize, const glm::vec4 &value)
	{
		return pack_values<Uint64, true, 0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000>(normalize, value);
	}

}
