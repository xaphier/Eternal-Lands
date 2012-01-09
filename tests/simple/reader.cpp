/****************************************************************************
 *            reader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "reader.hpp"
#include "memorybuffer.hpp"
#define BOOST_TEST_MODULE reader
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(reader_default_creation)
{
	el::MemoryBufferSharedPtr memory;
	el::ReaderSharedPtr reader;

	BOOST_CHECK_NO_THROW(memory.reset(new el::MemoryBuffer(128)));
	BOOST_CHECK_NO_THROW(reader.reset(new el::Reader(memory,
		el::String("test"))));

	BOOST_CHECK_EQUAL(reader->get_size(), 128);
	BOOST_CHECK_EQUAL(reader->get_position(), 0);
	BOOST_CHECK_EQUAL(reader->get_name().get(), "test");
}

BOOST_AUTO_TEST_CASE(reader_zero_read)
{
	el::MemoryBufferSharedPtr memory;
	el::ReaderSharedPtr reader;

	BOOST_CHECK_NO_THROW(memory.reset(new el::MemoryBuffer(128)));

	memset(memory->get_ptr(), 0, 128);

	BOOST_CHECK_NO_THROW(reader.reset(new el::Reader(memory,
		el::String("test"))));

	BOOST_CHECK_EQUAL(reader->get_size(), 128);
	BOOST_CHECK_EQUAL(reader->get_position(), 0);
	BOOST_CHECK_EQUAL(reader->get_name().get(), "test");

	BOOST_CHECK_EQUAL(reader->read_u8(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 1);

	BOOST_CHECK_EQUAL(reader->read_s8(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 2);

	BOOST_CHECK_EQUAL(reader->read_u16_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 4);

	BOOST_CHECK_EQUAL(reader->read_u16_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 6);

	BOOST_CHECK_EQUAL(reader->read_s16_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 8);

	BOOST_CHECK_EQUAL(reader->read_s16_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 10);

	BOOST_CHECK_EQUAL(reader->read_u32_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 14);

	BOOST_CHECK_EQUAL(reader->read_u32_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 18);

	BOOST_CHECK_EQUAL(reader->read_s32_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 22);

	BOOST_CHECK_EQUAL(reader->read_s32_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 26);

	BOOST_CHECK_EQUAL(reader->read_u64_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 34);

	BOOST_CHECK_EQUAL(reader->read_u64_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 42);

	BOOST_CHECK_EQUAL(reader->read_s64_le(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 50);

	BOOST_CHECK_EQUAL(reader->read_s64_be(), 0);
	BOOST_CHECK_EQUAL(reader->get_position(), 58);

	BOOST_CHECK_EQUAL(reader->read_float_le(), 0.0f);
	BOOST_CHECK_EQUAL(reader->get_position(), 62);

	BOOST_CHECK_EQUAL(reader->read_float_be(), 0.0f);
	BOOST_CHECK_EQUAL(reader->get_position(), 66);

	BOOST_CHECK_EQUAL(reader->read_utf8_string(32), "");
	BOOST_CHECK_EQUAL(reader->get_position(), 98);
}
