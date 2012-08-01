/****************************************************************************
 *            polygon.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8b3df003_4065_48b2_9ed8_8f3340e21c4a
#define	UUID_8b3df003_4065_48b2_9ed8_8f3340e21c4a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Polygon.
 * This file contains the @c class Polygon.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for polygon.
	 *
	 * @c class for polygon.
	 */
	class Polygon
	{
		private:
			Vec3Vector m_vertices;
			glm::vec3 m_normal;

		public:
			Polygon();
			~Polygon() noexcept;
			void remove_duplicates();
			void transform(const glm::mat4x3 &matrix);
			Polygon get_transformed(const glm::mat4x3 &matrix)
				const;

			inline void clear() noexcept
			{
				m_vertices.clear();
			}

			inline void add_vertex(const glm::vec3 &vertex) noexcept
			{
				m_vertices.push_back(vertex);
			}

			inline Uint32 get_vertex_count() const noexcept
			{
				return m_vertices.size();
			}

			inline const Vec3Vector &get_vertices() const noexcept
			{
				return m_vertices;
			}

			inline const glm::vec3 &get_vertex(const Uint32 index)
				const
			{
				assert(index < get_vertex_count());

				return m_vertices[index];
			}

			inline void remove_vertex(const Uint32 index)
			{
				Vec3Vector::iterator it;

				assert(index < get_vertex_count());

				it = m_vertices.begin();
				std::advance(it, index);

				m_vertices.erase(it);
			}

	};

}

#endif	/* UUID_8b3df003_4065_48b2_9ed8_8f3340e21c4a */
