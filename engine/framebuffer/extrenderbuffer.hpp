/****************************************************************************
 *            extrenderbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_eed3acd5_f766_4840_86c2_ff1238328718
#define	UUID_eed3acd5_f766_4840_86c2_ff1238328718

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class ExtRenderBuffer.
 * This file contains the @c class ExtRenderBuffer.
 */

namespace eternal_lands
{

	class ExtRenderBuffer
	{
		private:
			GLuint m_id;
			Uint32 m_width;
			Uint32 m_height;
			TextureFormatType m_format;

		public:
			ExtRenderBuffer(const Uint32 width, const Uint32 height,
				const TextureFormatType format);
			~ExtRenderBuffer() throw();
			void bind_to_framebuffer(const GLenum attachment);

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline TextureFormatType get_format() const
			{
				return m_format;
			}

	};

}

#endif	/* UUID_eed3acd5_f766_4840_86c2_ff1238328718 */
