/****************************************************************************
 *            renderbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c
#define	UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractrendertarget.hpp"

/**
 * @file
 * @brief The @c class RenderBuffer.
 * This file contains the @c class RenderBuffer.
 */

namespace eternal_lands
{

	class RenderBuffer: public AbstractRenderTarget
	{
		private:
			GLuint m_id;
			Uint32 m_width;
			Uint32 m_height;
			Uint32 m_samples;
			TextureFormatType m_format;

		public:
			RenderBuffer(const String &name, const Uint32 width,
				const Uint32 height,
				const Uint32 samples,
				const TextureFormatType format);
			virtual ~RenderBuffer() noexcept;
			virtual void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer);
			void attach(const GLenum attachment);

	};

}

#endif	/* UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c */
