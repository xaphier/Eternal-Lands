/****************************************************************************
 *            simpleframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "texturetargetutil.hpp"
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

			virtual void detach_texture(
				const FrameBufferAttachmentType attachment)
				override;
			virtual void attach_texture(
				const TextureSharedPtr &texture,
				const FrameBufferAttachmentType attachment,
				const Uint16 layer) override;
			virtual void do_attach_depth_render_buffer(
				bool &depth, bool &stencil);

		public:
			SimpleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const bool depth_buffer);
			virtual ~SimpleFrameBuffer() noexcept;
			virtual void bind();
			virtual void clear(const float depth,
				const GLint stencil) override;
			virtual void clear(const glm::vec4 &color,
				const Uint16 index) override;
			virtual void unbind();
			virtual void blit_to_back_buffer(const glm::uvec4 &rect,
				const Uint16 layer, const bool color,
				const bool depth, const bool stencil) override;
			virtual void set_draw_buffer(const Uint16 index,
				const bool enabled);

	};

}

#endif	/* UUID_41d88930_f53e_425a_a4dd_253a28e905c9 */
