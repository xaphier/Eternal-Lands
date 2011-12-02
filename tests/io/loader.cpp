/****************************************************************************
 *            main.cpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "loader.hpp"
#include "file/file.hpp"
#include <boost/exception/diagnostic_information.hpp>

namespace eternal_lands
{

	namespace
	{

		template <typename T>
		void check_loader_read(const LoaderSharedPtr &loader, const T value)
		{
			T tmp;

			tmp = loader->read_le<T>();

			if (tmp != value)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(loader->get_file_name()));
			}

			tmp = loader->read_be<T>();

			if (tmp != value)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(loader->get_file_name()));
			}
		}

		void check_loader_read_string(const LoaderSharedPtr &loader,
			const String &value)
		{
			String tmp;

			tmp = loader->read_string();

			if (tmp != value)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(loader->get_file_name()));
			}
		}

		void check_loader_read_string(const LoaderSharedPtr &loader,
			const String &value, const Uint32 size)
		{
			String tmp;

			tmp = loader->read_string(size);

			if (tmp != value)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(loader->get_file_name()));
			}
		}

		template <typename T>
		void check_loader_read_dynamic(const LoaderSharedPtr &loader, const T value)
		{
			T tmp;

			tmp = loader->read_dynamic<T>();

			if (tmp != value)
			{
				EL_THROW_EXCEPTION(IoErrorException()
					<< boost::errinfo_file_name(loader->get_file_name()));
			}
		}

		void check_loader(const String &dir)
		{
			LoaderSharedPtr loader;
			AbstractFileSharedPtr file;
			String file_name;

			file_name = AbstractFile::combine(dir, "load_save.dat");
			file = boost::make_shared<File>(file_name);
			loader = boost::make_shared<Loader>(file);

			check_loader_read<Uint8>(loader, 14);
			check_loader_read<Sint8>(loader, -37);
			check_loader_read<Uint8>(loader, 200);
			check_loader_read<Sint8>(loader, 4);
			check_loader_read<Uint16>(loader, 0x0E07);
			check_loader_read<Uint16>(loader, 0x3912);
			check_loader_read<Uint16>(loader, 0xFF00);
			check_loader_read<Uint16>(loader, 0x070E);
			check_loader_read<Uint16>(loader, 0x1239);
			check_loader_read<Uint16>(loader, 0x00FF);
			check_loader_read<Sint16>(loader, 0x0E07);
			check_loader_read<Sint16>(loader, 0x3912);
			check_loader_read<Sint16>(loader, -4096);
			check_loader_read<Sint16>(loader, -1657);
			check_loader_read<Sint16>(loader, -32234);

			check_loader_read<Uint32>(loader, 0x0E075354);
			check_loader_read<Uint32>(loader, 0x54776331);
			check_loader_read<Sint32>(loader, -1242353657);
			check_loader_read<Sint32>(loader, 1242353657);
			check_loader_read<Uint64>(loader, 0x2354350657E07E4A);
			check_loader_read<Uint64>(loader, 0xFEA42354350657E0);
			check_loader_read<Sint64>(loader, 12353458779780223);
			check_loader_read<Sint64>(loader, -12353458779780223);

			check_loader_read<float>(loader, 0.0f);
			check_loader_read<float>(loader, 110.0f);
			check_loader_read<float>(loader, 2343.0e3f);
			check_loader_read<float>(loader, 0.1f);

			check_loader_read<float>(loader, -0.0f);
			check_loader_read<float>(loader, -110.0f);
			check_loader_read<float>(loader, -1.0e-7f);
			check_loader_read<float>(loader, -0.1f);

			check_loader_read<double>(loader, 0.0);
			check_loader_read<double>(loader, 110.0);
			check_loader_read<double>(loader, 2343.0e3);
			check_loader_read<double>(loader, 0.1);

			check_loader_read<double>(loader, -0.0);
			check_loader_read<double>(loader, -110.0);
			check_loader_read<double>(loader, -1.0e-7);
			check_loader_read<double>(loader, -0.1);

			check_loader_read_string(loader, "Test");
			check_loader_read_string(loader, "rfg39fst23!effv34");
			check_loader_read_string(loader, "-4t939sdhfi4 rgtruz56");

			check_loader_read_string(loader, "Test", 32);
			check_loader_read_string(loader, "rfg39fst23!effv34", 64);
			check_loader_read_string(loader, "-4t939sdhfi4 rgtruz56", 128);

			check_loader_read_dynamic<Uint16>(loader, 114);
			check_loader_read_dynamic<Uint16>(loader, 1423);
			check_loader_read_dynamic<Uint16>(loader, 42376);

			check_loader_read_dynamic<Uint32>(loader, 114);
			check_loader_read_dynamic<Uint32>(loader, 1423);
			check_loader_read_dynamic<Uint32>(loader, 42376);
			check_loader_read_dynamic<Uint32>(loader, 2542376);
			check_loader_read_dynamic<Uint32>(loader, 3767542376);
		}

	}

}

int main(int argc, char *argv[])
{
	try
	{
		if (argc < 2)
		{
			eternal_lands::check_loader("");
		}
		else
		{
			eternal_lands::check_loader(argv[1]);
		}

		return 0;
	}
	catch (const boost::exception &exception)
	{
		std::cerr << boost::diagnostic_information(exception) << std::endl;
		throw;
	}
}
