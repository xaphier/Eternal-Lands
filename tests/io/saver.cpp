/****************************************************************************
 *            main.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "saver.hpp"
#include "file/file.hpp"
#include "memorybuffer.hpp"
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace eternal_lands
{

	namespace
	{

		template <typename T>
		void check_saver_write(const SaverSharedPtr &saver, const T value,
			const MemoryBuffer &buffer)
		{
			Uint32 position, size;

			position = saver->get_position();
			saver->write_le(value);
			size = saver->get_position() - position;

			if ((position + size) > buffer.get_size())
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(T).name()));
			}

			if (memcmp(buffer.get_ptr(position), saver->get_data()->get_ptr(position),
				size) != 0)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name()));
			}

			position = saver->get_position();
			saver->write_be(value);
			size = saver->get_position() - position;

			if ((position + size) > buffer.get_size())
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(T).name()));
			}

			if (memcmp(buffer.get_ptr(position), saver->get_data()->get_ptr(position),
				size) != 0)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(T).name()));
			}
		}

		void check_saver_write_string(const SaverSharedPtr &saver,
			const String &value, const MemoryBuffer &buffer)
		{
			Uint32 position, size;

			position = saver->get_position();
			saver->write_string(value);
			size = saver->get_position() - position;

			if ((position + size) > buffer.get_size())
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(String).name()));
			}

			if (memcmp(buffer.get_ptr(position), saver->get_data()->get_ptr(position),
				size) != 0)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(String).name()));
			}
		}

		void check_saver_write_string(const SaverSharedPtr &saver,
			const String &value, const Uint32 count, const MemoryBuffer &buffer)
		{
			Uint32 position, size;

			position = saver->get_position();
			saver->write_string(value, count);
			size = saver->get_position() - position;

			if ((position + size) > buffer.get_size())
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(String).name()));
			}

			if (memcmp(buffer.get_ptr(position), saver->get_data()->get_ptr(position),
				size) != 0)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(String).name()));
			}
		}

		template <typename T>
		void check_saver_write_dynamic(const SaverSharedPtr &saver, const T value,
			const MemoryBuffer &buffer)
		{
			Uint32 position, size;

			position = saver->get_position();
			saver->write_dynamic(value);
			size = saver->get_position() - position;

			if ((position + size) > buffer.get_size())
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(T).name()));
			}

			if (memcmp(buffer.get_ptr(position), saver->get_data()->get_ptr(position),
				size) != 0)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(saver->get_file_name())
					<< boost::errinfo_type_info_name(typeid(T).name()));
			}
		}

		void check_saver(const SaverSharedPtr &saver, const MemoryBuffer &buffer)
		{
			check_saver_write<Uint8>(saver, 14, buffer);
			check_saver_write<Sint8>(saver, -37, buffer);
			check_saver_write<Uint8>(saver, 200, buffer);
			check_saver_write<Sint8>(saver, 4, buffer);
			check_saver_write<Uint16>(saver, 0x0E07, buffer);
			check_saver_write<Uint16>(saver, 0x3912, buffer);
			check_saver_write<Uint16>(saver, 0xFF00, buffer);
			check_saver_write<Uint16>(saver, 0x070E, buffer);
			check_saver_write<Uint16>(saver, 0x1239, buffer);
			check_saver_write<Uint16>(saver, 0x00FF, buffer);
			check_saver_write<Sint16>(saver, 0x0E07, buffer);
			check_saver_write<Sint16>(saver, 0x3912, buffer);
			check_saver_write<Sint16>(saver, -4096, buffer);
			check_saver_write<Sint16>(saver, -1657, buffer);
			check_saver_write<Sint16>(saver, -32234, buffer);

			check_saver_write<Uint32>(saver, 0x0E075354, buffer);
			check_saver_write<Uint32>(saver, 0x54776331, buffer);
			check_saver_write<Sint32>(saver, -1242353657, buffer);
			check_saver_write<Sint32>(saver, 1242353657, buffer);
			check_saver_write<Uint64>(saver, 0x2354350657E07E4A, buffer);
			check_saver_write<Uint64>(saver, 0xFEA42354350657E0, buffer);
			check_saver_write<Sint64>(saver, 12353458779780223, buffer);
			check_saver_write<Sint64>(saver, -12353458779780223, buffer);

			check_saver_write<float>(saver, 0.0f, buffer);
			check_saver_write<float>(saver, 110.0f, buffer);
			check_saver_write<float>(saver, 2343.0e3f, buffer);
			check_saver_write<float>(saver, 0.1f, buffer);

			check_saver_write<float>(saver, -0.0f, buffer);
			check_saver_write<float>(saver, -110.0f, buffer);
			check_saver_write<float>(saver, -1.0e-7f, buffer);
			check_saver_write<float>(saver, -0.1f, buffer);

			check_saver_write<double>(saver, 0.0, buffer);
			check_saver_write<double>(saver, 110.0, buffer);
			check_saver_write<double>(saver, 2343.0e3, buffer);
			check_saver_write<double>(saver, 0.1, buffer);

			check_saver_write<double>(saver, -0.0, buffer);
			check_saver_write<double>(saver, -110.0, buffer);
			check_saver_write<double>(saver, -1.0e-7, buffer);
			check_saver_write<double>(saver, -0.1, buffer);

			check_saver_write_string(saver, "Test", buffer);
			check_saver_write_string(saver, "rfg39fst23!effv34", buffer);
			check_saver_write_string(saver, "-4t939sdhfi4 rgtruz56", buffer);

			check_saver_write_string(saver, "Test", 32, buffer);
			check_saver_write_string(saver, "rfg39fst23!effv34", 64, buffer);
			check_saver_write_string(saver, "-4t939sdhfi4 rgtruz56", 128, buffer);

			check_saver_write_dynamic<Uint16>(saver, 114, buffer);
			check_saver_write_dynamic<Uint16>(saver, 1423, buffer);
			check_saver_write_dynamic<Uint16>(saver, 42376, buffer);

			check_saver_write_dynamic<Uint32>(saver, 114, buffer);
			check_saver_write_dynamic<Uint32>(saver, 1423, buffer);
			check_saver_write_dynamic<Uint32>(saver, 42376, buffer);
			check_saver_write_dynamic<Uint32>(saver, 2542376, buffer);
			check_saver_write_dynamic<Uint32>(saver, 3767542376, buffer);
		}

		void load_buffer(const String &dir, MemoryBuffer &buffer)
		{
			AbstractFileSharedPtr file;
			String file_name;

			file_name = AbstractFile::combine(dir, "load_save.dat");
			file = boost::make_shared<File>(file_name);

			buffer.resize(file->get_size());

			file->read(buffer);
		}

	}
}

int main(int argc, char *argv[])
{
	try
	{
		eternal_lands::SaverSharedPtr saver;
		eternal_lands::MemoryBuffer buffer;

		if (argc < 2)
		{
			eternal_lands::load_buffer("", buffer);
		}
		else
		{
			eternal_lands::load_buffer(argv[1], buffer);
		}

		saver = boost::make_shared<eternal_lands::Saver>();

		eternal_lands::check_saver(saver, buffer);

		saver = boost::make_shared<eternal_lands::Saver>("test.dat");

		eternal_lands::check_saver(saver, buffer);

		return 0;
	}
	catch (const boost::exception &exception)
	{
		std::cerr << boost::diagnostic_information(exception) << std::endl;
		throw;
	}
}
