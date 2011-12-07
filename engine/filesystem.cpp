/****************************************************************************
 *            filesystem.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filesystem.hpp"
#include "exceptions.hpp"
#include "abstractarchive.hpp"
#include "memorybuffer.hpp"
#include "reader.hpp"
#include "zipfile.hpp"
#include "decompressutil.hpp"
#include <sys/stat.h>

#include "../io/elpathwrapper.h"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		class DirArchive: public AbstractArchive
		{
			public:
				/**
				 * Default constructor.
				 */
				DirArchive(const String &name);

				/**
				 * Default destructor.
				 */
				virtual ~DirArchive() throw();

				/**
				 * Opens the file for reading.
				 * @return The Reader object of the file.
				 */
				virtual ReaderSharedPtr get_file(
					const String &file_name) const;

				/**
				 * Checks if the file can be found in the
				 * archive.
				 * @return True if the file is in the archive,
				 * else false
				 */
				virtual bool get_has_file(
					const String &file_name) const;

		};

		DirArchive::DirArchive(const String &name):
			AbstractArchive(name)
		{
		}

		DirArchive::~DirArchive() throw()
		{
		}

		ReaderSharedPtr DirArchive::get_file(const String &file_name)
			const
		{
			ReaderSharedPtr reader;
			MemoryBufferSharedPtr buffer;
			Uint64 size;
			std::ifstream stream;
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = get_name().get();
			path += '/';
			path += file_name.get();
			path = utf8_to_string(path);

			gz_path = path;
			gz_path += ".gz";

			xz_path = path;
			xz_path += ".xz";

			if (stat(gz_path.c_str(), &fstat) == 0)
			{
				path = gz_path;
			}

			if (stat(xz_path.c_str(), &fstat) == 0)
			{
				path = xz_path;
			}

			if (stat(path.c_str(), &fstat) != 0)
			{
				EL_THROW_EXCEPTION(FileNotFoundException()
					<< boost::errinfo_file_name(file_name));
			}

			size = fstat.st_size;

			stream.open(path.c_str());

			buffer = boost::make_shared<MemoryBuffer>(size);

			stream.read(static_cast<char*>(buffer->get_ptr()),
				buffer->get_size());

			reader = boost::make_shared<Reader>(
				DecompresszUtil::decompress(buffer), file_name);

			return reader;
		}

		bool DirArchive::get_has_file(const String &file_name) const
		{
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = get_name().get();
			path += '/';
			path += file_name.get();
			path = utf8_to_string(path);

			gz_path = path;
			gz_path += ".gz";

			xz_path = path;
			xz_path += ".xz";

			if (stat(gz_path.c_str(), &fstat) == 0)
			{
				return true;
			}

			if (stat(xz_path.c_str(), &fstat) == 0)
			{
				return true;
			}

			return stat(path.c_str(), &fstat) == 0;
		}

	}

	FileSystem::FileSystem()
	{
		m_archives.push_back(new DirArchive(String(".")));
	}

	FileSystem::~FileSystem() throw()
	{
	}

	String FileSystem::get_file_name(const String &file_name)
	{
		StringTypeVector splits;

		boost::split(splits, file_name.get(), boost::is_any_of(
			UTF8("\\/")), boost::token_compress_on);

		if (splits.size() > 0)
		{
			return String(*splits.rbegin());
		}

		return String(UTF8(""));
	}

	StringTypeVector FileSystem::get_stripped_path(const String &file_name)
	{
		StringTypeVector splits, path;
		Uint32 i, count;

		boost::split(splits, file_name.get(), boost::is_any_of(
			UTF8("\\/")), boost::token_compress_on);

		count = splits.size();

		for (i = 0; i < count; i++)
		{
			if (splits[i] == "..")
			{
				if (path.size() > 0)
				{
					path.pop_back();
				}
				continue;
			}

			if (splits[i] == ".")
			{
				continue;
			}

			path.push_back(splits[i]);
		}

		return path;
	}

	String FileSystem::get_strip_relative_path(const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		Uint32 i, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count == 0)
		{
			return String(UTF8(""));
		}

		result = path[0];

		for (i = 1; i < count; i++)
		{
			result += '/';
			result += path[i];
		}

		return String(result);
	}

	String FileSystem::get_dir_name(const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		Uint32 i, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count < 2)
		{
			return String(UTF8(""));
		}

		count -= 1;
		result = path[0];

		for (i = 1; i < count; i++)
		{
			result += '/';
			result += path[i];
		}

		return String(result);
	}

	void FileSystem::add_dirs(const Uint16 major, const Uint16 minor,
		const Uint16 release)
	{
		StringStream str;
		StringType config_dir, update_dir, custom_dir;

		config_dir = string_to_utf8(get_path_config_base());

		m_archives.push_back(new DirArchive(String(config_dir)));

		str << major << "_" << minor << "_" << release;

		update_dir = config_dir;
		update_dir += "/update";
		update_dir += str.str();

		m_archives.push_back(new DirArchive(String(update_dir)));

		custom_dir = config_dir;
		custom_dir += "/custom";

		m_archives.push_back(new DirArchive(String(custom_dir)));
	}

	void FileSystem::add_dir(const String &dir_name)
	{
		m_archives.push_back(new DirArchive(dir_name));
	}

	void FileSystem::add_zip(const String &dir_name)
	{
		m_archives.push_back(new ZipFile(dir_name));
	}

	bool FileSystem::remove_archive(const String &name)
	{
		AbstractArchiveVector::iterator it, end;

		end = m_archives.end();

		for (it = m_archives.begin(); it != end; ++it)
		{
			if (it->get_name() == name)
			{
				m_archives.release(it);

				return true;
			}
		}

		return false;
	}

	ReaderSharedPtr FileSystem::get_file(const String &file_name)
	{
		AbstractArchiveVector::const_iterator it, end;
		String name;

		name = get_strip_relative_path(file_name);

		end = m_archives.end();

		for (it = m_archives.begin(); it != end; ++it)
		{
			if (it->get_has_file(file_name))
			{
				return it->get_file(file_name);
			}
		}

		EL_THROW_EXCEPTION(FileNotFoundException()
			<< boost::errinfo_file_name(file_name));
	}

	String FileSystem::get_file_string(const String &file_name)
	{
		ReaderSharedPtr reader;
		String str;

		reader = get_file(file_name);

		str = reader->read_utf8_string(reader->get_size());

		LOG_INFO(UTF8("File '%1%' has size %2% and string: '%3%'"),
			file_name % reader->get_size() % str);

		return str;
	}

}
