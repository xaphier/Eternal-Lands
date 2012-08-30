/****************************************************************************
 *            extsimpleframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_faf12790_c512_4a22_ace0_45faea23d9f9
#define	UUID_faf12790_c512_4a22_ace0_45faea23d9f9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractframebuffer.hpp"
#include "extframebuffer.hpp"
#include "textureformatutil.hpp"
#include "texturetargetutil.hpp"
#include "extrenderbuffer.hpp"

/**
 * @file
 * @brief The @c class ExtSimpleFrameBuffer.
 * This file contains the @c class ExtSimpleFrameBuffer.
 */
namespace eternal_lands
{

	class ExtSimpleFrameBuffer: public AbstractFrameBuffer
	{
		private:
			ExtFrameBuffer m_frame_buffer;
			boost::scoped_ptr<ExtRenderBuffer> m_render_buffer;

			virtual void detach_texture(
				const FrameBufferAttachmentType attachment)
				override;
			virtual void attach_texture(
				const TextureSharedPtr &texture,
				const FrameBufferAttachmentType attachment,
				const Uint16 layer) override;
			virtual void do_attach_depth_render_buffer(
				bool &depth, bool &stencil) override;

		public:
			ExtSimpleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const bool depth_buffer);
			virtual ~ExtSimpleFrameBuffer() noexcept;
			virtual void bind() override;
			virtual void clear(const float depth,
				const GLint stencil) override;
			virtual void clear(const glm::vec4 &color,
				const Uint16 index) override;
			virtual void unbind() override;
			virtual void blit_to_back_buffer(const glm::uvec4 &rect,
				const Uint16 layer, const bool color,
				const bool depth, const bool stencil) override;
			virtual void set_draw_buffer(const Uint16 index,
				const bool enabled);
			virtual void blit_buffers();

	};

}

#endif	/* UUID_faf12790_c512_4a22_ace0_45faea23d9f9 */
