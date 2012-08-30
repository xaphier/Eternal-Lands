/****************************************************************************
 *            extrenderbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_eed3acd5_f766_4840_86c2_ff1238328718
#define	UUID_eed3acd5_f766_4840_86c2_ff1238328718

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractrendertarget.hpp"

/**
 * @file
 * @brief The @c class ExtRenderBuffer.
 * This file contains the @c class ExtRenderBuffer.
 */

namespace eternal_lands
{

	class ExtRenderBuffer: public AbstractRenderTarget
	{
		private:
			GLuint m_id;

		public:
			ExtRenderBuffer(const String &name, const Uint32 width,
				const Uint32 height,
				const TextureFormatType format);
			virtual ~ExtRenderBuffer() noexcept;
			virtual void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer);
			void attach(const GLenum attachment);

	};

}

#endif	/* UUID_eed3acd5_f766_4840_86c2_ff1238328718 */
