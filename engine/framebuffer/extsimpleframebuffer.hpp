/****************************************************************************
 *            extsimpleframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

		public:
			ExtSimpleFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint16 mipmaps,
				const TextureFormatType format);
			virtual ~ExtSimpleFrameBuffer() throw();
			virtual void bind(const Uint32 layer);
			virtual void bind_texture(const Uint32 layer);
			virtual void blit();
			virtual void clear(const glm::vec4 &color);
			virtual void unbind();

	};

}

#endif	/* UUID_faf12790_c512_4a22_ace0_45faea23d9f9 */
