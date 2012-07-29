/****************************************************************************
 *            filesystem.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filesystem.hpp"
#include "exceptions.hpp"
#include "abstractarchive.hpp"
#include "readwritememory.hpp"
#include "reader.hpp"
#include "zipfile.hpp"
#include "decompressutil.hpp"
#include "sha1/sha1.h"
#include "logging.hpp"
#include "utf.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#ifdef	WINDOWS
#include <io.h>
#else	/* WINDOWS */
#include <ftw.h>
#include <fnmatch.h>
#endif	/* WINDOWS */

namespace eternal_lands
{

	namespace
	{

		inline String get_dir(const String &name)
		{
			if (name.get().empty())
			{
				return name;
			}

#ifdef	WINDOWS
			if (*name.get().rbegin() != UTF8('\\'))
			{
				return String(name.get() + UTF8('\\'));
			}
#else	/* WINDOWS */
			if (*name.get().rbegin() != UTF8('/'))
			{
				return String(name.get() + UTF8( '/'));
			}
#endif	/* WINDOWS */

			return name;
		}

		inline String append_dir(const String &name, const String &str)
		{
			if (name.get().empty())
			{
				return str;
			}

#ifdef	WINDOWS
			if (*name.get().rbegin() != UTF8('\\'))
			{
				return String(name.get() + UTF8('\\') +
					str.get());
			}
#else	/* WINDOWS */
			if (*name.get().rbegin() != UTF8('/'))
			{
				return String(name.get() + UTF8('/') +
					str.get());
			}
#endif	/* WINDOWS */

			return String(name.get() + str.get());
		}

		std::ofstream log_stream;

#ifdef	WINDOWS
		void scan_directory(const String &base_name,
			const String &dir_name, const String &pattern,
			StringSet &files)
		{
			long h_find;
			struct _finddata_t find_data;
			StringType actual_search, path, file_name;

			path = append_dir(base_name, dir_name);

			actual_search = append_dir(String(path), pattern);

			log_stream << "actual_search: " << actual_search << std::endl;

			// Find the first file
			h_find = _findfirst(actual_search.c_str(), &find_data);

			// Look for more
			do
			{
				//If no files are found
				if (h_find == -1L)
				{
					log_stream << "no files are found" << std::endl;

					//Done checking this folder
					break;
				}

				file_name = find_data.name;

				//If the file is a self-reference
				if ((file_name == ".") || (file_name == ".."))
				{
					//Skip to next file
					continue;
				}

				file_name = append_dir(dir_name, 
					String(file_name));

				//If the file is a folder
				if (find_data.attrib & _A_SUBDIR)
				{
					continue;
				}

				log_stream << "file name: " << file_name << std::endl;

				files.insert(String(file_name));
			}
			while (_findnext(h_find, &find_data));

			_findclose(h_find);

			actual_search = append_dir(String(path),
				String(UTF8("*")));

			log_stream << "actual_search dir: " << actual_search << std::endl;

			// Find the first file
			h_find = _findfirst(actual_search.c_str(), &find_data);

			// Look for more dirs
			do
			{
				//If no files are found
				if (h_find == -1L)
				{
					log_stream << "no files are found" << std::endl;

					//Done checking this folder
					break;
				}

				file_name = find_data.name;

				//If the file is a self-reference
				if ((file_name == ".") || (file_name == ".."))
				{
					//Skip to next file
					continue;
				}

				file_name = append_dir(dir_name,
					String(file_name));

				//If the file is a folder
				if (find_data.attrib & _A_SUBDIR)
				{
					log_stream << "dir name: " << file_name << std::endl;

					scan_directory(base_name,
						String(file_name), pattern,
						files);
				}
			}
			while (_findnext(h_find, &find_data));

			_findclose(h_find);
		}
#else	/* WINDOWS */
		void scan_directory(const String &base_name,
			const String &dir_name, const String &pattern,
			StringSet &files)
		{
			DIR* dir;
			struct dirent* entry;
			String current;
			StringType path;

			path = append_dir(base_name, dir_name);

			dir = opendir(path.c_str());

			if (dir == NULL)
			{
				return;
			}

			entry = readdir(dir);

			while (entry != 0)
			{
				current = append_dir(dir_name,
					String(entry->d_name));

				if (entry->d_type == DT_DIR)
				{
					if (strcmp(entry->d_name, ".") &&
						strcmp(entry->d_name, ".."))
					{
						scan_directory(base_name,
							current, pattern,
							files);
					}

					entry = readdir(dir);

					continue;
				}

				if (entry->d_type != DT_REG)
				{
					entry = readdir(dir);

					continue;
				}

				if (!fnmatch(pattern.get().c_str(),
					entry->d_name, 0))
				{
					files.insert(current);
				}

				entry = readdir(dir);
			}

			closedir(dir);
		}
#endif	/* WINDOWS */

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
				virtual ~DirArchive() noexcept;

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
				virtual void get_files(const String &dir,
					const String &pattern, StringSet &files)
					const;

		};

		DirArchive::DirArchive(const String &name):
			AbstractArchive(get_dir(name))
		{
		}

		DirArchive::~DirArchive() noexcept
		{
		}

		ReaderSharedPtr DirArchive::get_file(const String &file_name)
			const
		{
			ReaderSharedPtr reader;
			ReadWriteMemorySharedPtr buffer;
			Uint64 size;
			std::ifstream stream;
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = get_dir(get_name());
			path += file_name;
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

			stream.open(path.c_str(), std::ifstream::binary);

			buffer = boost::make_shared<ReadWriteMemory>(size);

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

			path = get_dir(get_name());
			path += file_name;
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

		void DirArchive::get_files(const String &dir,
			const String &pattern, StringSet &files) const
		{
			log_stream.open("files.log");

			scan_directory(String(utf8_to_string(get_dir(
					get_name()))),
				String(utf8_to_string(get_dir(dir))), pattern,
					files);

			log_stream.close();
		}

		Uint8Array20 get_file_sha1(
			const AbstractReadMemorySharedPtr &buffer)
		{
			Uint8Array20 digest;

			sha1::calc(buffer->get_ptr(), buffer->get_size(),
				digest.c_array());

			return digest;
		}

		Uint8Array20 get_file_sha1(const String &file_name)
		{
			ReadWriteMemorySharedPtr buffer;
			Uint64 size;
			std::ifstream stream;
			StringType path, gz_path, xz_path;
			struct stat fstat;

			path = utf8_to_string(file_name);

			if (stat(path.c_str(), &fstat) != 0)
			{
				EL_THROW_EXCEPTION(FileNotFoundException()
					<< boost::errinfo_file_name(file_name));
			}

			size = fstat.st_size;

			stream.open(path.c_str(), std::ifstream::binary);

			buffer = boost::make_shared<ReadWriteMemory>(size);

			stream.read(static_cast<char*>(buffer->get_ptr()),
				buffer->get_size());

			return get_file_sha1(buffer);
		}

		StringType get_sha1_str(const Uint8Array20 &sha1)
		{
			StringStream str;

			str << std::hex;

			BOOST_FOREACH(const Uint16 value, sha1)
			{
				str << (value >> 4) << (value & 0xF);
			}

			return str.str();
		}

		Uint8Array20 get_sha1(const char sha1[40])
		{
			Uint8Array20 result;
			Uint16 i, value;

			for (i = 0; i < 20; ++i)
			{
				StringStream str;

				str << sha1[i * 2 + 0];
				str << sha1[i * 2 + 1];

				str >> std::hex >> value;

				result[i] = value;
			}

			return result;
		}

	}

	FileSystem::FileSystem()
	{
		add_dir(String(UTF8(".")));
	}

	FileSystem::~FileSystem() noexcept
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

		for (i = 0; i < count; ++i)
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

		for (i = 1; i < count; ++i)
		{
#ifdef	WINDOWS
			result += UTF8('\\');
#else	/* WINDOWS */
			result += UTF8('/');
#endif	/* WINDOWS */
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

		for (i = 1; i < count; ++i)
		{
#ifdef	WINDOWS
			result += UTF8('\\');
#else	/* WINDOWS */
			result += UTF8('/');
#endif	/* WINDOWS */
			result += path[i];
		}

		return String(result);
	}

	void FileSystem::add_dir(const String &dir_name)
	{
		m_archives.push_back(new DirArchive(dir_name));
	}

	void FileSystem::add_zip(const String &zip_name)
	{
		m_archives.push_back(new ZipFile(zip_name));
	}

	void FileSystem::add_zip(const String &zip_name,
		const Uint8Array20 &sha1)
	{
		Uint8Array20 file_sha1;

		file_sha1 = get_file_sha1(zip_name);

		if (sha1 != file_sha1)
		{
			LOG_WARNING(lt_io, UTF8("File '%1%' sha1 should be "
				"'%2%', but is '%3%'"), zip_name %
				get_sha1_str(sha1) % get_sha1_str(file_sha1));
		}

		m_archives.push_back(new ZipFile(zip_name));
	}

	void FileSystem::add_zip(const String &zip_name,
		const char sha1[20])
	{
		Uint8Array20 file_sha1;

		file_sha1 = get_file_sha1(zip_name);

		if (sha1 != get_sha1_str(file_sha1))
		{
			LOG_WARNING(lt_io, UTF8("File '%1%' sha1 should be "
				"'%2%', but is '%3%'"), zip_name % sha1 %
				get_sha1_str(file_sha1));
		}

		m_archives.push_back(new ZipFile(zip_name));
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

	bool FileSystem::get_file_readable(const String &file_name)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		String name;

		name = get_strip_relative_path(file_name);

		end = m_archives.rend();

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			if (it->get_has_file(file_name))
			{
				return true;
			}
		}

		return false;
	}

	bool FileSystem::get_file_if_readable(const String &file_name,
		ReaderSharedPtr &reader)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		String name;

		name = get_strip_relative_path(file_name);

		end = m_archives.rend();

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			try
			{
				if (it->get_has_file(file_name))
				{
					reader = it->get_file(file_name);

					return true;
				}
			}
			catch (boost::exception &exception)
			{
				exception <<
					boost::errinfo_file_name(file_name);
				throw;
			}		
		}

		return false;
	}

	ReaderSharedPtr FileSystem::get_file(const String &file_name)
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		StringUint8Array20Map::const_iterator found;
		ReaderSharedPtr reader;
		Uint8Array20 sha1;
		String name;

		name = String(get_strip_relative_path(file_name));

		end = m_archives.rend();

		LOG_DEBUG(lt_io, UTF8("Checking file '%1%'"), file_name);

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			LOG_DEBUG(lt_io, UTF8("Checking archive '%1%'"),
				it->get_name());

			try
			{
				if (it->get_has_file(name))
				{
					reader = it->get_file(name);

					found = m_sha1s.find(name);

					if (found == m_sha1s.end())
					{
						return reader;
					}

					sha1 = get_file_sha1(
						reader->get_buffer());

					if (sha1 == found->second)
					{
						return reader;
					}

					LOG_WARNING(lt_io, UTF8("File '%1%/%2%'"
						" had sha1 %3% but %4% "
						"expected"), it->get_name() %
						name % get_sha1_str(sha1) %
						get_sha1_str(found->second));
				}
			}
			catch (boost::exception &exception)
			{
				exception <<
					boost::errinfo_file_name(file_name);
				throw;
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

		return str;
	}

	String FileSystem::get_name_without_extension(
		const String &file_name)
	{
		StringTypeVector path;
		StringType result;
		std::size_t pos;
		Uint32 i, index, count;

		path = get_stripped_path(file_name);

		count = path.size();

		if (count == 0)
		{
			return String(UTF8(""));
		}

		index = count - 1;

		pos = path[index].rfind(UTF8("."));

		if (pos != std::string::npos)
		{
			path[index].erase(path[index].begin() + pos,
				path[index].end());
		}

		result = path[0];

		for (i = 1; i < count; ++i)
		{
#ifdef	WINDOWS
			result += UTF8('\\');
#else	/* WINDOWS */
			result += UTF8('/');
#endif	/* WINDOWS */
			result += path[i];
		}

		return String(result);
	}

	String FileSystem::get_file_name_without_extension(
		const String &file_name)
	{
		StringType result;
		std::size_t pos;

		result = get_file_name(file_name);

		pos = result.rfind(UTF8("."));

		if (pos != std::string::npos)
		{
			result.erase(result.begin() + pos, result.end());
		}

		return String(result);
	}

	void FileSystem::clear()
	{
		m_archives.clear();
	}

	StringSet FileSystem::get_files(const String &dir,
		const String &pattern) const
	{
		AbstractArchiveVector::const_reverse_iterator it, end;
		StringSet result;
		String name;

		name = String(get_strip_relative_path(dir));

		end = m_archives.rend();

		LOG_DEBUG(lt_io, UTF8("Checking dir '%1%'"), dir);

		for (it = m_archives.rbegin(); it != end; ++it)
		{
			LOG_DEBUG(lt_io, UTF8("Checking archive '%1%'"),
				it->get_name());

			try
			{
				it->get_files(name, pattern, result);
			}
			catch (boost::exception &exception)
			{
				exception << boost::errinfo_file_name(dir);
				throw;
			}
		}

		return result;
	}

}
