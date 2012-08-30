/****************************************************************************
 *            abstractrendertarget.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_9c43d14d_b209_485a_b9cb_901f1f0ccbbe
#define	UUID_9c43d14d_b209_485a_b9cb_901f1f0ccbbe

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "texturetargetutil.hpp"

/**
 * @file
 * @brief The @c class AbstractRenderTarget.
 * This file contains the @c class AbstractRenderTarget.
 */
namespace eternal_lands
{

	/**
	 * Types of cube map faces.
	 * @{
	 */
	enum CubeMapFaceType
	{
		cmft_negative_x = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		cmft_negative_y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		cmft_negative_z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		cmft_positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		cmft_positive_y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		cmft_positive_z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	};
	/**
	 * @}
	 */

	/**
	 * @brief base @c class for render targets.
	 *
	 * base @c class for render targets.
	 */
	class AbstractRenderTarget: public boost::noncopyable
	{
		private:
			const String m_name;
			Uint32 m_width;
			Uint32 m_height;
			Uint16 m_samples;
			TextureFormatType m_format;

		protected:
			inline void set_width(const Uint32 width)
			{
				m_width = width;
			}

			inline void set_height(const Uint32 height)
			{
				m_height = height;
			}

			inline void set_samples(const Uint32 samples)
			{
				m_samples = samples;
			}

			inline void set_format(const TextureFormatType format)
				noexcept
			{
				m_format = format;
			}

		public:
			AbstractRenderTarget(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 samples,
				const TextureFormatType format);
			virtual ~AbstractRenderTarget() noexcept;
			virtual void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer) = 0;
			static String get_str(const CubeMapFaceType value);

			inline TextureFormatType get_format() const noexcept
			{
				return m_format;
			}

			inline Uint16 get_samples() const noexcept
			{
				return m_samples;
			}

			inline Uint32 get_width() const noexcept
			{
				return m_width;
			}

			inline Uint32 get_height() const noexcept
			{
				return m_height;
			}

			inline GLenum get_gl_format() const noexcept
			{
				return TextureFormatUtil::get_gl_format(
					get_format());
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

	OutStream& operator<<(OutStream &str, const CubeMapFaceType value);

}

#endif	/* UUID_9c43d14d_b209_485a_b9cb_901f1f0ccbbe */
