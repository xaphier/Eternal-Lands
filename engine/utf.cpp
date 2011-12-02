/****************************************************************************
 *            utf.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "utf.hpp"

namespace eternal_lands
{

	namespace
	{

		bool get_valid_utf8_continuation_byte(const Uint8 utf8)
		{
			return (0x80 <= utf8) && (utf8 <= 0xBF);
		}

		Uint32 get_utf8_count(const Uint8 utf8)
		{
			if (utf8 <= 0x7F)
			{
				return 1;
			}

			if (utf8 <= 0xBF)
			{
				return 0;
			}

			if (utf8 <= 0xC1)
			{
				return 0;
			}

			if (utf8 <= 0xDF)
			{
				return 2;
			}

			if (utf8 <= 0xEF)
			{
				return 3;
			}

			if (utf8 <= 0xF4)
			{
				return 4;
			}

			return 0;
		}

		bool get_valid_utf8_start_byte(const Uint8 utf8)
		{
			return get_utf8_count(utf8) > 0;
		}

		Uint32 get_len_as_utf8(const Uint32 utf32)
		{
			if (utf32 <= 0x00007F)
			{
				return 1;
			}

			if (utf32 <= 0x0007FF)
			{
				return 2;
			}

			if (utf32 <= 0x00FFFF)
			{
				return 3;
			}

			if (utf32 <= 0x10FFFF)
			{
				return 4;
			}

			return 0;
		}

		Uint32 get_len_as_utf16(const Uint32 utf32)
		{
			if (utf32 <= 0x00FFFF)
			{
				return 1;
			}

			if (utf32 <= 0x10FFFF)
			{
				return 2;
			}

			return 0;
		}

		Uint32 utf8_to_utf32(const Uint8Array4 &utf8, const Uint32 size,
			Uint32 &utf32)
		{
			Uint32 count, i;

			assert(size > 0);

			count = get_utf8_count(utf8[0]);

			if (size < count)
			{
				return 0;
			}

			for (i = 1; i < count; i++)
			{
				if ((0x80 > utf8[i]) || (utf8[i] > 0xBF))
				{
					return 0;
				}
			}

			switch (count)
			{
				case 1:
					utf32 = utf8[0] & 0x7F;
					return 1;
				case 2:
					utf32 = utf8[0] & 0x1F;
					utf32 = (utf32 << 6) | (utf8[1] & 0x3F);
					return 2;
				case 3:
					utf32 = utf8[0] & 0x0F;
					utf32 = (utf32 << 6) | (utf8[1] & 0x3F);
					utf32 = (utf32 << 6) | (utf8[2] & 0x3F);
					return 3;
				case 4:
					utf32 = utf8[0] & 0x07;
					utf32 = (utf32 << 6) | (utf8[1] & 0x3F);
					utf32 = (utf32 << 6) | (utf8[2] & 0x3F);
					utf32 = (utf32 << 6) | (utf8[3] & 0x3F);
					return 4;
				default:
					return 0;
			}
		}

		Uint32 utf16_to_utf32(const Uint16Array2 &utf16,
			const Uint32 size, Uint32 &utf32)
		{
			assert((size > 0));

			if ((0xD800 > utf16[0]) || (utf16[0] > 0xDFFF))
			{
				utf32 = utf16[0];

				return 1;
			}

			if ((size < 2) || (utf16[0] > 0xDBFF))
			{
				return 0;
			}

			if ((0xDC00 > utf16[1]) || (utf16[1] > 0xDFFF))
			{
				return 0;
			}

			utf32 = (utf16[0] & 0x03FF) << 10;
			utf32 |= utf16[1] & 0x03FF;
			utf32 += 0x010000;

			return 2;
		}

		Uint32 utf32_to_utf8(const Uint32 utf32, Uint8Array4 &utf8)
		{
			if (utf32 <= 0x00007F)
			{
				utf8[0] = utf32;
				utf8[1] = 0x00;
				utf8[2] = 0x00;
				utf8[3] = 0x00;

				return 1;
			}

			if (utf32 <= 0x0007FF)
			{
				utf8[0] = (utf32 >> 6) | 0xC0;
				utf8[1] = (utf32 & 0x3F) | 0x80;
				utf8[2] = 0x00;
				utf8[3] = 0x00;

				return 2;
			}

			if (utf32 <= 0x00FFFF)
			{
				utf8[0] = (utf32 >> 12) | 0xE0;
				utf8[1] = ((utf32 >> 6) & 0x3F) | 0x80;
				utf8[2] = (utf32 & 0x3F) | 0x80;
				utf8[3] = 0x00;

				return 3;
			}

			if (utf32 <= 0x10FFFF)
			{
				utf8[0] = (utf32 >> 18) | 0xF0;
				utf8[1] = ((utf32 >> 12) & 0x3F) | 0x80;
				utf8[2] = ((utf32 >> 6) & 0x3F) | 0x80;
				utf8[3] = (utf32 & 0x3F) | 0x80;

				return 4;
			}

			return 0;
		}

		Uint32 utf32_to_utf16(const Uint32 utf32, Uint16Array2 &utf16)
		{
			Uint32 tmp;

			if (utf32 <= 0x00FFFF)
			{
				utf16[0] = utf32;
				utf16[1] = 0x0000;

				return 1;
			}

			if (utf32 <= 0x10FFFF)
			{
				tmp = utf32 - 0x010000;

				utf16[0] = ((tmp >> 10) & 0x03FF) | 0xD800;
				utf16[1] = (tmp & 0x03FF) | 0xDC00;

				return 2;
			}

			return 0;
		}

	}

#if	0
(WCHAR_MAX <= 0xFFFF)
	StringType utf8_to_string(const std::string &str)
	{
		StringType result;
		Uint32 index, size, len, count, tmp, i;
		Uint16Array2 dst;
		Uint8Array4 src;

		index = 0;
		count = str.size();

		while (index < count)
		{
			len = std::min(count - index, 4u);

			for (i = 0; i < len; i++)
			{
				src[i] = static_cast<Uint8>(str[i + index]);
			}

			size = utf8_to_utf32(src, len, tmp);

			index += std::max(size, 1u);

			if (size > 0)
			{
				size = utf32_to_utf16(tmp, dst);

				for (i = 0; i < size; i++)
				{
					result.push_back(
						static_cast<wchar_t>(dst[i]));
				}
			}
		}

		return result;
	}

	std::string string_to_utf8(const StringType &str)
	{
		std::string result;
		Uint32 index, size, count, tmp, i, len;
		Uint16Array2 src;
		Uint8Array4 dst;

		index = 0;
		count = str.size();

		while (index < count)
		{
			len = std::min(count - index, 2u);

			for (i = 0; i < len; i++)
			{
				src[i] = static_cast<Uint16>(str[i + index]);
			}

			size = utf16_to_utf32(src, len, tmp);

			index += std::max(size, 1u);

			if (size == 0)
			{
				continue;
			}

			size = utf32_to_utf8(tmp, dst);

			for (i = 0; i < size; i++)
			{
				result.push_back(static_cast<char>(dst[i]));
			}
		}

		return result;
	}
#else
	StringType utf8_to_string(const std::string &str)
	{
		StringType result;
		Uint32 index, size, len, count, tmp, i;
		Uint8Array4 src;

		index = 0;
		count = str.size();

		while (index < count)
		{
			len = std::min(count - index, 4u);

			for (i = 0; i < len; i++)
			{
				src[i] = static_cast<Uint8>(str[i + index]);
			}

			size = utf8_to_utf32(src, len, tmp);

			index += std::max(size, 1u);

			if (size > 0)
			{
				result.push_back(static_cast<wchar_t>(tmp));
			}
		}

		return result;
	}

	std::string string_to_utf8(const StringType &str)
	{
		std::string result;
		Uint32 index, size, count, src, i;
		Uint8Array4 tmp;

		index = 0;
		count = str.size();

		while (index < count)
		{
			src = static_cast<Uint32>(str[index]);

			size = utf32_to_utf8(src, tmp);

			index++;

			for (i = 0; i < size; i++)
			{
				result.push_back(static_cast<char>(tmp[i]));
			}
		}

		return result;
	}
#endif

}
