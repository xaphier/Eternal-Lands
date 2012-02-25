/****************************************************************************
 *            simpleframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_41d88930_f53e_425a_a4dd_253a28e905c9
#define	UUID_41d88930_f53e_425a_a4dd_253a28e905c9

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
 * @brief The @c class SimpleFrameBuffer.
 * This file contains the @c class SimpleFrameBuffer.
 */
namespace eternal_lands
{

	class SimpleFrameBuffer: public AbstractFrameBuffer
	{
		private:
			FrameBuffer m_frame_buffer;
			boost::scoped_ptr<RenderBuffer> m_render_buffer;
			Uint32 m_layer;
			bool m_color;

			void do_bind(const Uint32 layer);

		public:
			SimpleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 layers, const Uint16 mipmaps,
				const TextureFormatType format);
			virtual ~SimpleFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color);
			virtual void clear(const glm::vec4 &color,
				const float depth);
			virtual void unbind();

			inline Uint32 get_layer() const
			{
				return m_layer;
			}

	};

}

#endif	/* UUID_41d88930_f53e_425a_a4dd_253a28e905c9 */
