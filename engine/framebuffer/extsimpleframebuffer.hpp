/****************************************************************************
 *            extsimpleframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			Uint32 m_layer;

			void do_bind(const Uint32 layer);

		public:
			ExtSimpleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 depth, const Uint16 mipmaps,
				const TextureTargetType target,
				const TextureFormatType format,
				const bool depth_buffer);
			virtual ~ExtSimpleFrameBuffer() noexcept;
			virtual void bind(const Uint32 layer) override;
			virtual void bind_texture(const Uint32 layer) override;
			virtual void blit() override;
			virtual void clear(const glm::vec4 &color) override;
			virtual void clear(const glm::vec4 &color,
				const float depth) override;
			virtual void unbind() override;
			virtual void blit_to_back_buffer(const glm::uvec4 &rect,
				const Uint16 layer, const bool color,
				const bool depth, const bool stencil) override;

			inline Uint32 get_layer() const
			{
				return m_layer;
			}

	};

}

#endif	/* UUID_faf12790_c512_4a22_ace0_45faea23d9f9 */
