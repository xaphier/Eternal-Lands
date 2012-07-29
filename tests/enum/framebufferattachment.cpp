/****************************************************************************
 *            framebufferattachment.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "framebufferattachmentutil.hpp"
#define BOOST_TEST_MODULE frame_buffer_attachment
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_frame_buffer_attachment_count)
{
	BOOST_CHECK_GT(el::FrameBufferAttachmentUtil::
		get_frame_buffer_attachment_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::FrameBufferAttachmentUtil::
		get_frame_buffer_attachment_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::FrameBufferAttachmentUtil::get_str(
			static_cast<el::FrameBufferAttachmentType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_index)
{
	Uint32 i, count;

	count = el::FrameBufferAttachmentUtil::
		get_frame_buffer_attachment_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::FrameBufferAttachmentUtil::get_index(
			static_cast<el::FrameBufferAttachmentType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::FrameBufferAttachmentType type;

	count = el::FrameBufferAttachmentUtil::get_frame_buffer_attachment_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::FrameBufferAttachmentType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::FrameBufferAttachmentUtil::get_str(type).get());
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_str(
		el::fbat_depth).get(), "depth");

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_str(
		el::fbat_stencil).get(), "stencil");

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_str(
		el::fbat_color_1).get(), "color[1]");

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_str(
		el::fbat_color_7).get(), "color[7]");
}

BOOST_AUTO_TEST_CASE(get_index_value)
{
	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_index(
		el::fbat_depth), 0);

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_index(
		el::fbat_stencil), 0);

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_index(
		el::fbat_color_1), 1);

	BOOST_CHECK_EQUAL(el::FrameBufferAttachmentUtil::get_index(
		el::fbat_color_7), 7);
}
