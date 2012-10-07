/****************************************************************************
 *            framebufferattachmentutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7c0cb550_305e_4537_af9c_919d2a508372
#define	UUID_7c0cb550_305e_4537_af9c_919d2a508372

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of framebuffer attachments.
	 * @{
	 */
	enum FrameBufferAttachmentType
	{
		/**
		 * Depth attachment.
		 */
		fbat_depth = 0,
		/**
		 * Stencil attachment.
		 */
		fbat_stencil,
		/**
		 * First color attachment.
		 */
		fbat_color_0,
		/**
		 * Second color attachment.
		 */
		fbat_color_1,
		/**
		 * Third color attachment.
		 */
		fbat_color_2,
		/**
		 * Fourth color attachment.
		 */
		fbat_color_3,
		/**
		 * Fifth color attachment.
		 */
		fbat_color_4,
		/**
		 * Sixth color attachment.
		 */
		fbat_color_5,
		/**
		 * Seventh color attachment.
		 */
		fbat_color_6,
		/**
		 * Eighth color attachment.
		 */
		fbat_color_7
	};
	/**
	 * @}
	 */

	class FrameBufferAttachmentUtil
	{
		public:
			static const String &get_str(
				const FrameBufferAttachmentType attachment);
			static FrameBufferAttachmentType
				get_frame_buffer_attachment(const String &str);
			static Uint16 get_index(
				const FrameBufferAttachmentType attachment);
			static Uint32 get_frame_buffer_attachment_count()
				noexcept;

	};

	OutStream &operator<<(OutStream &str,
		const FrameBufferAttachmentType value);
	InStream &operator>>(InStream &str, FrameBufferAttachmentType &value);

}

#endif	/* UUID_7c0cb550_305e_4537_af9c_919d2a508372 */
