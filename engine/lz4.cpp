/****************************************************************************
 *            lz4.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lz4.hpp"
#include "readwritememory.hpp"
#include "exceptions.hpp"
#include "lz4/lz4.h"
#include "lz4/lz4hc.h"

namespace eternal_lands
{

	void Lz4::decompress(const AbstractReadMemorySharedPtr &in_buffer,
		const AbstractWriteMemorySharedPtr &out_buffer)
	{
		Sint32 size;

		size = LZ4_compress(
			static_cast<const char*>(in_buffer->get_ptr()),
			static_cast<char*>(out_buffer->get_ptr()),
			out_buffer->get_size());

		if (size < 0)
		{
			EL_THROW_EXCEPTION(DecompressException()
				<< errinfo_message(UTF8("lz4 decompression "
					"error"))
				<< errinfo_parameter_name(UTF8("lz4")));
		}
	}

	ReadWriteMemorySharedPtr Lz4::decompress(
		const AbstractReadMemorySharedPtr &buffer,
		const Uint32 uncompressed_size)
	{
		ReadWriteMemorySharedPtr result;

		result = boost::make_shared<ReadWriteMemory>(uncompressed_size);

		decompress(buffer, result);

		return result;
	}

	ReadWriteMemorySharedPtr Lz4::compress(
		const AbstractReadMemorySharedPtr &buffer)
	{
		ReadWriteMemorySharedPtr result;
		Sint32 size;

		size = LZ4_compressBound(buffer->get_size());

		result = boost::make_shared<ReadWriteMemory>(size);

		size = LZ4_compress(static_cast<const char*>(buffer->get_ptr()),
			static_cast<char*>(result->get_ptr()),
			buffer->get_size());

		if (size < 0)
		{
			EL_THROW_EXCEPTION(CompressException()
				<< errinfo_message(UTF8("lz4 compression "
					"error"))
				<< errinfo_parameter_name(UTF8("lz4")));
		}

		result->resize(size);

		return result;
	}

	ReadWriteMemorySharedPtr Lz4::compress_high(
		const AbstractReadMemorySharedPtr &buffer)
	{
		ReadWriteMemorySharedPtr result;
		Sint32 size;

		size = LZ4_compressBound(buffer->get_size());

		result = boost::make_shared<ReadWriteMemory>(size);

		size = LZ4_compressHC(
			static_cast<const char*>(buffer->get_ptr()),
			static_cast<char*>(result->get_ptr()),
			buffer->get_size());

		if (size < 0)
		{
			EL_THROW_EXCEPTION(CompressException()
				<< errinfo_message(UTF8("lz4 compression "
					"error"))
				<< errinfo_parameter_name(UTF8("lz4")));
		}

		result->resize(size);

		return result;
	}

}
