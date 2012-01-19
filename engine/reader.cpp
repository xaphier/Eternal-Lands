/****************************************************************************
 *            reader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "reader.hpp"
#include "abstractreadmemory.hpp"
#include "readwritememory.hpp"
#include "exceptions.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	Reader::Reader(const AbstractReadMemorySharedPtr &buffer,
		const String &name): m_buffer(buffer), m_name(name),
		m_position(0)
	{
	}

	Reader::Reader(const Reader &reader): m_buffer(reader.m_buffer),
		m_name(reader.m_name), m_position(0)
	{
	}

	Reader::~Reader() throw()
	{
	}

	void Reader::read(void* buffer, const Uint64 size)
	{
		assert(check_size(size));

		memcpy(buffer, static_cast<const Uint8* const>(
			m_buffer->get_ptr()) + get_position(), size);

		skip(size);
	}

	void Reader::read(ReadWriteMemory &buffer)
	{
		Uint64 size;

		size = get_bytes_left();

		buffer.resize(size);

		memcpy(buffer.get_ptr(), static_cast<const Uint8* const>(
			m_buffer->get_ptr()) + get_position(), size);

		skip(size);
	}

	void Reader::read_le(ReadWriteMemory &buffer, const Uint16 swap_size)
	{
		Uint64 size, i;

		size = get_bytes_left();

		if ((size % swap_size) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("swap_size"))
				<< errinfo_value(swap_size)
				<< boost::errinfo_file_name(get_name()));
		}

		buffer.resize(size);

		switch (swap_size)
		{
			case 1:
				read(buffer);
				break;
			case 2:
				for (i = 0; i < (size / 2); ++i)
				{
					static_cast<Uint16*>(buffer.get_ptr())[i]
						= read_u16_le();
				}
				break;
			case 4:
				for (i = 0; i < (size / 4); ++i)
				{
					static_cast<Uint32*>(buffer.get_ptr())[i]
						= read_u32_le();
				}
				break;
			case 8:
				for (i = 0; i < (size / 8); ++i)
				{
					static_cast<Uint64*>(buffer.get_ptr())[i]
						= read_u64_le();
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

	void Reader::read_be(ReadWriteMemory &buffer, const Uint16 swap_size)
	{
		Uint64 size, i;

		size = get_bytes_left();

		if ((size % swap_size) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("swap_size"))
				<< errinfo_value(swap_size)
				<< boost::errinfo_file_name(get_name()));
		}

		buffer.resize(size);

		switch (swap_size)
		{
			case 1:
				read(buffer);
				break;
			case 2:
				for (i = 0; i < (size / 2); ++i)
				{
					static_cast<Uint16*>(buffer.get_ptr())[i]
						= read_u16_be();
				}
				break;
			case 4:
				for (i = 0; i < (size / 4); ++i)
				{
					static_cast<Uint32*>(buffer.get_ptr())[i]
						= read_u32_be();
				}
				break;
			case 8:
				for (i = 0; i < (size / 8); ++i)
				{
					static_cast<Uint64*>(buffer.get_ptr())[i]
						= read_u64_be();
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

	String Reader::read_utf8_string(const Uint32 size)
	{
		std::string result;
		size_t len;

		assert(check_size(size));

		result = std::string(static_cast<const char* const>(
			m_buffer->get_ptr()) + get_position(), size);

		len = result.find('\0');

		if (len != std::string::npos)
		{
			result.resize(len);
		}

		skip(size);

		return String(result);
	}

	Uint64 Reader::get_size() const
	{
		return m_buffer->get_size();
	}

	Uint8 Reader::read_u8()
	{
		Uint8 result;

		read(&result, sizeof(result));

		return result;
	}

	Sint8 Reader::read_s8()
	{
		Sint8 result;

		read(&result, sizeof(result));

		return result;
	}

	Uint16 Reader::read_u16_le()
	{
		Uint16 result;

		read(&result, sizeof(result));

		return SDL_SwapLE16(result);
	}

	Uint16 Reader::read_u16_be()
	{
		Uint16 result;

		read(&result, sizeof(result));

		return SDL_SwapBE16(result);
	}

	Sint16 Reader::read_s16_le()
	{
		Sint16 result;

		read(&result, sizeof(result));

		return SDL_SwapLE16(result);
	}

	Sint16 Reader::read_s16_be()
	{
		Sint16 result;

		read(&result, sizeof(result));

		return SDL_SwapBE16(result);
	}

	Uint32 Reader::read_u32_le()
	{
		Uint32 result;

		read(&result, sizeof(result));

		return SDL_SwapLE32(result);
	}

	Uint32 Reader::read_u32_be()
	{
		Uint32 result;

		read(&result, sizeof(result));

		return SDL_SwapBE32(result);
	}

	Sint32 Reader::read_s32_le()
	{
		Sint32 result;

		read(&result, sizeof(result));

		return SDL_SwapLE32(result);
	}

	Sint32 Reader::read_s32_be()
	{
		Sint32 result;

		read(&result, sizeof(result));

		return SDL_SwapBE32(result);
	}

	Uint64 Reader::read_u64_le()
	{
		Uint64 result;

		read(&result, sizeof(result));

		return SDL_SwapLE64(result);
	}

	Uint64 Reader::read_u64_be()
	{
		Uint64 result;

		read(&result, sizeof(result));

		return SDL_SwapBE64(result);
	}

	Sint64 Reader::read_s64_le()
	{
		Sint64 result;

		read(&result, sizeof(result));

		return SDL_SwapLE64(result);
	}

	Sint64 Reader::read_s64_be()
	{
		Sint64 result;

		read(&result, sizeof(result));

		return SDL_SwapBE64(result);
	}

	float Reader::read_float_le()
	{
		union
		{
			float f;
			Uint32 i;
		} result;

		read(&result.f, sizeof(result.f));

		result.i = SDL_SwapLE32(result.i);

		return result.f;
	}

	float Reader::read_float_be()
	{
		union
		{
			float f;
			Uint32 i;
		} result;

		read(&result.f, sizeof(result.f));

		result.i = SDL_SwapBE32(result.i);

		return result.f;
	}

}
