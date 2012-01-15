/****************************************************************************
 *            multisampleframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			FrameBuffer m_frame_buffer;
			FrameBuffer m_multisample_frame_buffer;
			boost::scoped_ptr<RenderBuffer> m_render_buffer;
			boost::scoped_ptr<RenderBuffer> m_depth_render_buffer;
			Uint32 m_layer;

		public:
			MultiSampleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 layers, const Uint16 mipmaps,
				const Uint16 samples,
				const TextureFormatType format);
			~MultiSampleFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color);
			virtual void clear(const glm::vec4 &color,
				const float depth);
			virtual void unbind();

	};

}

#endif	/* UUID_9712d3ad_9e50_46ae_857e_292e2df8596c */
