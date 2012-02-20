/****************************************************************************
 *            decompressutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "decompressutil.hpp"
#include "readwritememory.hpp"
#include "exceptions.hpp"
#include "../xz/Xz.h"
#include "../xz/7zCrc.h"
#include "../xz/XzCrc64.h"
#include <zlib.h>

namespace eternal_lands
{

	namespace
	{

		void *SzAlloc(void* /*p*/, size_t size)
		{
			return malloc(size);
		}

		void SzFree(void* /*p*/, void *address)
		{
			free(address);
		}

		ISzAlloc lzmaAlloc = { SzAlloc, SzFree };

		void init_crc_tables()
		{
			CrcGenerateTable();
			Crc64GenerateTable();
		}

		const Uint32 GZ_SIG_SIZE = 2;
		const Uint8 GZ_SIG[2] = { 31, 139 };

	}

	ReadWriteMemorySharedPtr DecompresszUtil::decompress_gz(
		const AbstractReadMemorySharedPtr &buffer)
	{
		ReadWriteMemorySharedPtr result;
		z_stream stream;
		Uint32 err;

		stream.next_in = const_cast<Bytef*>(static_cast<const Bytef*>(
			buffer->get_ptr()));
		stream.avail_in = buffer->get_size();
		stream.total_in = buffer->get_size();

		stream.next_out = 0;
		stream.avail_out = 0;
		stream.total_out = 0;
		stream.zalloc = 0;
		stream.zfree = 0;

		err = inflateInit2(&stream, 32 + MAX_WBITS);

		if (err != Z_OK)
		{
			EL_THROW_EXCEPTION(UncompressException()
				<< errinfo_message(UTF8("gz uncompress "
					"error")));
		}

		result = boost::make_shared<ReadWriteMemory>();

		do
		{
			stream.avail_out += 0x40000;
			stream.total_out += 0x40000;

			result->resize(stream.total_out);

			stream.next_out = static_cast<Bytef*>(
				result->get_ptr());

			err = inflate(&stream, 0);
		}
		while (err == Z_STREAM_END);

		result->resize(stream.total_out - stream.avail_out);

		inflateEnd(&stream);

		if (err != Z_OK)
		{
			EL_THROW_EXCEPTION(UncompressException()
				<< errinfo_message(UTF8("gz uncompress "
					"error")));
		}

		return result;
	}

	ReadWriteMemorySharedPtr DecompresszUtil::decompress_xz(
		const AbstractReadMemorySharedPtr &buffer)
	{
		ReadWriteMemorySharedPtr result;
		CXzUnpacker state;
		Uint64 uncompressed_size, dst_idx, src_idx;
		SizeT dst_size, src_size;
		Uint32 err;
		ECoderStatus status;

		init_crc_tables();

		err = XzUnpacker_Create(&state, &lzmaAlloc);

		if (err != SZ_OK)
		{
			EL_THROW_EXCEPTION(UncompressException()
				<< errinfo_message(UTF8("xz uncompress "
					"error")));
		}

		dst_idx = 0;
		src_idx = 0;

		uncompressed_size = 0;

		result = boost::make_shared<ReadWriteMemory>();

		do
		{
			uncompressed_size += 0x40000;

			result->resize(uncompressed_size);

			dst_size = uncompressed_size - dst_idx;
			src_size = buffer->get_size() - src_idx;

			err = XzUnpacker_Code(&state,
				(Byte*)result->get_ptr() + dst_idx,
				&dst_size, (Byte*)buffer->get_ptr() + src_idx,
				&src_size, CODER_FINISH_ANY, &status);

			src_idx += src_size;
			dst_idx += dst_size;
		}
		while ((err == SZ_OK) && (status == CODER_STATUS_NOT_FINISHED));

		XzUnpacker_Free(&state);

		if (err != SZ_OK)
		{
			EL_THROW_EXCEPTION(UncompressException()
				<< errinfo_message(UTF8("xz uncompress "
					"error")));
		}

		result->resize(dst_idx);

		return result;
	}

	AbstractReadMemorySharedPtr DecompresszUtil::decompress(
		const AbstractReadMemorySharedPtr &buffer)
	{
		if (buffer->get_size() > XZ_SIG_SIZE)
		{
			if (memcmp(buffer->get_ptr(), XZ_SIG, XZ_SIG_SIZE) == 0)
			{
				return decompress_xz(buffer);
			}
		}

		if (buffer->get_size() > GZ_SIG_SIZE)
		{
			if (memcmp(buffer->get_ptr(), GZ_SIG, GZ_SIG_SIZE) == 0)
			{
				return decompress_gz(buffer);
			}
		}

		return buffer;
	}

}
