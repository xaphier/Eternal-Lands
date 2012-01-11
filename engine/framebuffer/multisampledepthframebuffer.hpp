/****************************************************************************
 *            multisampledepthframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_15feb442_dae9_4dad_8e20_a3311cd0f172
#define	UUID_15feb442_dae9_4dad_8e20_a3311cd0f172

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
 * @brief The @c class MultiSampleDepthFrameBuffer.
 * This file contains the @c class MultiSampleDepthFrameBuffer.
 */
namespace eternal_lands
{

	class MultiSampleDepthFrameBuffer: public AbstractFrameBuffer
	{
		private:
			FrameBuffer m_frame_buffer;
			FrameBuffer m_multisample_frame_buffer;
			boost::scoped_ptr<RenderBuffer> m_render_buffer;
			Uint32 m_layer;
			bool m_stencil;

		public:
			MultiSampleDepthFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 layers, const Uint16 mipmaps,
				const Uint16 samples,
				const TextureFormatType format);
			~MultiSampleDepthFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color,
				const float depth);
			virtual void unbind();

	};

}

#endif	/* UUID_15feb442_dae9_4dad_8e20_a3311cd0f172 */
