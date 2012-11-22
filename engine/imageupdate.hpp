/****************************************************************************
 *            imageupdate.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c9415c5d_2786_41a2_aca2_da562dbf9622
#define	UUID_c9415c5d_2786_41a2_aca2_da562dbf9622

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "cubemapfaceutil.hpp"

/**
 * @file
 * @brief The @c class ImageUpdate.
 * This file contains the @c class ImageUpdate.
 */
namespace eternal_lands
{

	/**
	 * @brief class for image updates.
	 *
	 * class for image updates.
	 */
	class ImageUpdate
	{
		private:
			String m_name;
			AbstractReadMemorySharedPtr m_buffer;
			glm::uvec3 m_offset;
			glm::uvec3 m_size;
			CubeMapFaceType m_face;
			GLenum m_format;
			GLenum m_type;
			Uint16 m_mipmap;
			bool m_compressed;

		public:
			ImageUpdate(const String &name,
				const AbstractReadMemorySharedPtr &buffer,
				const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const CubeMapFaceType face,
				const GLenum format, const GLenum type,
				const Uint16 mipmap, const bool compressed);
			~ImageUpdate() noexcept;

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_buffer(
				const AbstractReadMemorySharedPtr &buffer)
				noexcept
			{
				m_buffer = buffer;
			}

			inline void set_offset(const glm::uvec3 &offset)
				noexcept
			{
				m_offset = offset;
			}

			inline void set_size(const glm::uvec3 &size) noexcept
			{
				m_size = size;
			}

			inline void set_face(const CubeMapFaceType face)
				noexcept
			{
				m_face = face;
			}

			inline void set_format(const GLenum format) noexcept
			{
				m_format = format;
			}

			inline void set_type(const GLenum type) noexcept
			{
				m_type = type;
			}

			inline void set_mipmap(const Uint16 mipmap) noexcept
			{
				m_mipmap = mipmap;
			}

			inline void set_compressed(const bool compressed)
				noexcept
			{
				m_compressed = compressed;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const AbstractReadMemorySharedPtr &get_buffer()
				const noexcept
			{
				return m_buffer;
			}

			inline const glm::uvec3 &get_offset() const noexcept
			{
				return m_offset;
			}

			inline const glm::uvec3 &get_size() const noexcept
			{
				return m_size;
			}

			inline CubeMapFaceType get_face() const noexcept
			{
				return m_face;
			}

			inline GLenum get_format() const noexcept
			{
				return m_format;
			}

			inline GLenum get_type() const noexcept
			{
				return m_type;
			}

			inline Uint16 get_mipmap() const noexcept
			{
				return m_mipmap;
			}

			inline bool get_compressed() const noexcept
			{
				return m_compressed;
			}

	};

}

#endif	/* UUID_c9415c5d_2786_41a2_aca2_da562dbf9622 */

