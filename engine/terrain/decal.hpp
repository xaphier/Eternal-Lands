/****************************************************************************
 *            decal.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_12f9139b_9187_4f8d_ae08_f803fb2c4428
#define	UUID_12f9139b_9187_4f8d_ae08_f803fb2c4428

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Decal.
 * This file contains the @c class Decal.
 */
namespace eternal_lands
{

	class Decal: public boost::noncopyable
	{
		private:
			TextureSharedPtr m_texture;
			glm::mat2x3 m_transform_matrix; 

		public:
			Decal(const TextureCacheSharedPtr &texture_cache,
				const DecalDescription &description);
			~Decal() noexcept;
			glm::vec4 get_decal_rect() const;

			inline const TextureSharedPtr &get_texture() const
				noexcept
			{
				return m_texture;
			}

			inline const glm::mat2x3 &get_transform_matrix() const
				noexcept
			{
				return m_transform_matrix;
			}

			inline void set_texture(const TextureSharedPtr &texture)
				noexcept
			{
				m_texture = texture;
			}

			inline void set_transform_matrix(
				const glm::mat2x3 &transform_matrix) noexcept
			{
				m_transform_matrix = transform_matrix;
			}

	};

}

#endif	/* UUID_12f9139b_9187_4f8d_ae08_f803fb2c4428 */
