/****************************************************************************
 *            filterframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_166600ae_f8bc_4049_a17f_41226a789a54
#define	UUID_166600ae_f8bc_4049_a17f_41226a789a54

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractframebuffer.hpp"
#include "framebuffer.hpp"
#include "textureformatutil.hpp"
#include "renderbuffer.hpp"

/**
 * @file
 * @brief The @c class FilterFrameBuffer.
 * This file contains the @c class FilterFrameBuffer.
 */
namespace eternal_lands
{

	class FilterFrameBuffer: public AbstractFrameBuffer
	{
		private:
			FrameBuffer m_frame_buffer;

		public:
			FilterFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const TextureFormatType format);
			~FilterFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color);
			virtual void unbind();

	};

}

#endif	/* UUID_166600ae_f8bc_4049_a17f_41226a789a54 */
