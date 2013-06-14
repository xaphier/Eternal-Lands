/****************************************************************************
 *            zipfile.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "zipfile.hpp"
#include <unzip.h>
#include "readwritememory.hpp"
#include "reader.hpp"
#include "utf.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class ZipFileReader
		{
			private:
				unzFile m_file;

			public:
				ZipFileReader(const unzFile file,
					const unz64_file_pos &position);
				~ZipFileReader() noexcept;
				ReadWriteMemorySharedPtr get_buffer();
				Uint64 get_size();

		};

		ZipFileReader::ZipFileReader(const unzFile file,
			const unz64_file_pos &position): m_file(file)
		{
			unzGoToFilePos64(m_file, &position);
			unzOpenCurrentFile(m_file);
		}

		ZipFileReader::~ZipFileReader() noexcept
		{
			unzCloseCurrentFile(m_file);
		}

		Uint64 ZipFileReader::get_size()
		{
			unz_file_info64 info;

			unzGetCurrentFileInfo64(m_file, &info, 0, 0, 0, 0, 0,
				0);

			return info.uncompressed_size;
		}

		ReadWriteMemorySharedPtr ZipFileReader::get_buffer()
		{
			ReadWriteMemorySharedPtr buffer;

			buffer = boost::make_shared<ReadWriteMemory>(
				get_size());

			unzReadCurrentFile(m_file, buffer->get_ptr(),
				buffer->get_size());

			return buffer;
		}

		voidpf ZCALLBACK open_ifstream_func(voidpf opaque,
			const void* file_name, int mode)
		{
			return new std::ifstream(
				static_cast<const char*>(file_name),
				std::ifstream::binary);
		}

		uLong ZCALLBACK read_istream_func(voidpf opaque,
			voidpf ptr, void* buf, uLong size)
		{
			std::istream* stream;

			assert(ptr != 0);

			stream = static_cast<std::istream*>(ptr);

			stream->read(static_cast<char*>(buf), size);

			if (stream->bad())
			{
				return 0;
			}

			return size;
		}

		uLong ZCALLBACK write_istream_func(voidpf opaque,
			voidpf ptr, const void* buf, uLong size)
		{
			assert(ptr != 0);

			return 0;
		}

		ZPOS64_T ZCALLBACK tell_istream_func(voidpf opaque, voidpf ptr)
		{
			std::istream* stream;

			assert(ptr != 0);

			stream = static_cast<std::istream*>(ptr);

			return stream->tellg();
		}

		long ZCALLBACK seek_istream_func(voidpf opaque, voidpf ptr,
			ZPOS64_T offset, int origin)
		{
			std::istream* stream;

			assert(ptr != 0);

			stream = static_cast<std::istream*>(ptr);

			switch (origin)
			{
				case ZLIB_FILEFUNC_SEEK_CUR:
					stream->seekg(offset,
						std::ios_base::cur);
					break;
				case ZLIB_FILEFUNC_SEEK_END:
					stream->seekg(offset,
						std::ios_base::end);
					break;
				case ZLIB_FILEFUNC_SEEK_SET:
					stream->seekg(offset,
						std::ios_base::beg);
					break;
				default:
					return -1;
			}

			if (stream->bad())
			{
				return -1;
			}

			return 0;
		}

		int ZCALLBACK close_istream_func(voidpf opaque, voidpf ptr)
		{
			std::istream* stream;

			assert(ptr != 0);

			stream = static_cast<std::istream*>(ptr);

			delete stream;

			return 0;
		}

		int ZCALLBACK error_istream_func(voidpf opaque, voidpf ptr)
		{
			std::istream* stream;

			stream = static_cast<std::istream*>(ptr);

			if (stream == 0)
			{
				return -1;
			}

			if (stream->bad())
			{
				return -1;
			}

			return 0;
		}

	}

	class ZipFile::ZipFileEntry
	{
		private:
			unz64_file_pos m_position;

		public:
			inline ZipFileEntry()
			{
			}

			inline ZipFileEntry(const unz64_file_pos &position)
			{
				set_position(position);
			}

			inline ~ZipFileEntry() noexcept
			{
			}

			inline unz64_file_pos &get_position() noexcept
			{
				return m_position;
			}

			inline const unz64_file_pos &get_position() const
				noexcept
			{
				return m_position;
			}

			inline void set_position(const unz64_file_pos &position)
				noexcept
			{
				m_position = position;
			}
	};

	ZipFile::ZipFile(const String &name): AbstractArchive(name)
	{
		init();
	}

	ZipFile::~ZipFile() noexcept
	{
		unzClose(m_file);
	}

	void ZipFile::init()
	{
		zlib_filefunc64_def file_functions;
		unz_file_info64 info;
		unz64_file_pos position;
		boost::scoped_array<char> file_name;
		String index;
		Uint32 size;
		Sint32 ok;

		file_functions.zopen64_file = open_ifstream_func;
		file_functions.zread_file = read_istream_func;
		file_functions.zwrite_file = write_istream_func;
		file_functions.ztell64_file = tell_istream_func;
		file_functions.zseek64_file = seek_istream_func;
		file_functions.zclose_file = close_istream_func;
		file_functions.zerror_file = error_istream_func;

		m_file = unzOpen2_64(utf8_to_string(get_name()).c_str(),
			&file_functions);

		ok = unzGoToFirstFile(m_file);

		while (ok == UNZ_OK)
		{
			unzGetFilePos64(m_file, &position);

			unzGetCurrentFileInfo64(m_file, &info, 0, 0, 0, 0, 0,
				0);

			size = info.size_filename;

			file_name.reset(new char[size + 1]);

			unzGetCurrentFileInfo64(m_file, 0, file_name.get(),
				size, 0, 0, 0, 0);

			index = String(string_to_utf8(std::string(
				file_name.get(), size)));

			m_files[index] = ZipFileEntry(position);

			ok = unzGoToNextFile(m_file);
		}
	}

	ReaderSharedPtr ZipFile::get_file(const String &file_name) const
	{
		ZipFileEntries::const_iterator found;
		ReaderSharedPtr reader;

		found = m_files.find(file_name);

		if (found != m_files.end())
		{
			ZipFileReader zip_reader(m_file,
				found->second.get_position());

			reader = boost::make_shared<Reader>(
				zip_reader.get_buffer(), file_name);

			return reader;
		}

		EL_THROW_EXCEPTION(FileNotFoundException()
			<< boost::errinfo_file_name(file_name));
	}

	bool ZipFile::get_has_file(const String &file_name) const
	{
		return m_files.find(file_name) != m_files.end();
	}

	void ZipFile::get_files(const String &dir, const String &pattern,
		StringSet &files) const
	{
	}

}
