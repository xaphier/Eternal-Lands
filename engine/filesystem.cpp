/****************************************************************************
 *            filesystem.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filesystem.hpp"
#include "exceptions.hpp"
#include "abstractarchive.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "memorybuffer.hpp"
#include "reader.hpp"
#include "zipfile.hpp"
#include "decompressutil.hpp"

#include "../io/elpathwrapper.h"

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
			boost::filesystem::ifstream stream;
			boost::filesystem::path path, gz_path, xz_path;

			path = get_name().get();
			path /= file_name.get();

			gz_path = path;
			gz_path /= L".gz";

			xz_path = path;
			xz_path /= L".xz";

			if (boost::filesystem::exists(gz_path))
			{
				path = gz_path;
			}

			if (boost::filesystem::exists(xz_path))
			{
				path = xz_path;
			}

			size = boost::filesystem::file_size(path);

			stream.open(path);

			buffer = boost::make_shared<MemoryBuffer>(size);

			stream.read(static_cast<char*>(buffer->get_ptr()),
				buffer->get_size());

			reader = boost::make_shared<Reader>(
				DecompresszUtil::decompress(buffer), file_name);

			return reader;
		}

		bool DirArchive::get_has_file(const String &file_name) const
		{
			boost::filesystem::path path, gz_path, xz_path;

			path = get_name().get();
			path /= file_name.get();

			gz_path = get_name().get();
			gz_path /= file_name.get() + L".gz";

			xz_path = get_name().get();
			xz_path /= file_name.get() + L".xz";

			if (boost::filesystem::exists(gz_path))
			{
				return true;
			}

			if (boost::filesystem::exists(xz_path))
			{
				return true;
			}

			return boost::filesystem::exists(path);
		}

	}

	FileSystem::FileSystem()
	{
		m_archives.push_back(new DirArchive(String(L".")));
	}

	FileSystem::~FileSystem() throw()
	{
	}

	void FileSystem::add_dirs(const Uint16 major, const Uint16 minor,
		const Uint16 release)
	{
		StringStream str;
		boost::filesystem::path config_dir, update_dir, custom_dir;

		config_dir = utf8_to_string(get_path_config_base());

		m_archives.push_back(new DirArchive(String(config_dir.wstring())));

		str << major << "_" << minor << "_" << release;

		update_dir = config_dir;
		update_dir /= L"update";
		update_dir /= str.str();

		m_archives.push_back(new DirArchive(String(update_dir.wstring())));

		custom_dir = config_dir;
		custom_dir /= L"custom";

		m_archives.push_back(new DirArchive(String(custom_dir.wstring())));
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

	bool FileSystem::delete_file(const String &file_name)
	{
		return false;
	}

	ReaderSharedPtr FileSystem::get_file(const String &file_name)
	{
		AbstractArchiveVector::const_iterator it, end;

		end = m_archives.end();

		for (it = m_archives.begin(); it != end; ++it)
		{
			if (it->get_has_file(file_name))
			{
				return it->get_file(file_name);
			}
		}

		EL_THROW_EXCEPTION(FileNotFoundException()
			<< boost::errinfo_file_name(string_to_utf8(file_name)));
	}

	String FileSystem::get_file_string(const String &file_name)
	{
		ReaderSharedPtr reader;

		reader = get_file(file_name);

		return reader->read_utf8_string(reader->get_size());
	}

}
