/****************************************************************************
 *            layeredframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5cdd664b_bfc8_4d5a_9b45_660fb3d8c7ef
#define	UUID_5cdd664b_bfc8_4d5a_9b45_660fb3d8c7ef

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
 * @brief The @c class LayeredFrameBuffer.
 * This file contains the @c class LayeredFrameBuffer.
 */
namespace eternal_lands
{

	class LayeredFrameBuffer: public AbstractFrameBuffer
	{
		private:
			FrameBuffer m_frame_buffer;
			boost::scoped_ptr<RenderBuffer> m_render_buffer;
			bool m_color;

		public:
			LayeredFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 layers, const Uint16 mipmaps,
				const TextureFormatType format);
			virtual ~LayeredFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color);
			virtual void clear(const glm::vec4 &color,
				const float depth);
			virtual void unbind();

	};

}

#endif	/* UUID_5cdd664b_bfc8_4d5a_9b45_660fb3d8c7ef */