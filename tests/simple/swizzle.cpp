/****************************************************************************
 *            swizzle.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "packtool.hpp"
#include "readwritememory.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE swizzle
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(swizzle_3_3_2)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec3 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint8>::max();

	memory.resize(count * sizeof(Uint8));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint8*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0xE0;
	masks.g = 0x1C;
	masks.b = 0x03;

	position.r = 0;
	position.g = 1;
	position.b = 2;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint8*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_3_3_2(false, i);
		t1 = el::PackTool::unpack_uint_2_3_3_rev(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_2_3_3_rev)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec3 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint8>::max();

	memory.resize(count * sizeof(Uint8));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint8*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0x07;
	masks.g = 0x38;
	masks.b = 0xC0;

	position.r = 2;
	position.g = 1;
	position.b = 0;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint8*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_2_3_3_rev(false, i);
		t1 = el::PackTool::unpack_uint_3_3_2(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_5_6_5)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec3 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0xF800;
	masks.g = 0x07E0;
	masks.b = 0x001F;

	position.r = 0;
	position.g = 1;
	position.b = 2;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_5_6_5(false, i);
		t1 = el::PackTool::unpack_uint_5_6_5_rev(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_5_6_5_rev)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec3 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0x001F;
	masks.g = 0x07E0;
	masks.b = 0xF800;

	position.r = 2;
	position.g = 1;
	position.b = 0;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_5_6_5_rev(false, i);
		t1 = el::PackTool::unpack_uint_5_6_5(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_5_5_5_1)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0xF800;
	masks.g = 0x07C0;
	masks.b = 0x003E;
	masks.a = 0x0001;

	position.r = 0;
	position.g = 1;
	position.b = 2;
	position.a = 3;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_5_5_5_1(false, i);
		t1 = el::PackTool::unpack_uint_1_5_5_5_rev(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_1_5_5_5_rev)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0x001F;
	masks.g = 0x03E0;
	masks.b = 0x7C00;
	masks.a = 0x8000;

	position.r = 3;
	position.g = 2;
	position.b = 1;
	position.a = 0;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_1_5_5_5_rev(false, i);
		t1 = el::PackTool::unpack_uint_5_5_5_1(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_4_4_4_4)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0xF000;
	masks.g = 0x0F00;
	masks.b = 0x00F0;
	masks.a = 0x000F;

	position.r = 0;
	position.g = 1;
	position.b = 2;
	position.a = 3;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_4_4_4_4(false, i);
		t1 = el::PackTool::unpack_uint_4_4_4_4_rev(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_4_4_4_4_rev)
{
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint16));

	for (i = 0; i < count; i++)
	{
		static_cast<Uint16*>(memory.get_ptr())[i] = i;
	}

	masks.r = 0x000F;
	masks.g = 0x00F0;
	masks.b = 0x0F00;
	masks.a = 0xF000;

	position.r = 3;
	position.g = 2;
	position.b = 1;
	position.a = 0;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint16*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_4_4_4_4_rev(false, i);
		t1 = el::PackTool::unpack_uint_4_4_4_4(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_10_10_10_2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	el::Uint32Vector values;
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint32));

	for (i = 0; i < count; i++)
	{
		values.push_back(random_uint32());
		static_cast<Uint32*>(memory.get_ptr())[i] = values[i];
	}

	masks.r = 0xFFC00000;
	masks.g = 0x003FF000;
	masks.b = 0x00000FFC;
	masks.a = 0x00000003;

	position.r = 0;
	position.g = 1;
	position.b = 2;
	position.a = 3;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint32*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_10_10_10_2(false, values[i]);
		t1 = el::PackTool::unpack_uint_2_10_10_10_rev(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(swizzle_2_10_10_10_rev)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	el::Uint32Vector values;
	el::ReadWriteMemory memory;
	glm::uvec4 masks, position;
	glm::vec4 t0, t1;
	Uint32 i, value, count;

	count = std::numeric_limits<Uint16>::max();

	memory.resize(count * sizeof(Uint32));

	for (i = 0; i < count; i++)
	{
		values.push_back(random_uint32());
		static_cast<Uint32*>(memory.get_ptr())[i] = values[i];
	}

	masks.r = 0x000003FF;
	masks.g = 0x000FFC00;
	masks.b = 0x3FF00000;
	masks.a = 0xC0000000;

	position.r = 3;
	position.g = 2;
	position.b = 1;
	position.a = 0;

	el::PackTool::swizzle(masks, position, count, memory, memory);

	for (i = 0; i < count; i++)
	{
		value = static_cast<Uint32*>(memory.get_ptr())[i];

		t0 = el::PackTool::unpack_uint_2_10_10_10_rev(false, values[i]);
		t1 = el::PackTool::unpack_uint_10_10_10_2(false, value);

		BOOST_CHECK_CLOSE(t0.r, t1.r, 0.01);
		BOOST_CHECK_CLOSE(t0.g, t1.g, 0.01);
		BOOST_CHECK_CLOSE(t0.b, t1.b, 0.01);
		BOOST_CHECK_CLOSE(t0.a, t1.a, 0.01);
	}
}
