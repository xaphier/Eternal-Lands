/****************************************************************************
 *            framebufferattachmentutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "framebufferattachmentutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class FrameBufferAttachmentTypeData
		{
			private:
				const String m_name;
				const Uint16 m_index;

			public:
				inline FrameBufferAttachmentTypeData(
					const String &name,
					const Uint16 index): m_name(name),
					m_index(index)
				{
				}

				inline ~FrameBufferAttachmentTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline Uint16 get_index() const noexcept
				{
					return m_index;
				}

		};

		const FrameBufferAttachmentTypeData
			frame_buffer_attachment_datas[] =
		{
			FrameBufferAttachmentTypeData(String(UTF8("depth")),
				0),
			FrameBufferAttachmentTypeData(String(UTF8("stencil")),
				0),
			FrameBufferAttachmentTypeData(String(UTF8("color[0]")),
				0),
			FrameBufferAttachmentTypeData(String(UTF8("color[1]")),
				1),
			FrameBufferAttachmentTypeData(String(UTF8("color[2]")),
				2),
			FrameBufferAttachmentTypeData(String(UTF8("color[3]")),
				3),
			FrameBufferAttachmentTypeData(String(UTF8("color[4]")),
				4),
			FrameBufferAttachmentTypeData(String(UTF8("color[5]")),
				5),
			FrameBufferAttachmentTypeData(String(UTF8("color[6]")),
				6),
			FrameBufferAttachmentTypeData(String(UTF8("color[7]")),
				7)
		};

		const Uint32 frame_buffer_attachment_datas_count =
			sizeof(frame_buffer_attachment_datas) /
			sizeof(FrameBufferAttachmentTypeData);

	}

	const String &FrameBufferAttachmentUtil::get_str(
		const FrameBufferAttachmentType frame_buffer_attachment)
	{
		if (frame_buffer_attachment_datas_count <=
			frame_buffer_attachment)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_frame_buffer_attachment_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					frame_buffer_attachment))
				<< boost::errinfo_type_info_name(
					UTF8("FrameBufferAttachmentType")));
		}

		return frame_buffer_attachment_datas[
			frame_buffer_attachment].get_name();
	}

	Uint16 FrameBufferAttachmentUtil::get_index(
		const FrameBufferAttachmentType frame_buffer_attachment)
	{
		if (frame_buffer_attachment_datas_count <=
			frame_buffer_attachment)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_frame_buffer_attachment_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					frame_buffer_attachment))
				<< boost::errinfo_type_info_name(
					UTF8("FrameBufferAttachmentType")));
		}

		return frame_buffer_attachment_datas[
			frame_buffer_attachment].get_index();
	}

	FrameBufferAttachmentType FrameBufferAttachmentUtil::
		get_frame_buffer_attachment(const String &str)
	{
		Uint32 i;
		FrameBufferAttachmentType frame_buffer_attachment;

		for (i = 0; i < frame_buffer_attachment_datas_count; i++)
		{
			frame_buffer_attachment = static_cast<
				FrameBufferAttachmentType>(i);

			if (str == get_str(frame_buffer_attachment))
			{
				return frame_buffer_attachment;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("FrameBufferAttachmentType")));
	}

	Uint32 FrameBufferAttachmentUtil::get_frame_buffer_attachment_count()
		noexcept
	{
		return frame_buffer_attachment_datas_count;
	}

	OutStream& operator<<(OutStream &str,
		const FrameBufferAttachmentType value)
	{
		str << FrameBufferAttachmentUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, FrameBufferAttachmentType &value)
	{
		StringType string;

		str >> string;

		value = FrameBufferAttachmentUtil::get_frame_buffer_attachment(
			String(string));

		return str;
	}

}
