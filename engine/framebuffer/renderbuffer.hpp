/****************************************************************************
 *            renderbuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c
#define	UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class RenderBuffer.
 * This file contains the @c class RenderBuffer.
 */

namespace eternal_lands
{

	class RenderBuffer
	{
		private:
			GLuint m_id;
			Uint32 m_width;
			Uint32 m_height;
			Uint32 m_samples;
			TextureFormatType m_format;

		public:
			RenderBuffer(const Uint32 width, const Uint32 height,
				const Uint32 samples,
				const TextureFormatType format);
			~RenderBuffer() throw();
			void bind_to_framebuffer(const GLenum attachment);

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline Uint32 get_samples() const
			{
				return m_samples;
			}

			inline TextureFormatType get_format() const
			{
				return m_format;
			}

	};

}

#endif	/* UUID_8469d7c3_d4aa_4726_b0e2_691b32c2de9c */
