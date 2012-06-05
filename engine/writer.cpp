/****************************************************************************
 *            writer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "writer.hpp"
#include "abstractreadmemory.hpp"
#include "readwritememory.hpp"
#include "exceptions.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	Writer::Writer(const OutStreamSharedPtr &stream, const String &name):
		m_stream(stream), m_name(name)
	{
	}

	Writer::Writer(const String &name): m_name(name)
	{
		m_stream = boost::make_shared<std::fstream>(name.get().c_str(),
			std::ios::binary | std::ios::out | std::ios::trunc);
	}

	Writer::~Writer() noexcept
	{
	}

	void Writer::write(const AbstractReadMemory &buffer)
	{
		write(buffer.get_ptr(), buffer.get_size());
	}

	void Writer::write_le(const AbstractReadMemory &buffer,
		const Uint16 swap_size)
	{
		Uint64 size, i;

		size = buffer.get_size();

		if ((size % swap_size) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("swap_size"))
				<< errinfo_value(swap_size)
				<< boost::errinfo_file_name(get_name()));
		}

		switch (swap_size)
		{
			case 1:
				write(buffer);
				break;
			case 2:
				for (i = 0; i < (size / 2); ++i)
				{
					write_u16_le(static_cast<const Uint16*>(
						buffer.get_ptr())[i]);
				}
				break;
			case 4:
				for (i = 0; i < (size / 4); ++i)
				{
					write_u32_le(static_cast<const Uint32*>(
						buffer.get_ptr())[i]);
				}
				break;
			case 8:
				for (i = 0; i < (size / 8); ++i)
				{
					write_u64_le(static_cast<const Uint64*>(
						buffer.get_ptr())[i]);
				}
				break;
			default:
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(UTF8(
						"swap_size"))
					<< errinfo_value(swap_size)
					<< boost::errinfo_file_name(
						get_name()));
		}
	}

	void Writer::write_be(const AbstractReadMemory &buffer,
		const Uint16 swap_size)
	{
		Uint64 size, i;

		size = buffer.get_size();

		if ((size % swap_size) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("swap_size"))
				<< errinfo_value(swap_size)
				<< boost::errinfo_file_name(get_name()));
		}

		switch (swap_size)
		{
			case 1:
				write(buffer);
				break;
			case 2:
				for (i = 0; i < (size / 2); ++i)
				{
					write_u16_be(static_cast<const Uint16*>(
						buffer.get_ptr())[i]);
				}
				break;
			case 4:
				for (i = 0; i < (size / 4); ++i)
				{
					write_u32_be(static_cast<const Uint32*>(
						buffer.get_ptr())[i]);
				}
				break;
			case 8:
				for (i = 0; i < (size / 8); ++i)
				{
					write_u64_be(static_cast<const Uint64*>(
						buffer.get_ptr())[i]);
				}
				break;
			default:
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_parameter_name(UTF8(
						"swap_size"))
					<< errinfo_value(swap_size)
					<< boost::errinfo_file_name(
						get_name()));
		}
	}

	void Writer::write_utf8_string(const String &str, const Uint32 size)
	{
		Uint32 i, count;

		count = str.get().length();
		count = std::min(count, size);

		for (i = 0; i < count; ++i)
		{
			write_s8(str.get()[i]);
		}

		for (i = count; i < size; ++i)
		{
			write_s8(0);
		}
	}

	void Writer::write_u8(const Uint8 value)
	{
		write(&value, sizeof(value));
	}

	void Writer::write_s8(const Sint8 value)
	{
		write(&value, sizeof(value));
	}

	void Writer::write_u16_le(const Uint16 value)
	{
		Uint16 tmp;

		tmp = SDL_SwapLE16(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s16_le(const Sint16 value)
	{
		Sint16 tmp;

		tmp = SDL_SwapLE16(value);

		write(&tmp, sizeof(value));
	}
	void Writer::write_u16_be(const Uint16 value)
	{
		Uint16 tmp;

		tmp = SDL_SwapBE16(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s16_be(const Sint16 value)
	{
		Sint16 tmp;

		tmp = SDL_SwapBE16(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_u32_le(const Uint32 value)
	{
		Uint32 tmp;

		tmp = SDL_SwapLE32(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s32_le(const Sint32 value)
	{
		Sint32 tmp;

		tmp = SDL_SwapLE32(value);

		write(&tmp, sizeof(value));
	}
	void Writer::write_u32_be(const Uint32 value)
	{
		Uint32 tmp;

		tmp = SDL_SwapBE32(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s32_be(const Sint32 value)
	{
		Sint32 tmp;

		tmp = SDL_SwapBE32(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_u64_le(const Uint64 value)
	{
		Uint64 tmp;

		tmp = SDL_SwapLE64(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s64_le(const Sint64 value)
	{
		Sint64 tmp;

		tmp = SDL_SwapLE64(value);

		write(&tmp, sizeof(value));
	}
	void Writer::write_u64_be(const Uint64 value)
	{
		Uint64 tmp;

		tmp = SDL_SwapBE64(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_s64_be(const Sint64 value)
	{
		Sint64 tmp;

		tmp = SDL_SwapBE64(value);

		write(&tmp, sizeof(value));
	}

	void Writer::write_float_le(const float value)
	{
		union
		{
			float f;
			Uint32 i;
		} tmp;

		tmp.f = value;
		tmp.i = SDL_SwapLE32(tmp.i);

		write(&tmp.i, sizeof(value));
	}

	void Writer::write_float_be(const float value)
	{
		union
		{
			float f;
			Uint32 i;
		} tmp;

		tmp.f = value;
		tmp.i = SDL_SwapBE32(tmp.i);

		write(&tmp.i, sizeof(value));
	}

}
