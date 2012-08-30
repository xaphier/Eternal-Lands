/****************************************************************************
 *            multisampleframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_9712d3ad_9e50_46ae_857e_292e2df8596c
#define	UUID_9712d3ad_9e50_46ae_857e_292e2df8596c

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
 * @brief The @c class MultiSampleFrameBuffer.
 * This file contains the @c class MultiSampleFrameBuffer.
 */
namespace eternal_lands
{

	class MultiSampleFrameBuffer: public AbstractFrameBuffer
	{
		private:
			typedef std::map<FrameBufferAttachmentType,
				boost::shared_ptr<RenderBuffer> >
				FrameBufferAttachmentTypeRenderBufferMap;
			typedef std::pair<FrameBufferAttachmentType,
				boost::shared_ptr<RenderBuffer> >
				FrameBufferAttachmentTypeRenderBufferPair;
			FrameBuffer m_frame_buffer;
			FrameBuffer m_multisample_frame_buffer;
			boost::scoped_ptr<RenderBuffer> m_render_buffer;
			FrameBufferAttachmentTypeRenderBufferMap
				m_render_buffers;
			Uint16 m_samples, m_draw_buffer;

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
			MultiSampleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint16 samples, const bool depth_buffer);
			~MultiSampleFrameBuffer() noexcept;
			void bind();
			void clear(const float depth, const GLint stencil);
			void clear(const glm::vec4 &color,
				const Uint16 index);
			void unbind();
			void blit_to_back_buffer(const glm::uvec4 &rect,
				const Uint16 layer, const bool color,
				const bool depth, const bool stencil);
			void set_draw_buffer(const Uint16 index,
				const bool enabled);
			virtual void blit_buffers();

			inline Uint16 get_samples() const noexcept
			{
				return m_samples;
			}

	};

}

#endif	/* UUID_9712d3ad_9e50_46ae_857e_292e2df8596c */
