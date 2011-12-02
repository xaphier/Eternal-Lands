/****************************************************************************
 *            zipfile.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "zipfile.hpp"
#include "../io/unzip.h"
#include <boost/filesystem/fstream.hpp>
#include "memorybuffer.hpp"
#include "reader.hpp"

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
				~ZipFileReader() throw();
				MemoryBufferSharedPtr get_buffer();
				Uint64 get_size();

		};

		ZipFileReader::ZipFileReader(const unzFile file,
			const unz64_file_pos &position): m_file(file)
		{
			unzGoToFilePos64(m_file, &position);
			unzOpenCurrentFile(m_file);
		}

		ZipFileReader::~ZipFileReader() throw()
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

		MemoryBufferSharedPtr ZipFileReader::get_buffer()
		{
			MemoryBufferSharedPtr buffer;

			buffer = boost::make_shared<MemoryBuffer>(get_size());

			unzReadCurrentFile(m_file, buffer->get_ptr(),
				buffer->get_size());

			return buffer;
		}

		voidpf ZCALLBACK open_ifstream_func(voidpf opaque,
			const wchar_t* file_name, int mode)
		{
			return new boost::filesystem::fstream(file_name);
		}

		uLong ZCALLBACK read_istream_func(voidpf opaque,
			std::istream* stream, void* buf, uLong size)
		{
			assert(stream != 0);

			stream->read(static_cast<char*>(buf), size);

			if (stream->bad())
			{
				return 0;
			}

			return size;
		}

		uLong ZCALLBACK write_istream_func(voidpf opaque,
			voidpf stream, const void* buf, uLong size)
		{
			assert(stream != 0);

			return 0;
		}

		long ZCALLBACK tell_istream_func(voidpf opaque,
			std::istream* stream)
		{
			assert(stream != 0);

			return stream->tellg();
		}

		long ZCALLBACK seek_istream_func(voidpf opaque,
			std::istream* stream, ZPOS64_T offset, int origin)
		{
			assert(stream != 0);

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

			return stream->tellg();
		}

		int ZCALLBACK close_istream_func(voidpf opaque,
			std::istream* stream)
		{
			assert(stream != 0);

			delete stream;

			return 0;
		}

		int ZCALLBACK error_istream_func(voidpf opaque,
			std::istream* stream)
		{
			if (stream != 0)
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

	class ZipFile::ZipFileEntry: public unz64_file_pos
	{
	};

	ZipFile::ZipFile(const String &name): AbstractArchive(name)
	{
		
	}

	ZipFile::~ZipFile() throw()
	{
		unzClose(m_file);
	}

	ReaderSharedPtr ZipFile::get_file(const String &file_name) const
	{
		ZipFileEntries::const_iterator found;
		ReaderSharedPtr reader;

		found = m_files.find(file_name);

		if (found == m_files.end())
		{
			ZipFileReader zip_reader(m_file, found->second);

			reader = boost::make_shared<Reader>(
				zip_reader.get_buffer(), file_name);

			return reader;
		}

		return reader;
	}

	bool ZipFile::get_has_file(const String &file_name) const
	{
		return m_files.find(file_name) != m_files.end();
	}

}
