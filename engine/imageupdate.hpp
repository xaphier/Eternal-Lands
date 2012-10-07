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
#include "readwritememory.hpp"

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
			ImageSharedPtr m_image;
			glm::uvec3 m_offsets;
			glm::uvec3 m_sizes;
			Uint16 m_mipmap;

		public:
			ImageUpdate();
			ImageUpdate(const ImageSharedPtr &image,
				const Uint16 mipmap = 0);
			ImageUpdate(const ImageSharedPtr &image,
				const glm::uvec3 &offsets,
				const glm::uvec3 &sizes,
				const Uint16 mipmap = 0);
			~ImageUpdate() noexcept;

			inline void set_image(const ImageSharedPtr &image)
				noexcept
			{
				m_image = image;
			}

			inline void set_offset(const glm::uvec3 &offsets)
				noexcept
			{
				m_offsets = offsets;
			}

			inline void set_sizes(const glm::uvec3 &sizes) noexcept
			{
				m_sizes = sizes;
			}

			inline void set_mipmap(const Uint16 mipmap) noexcept
			{
				m_mipmap = mipmap;
			}

			inline const ImageSharedPtr &get_image() const noexcept
			{
				return m_image;
			}

			inline const glm::uvec3 &get_offsets() const noexcept
			{
				return m_offsets;
			}

			inline const glm::uvec3 &get_sizes() const noexcept
			{
				return m_sizes;
			}

			inline Uint16 get_mipmap() const noexcept
			{
				return m_mipmap;
			}

	};

}

#endif	/* UUID_c9415c5d_2786_41a2_aca2_da562dbf9622 */

