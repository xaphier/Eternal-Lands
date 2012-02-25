/****************************************************************************
 *            triangles.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d6fa9a54_47b0_48ca_a706_386a8678befb
#define	UUID_d6fa9a54_47b0_48ca_a706_386a8678befb

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "primitiveutil.hpp"

/**
 * @file
 * @brief The @c class Triangles.
 * This file contains the @c class Triangles.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for triangles.
	 *
	 * @c class for triangles.
	 */
	class Triangles
	{
		private:
			const Uint32Vector &m_indices;
			const SubMeshVector &m_sub_meshs;
			Uint32Array3 m_current_indices;
			Uint32 m_index;
			Uint32 m_count;
			Uint32 m_offset;
			Uint32 m_base_vertex;
			Uint32 m_restart_index;
			PrimitiveType m_primitive;
			bool m_use_restart_index;
			bool m_flip_triangle;

			bool get_degenerated_triangle() const;
			bool do_next_triangle();

		public:
			Triangles(const Uint32Vector &indices,
				const SubMeshVector &sub_meshs,
				const PrimitiveType primitive,
				const Uint32 restart_index,
				const bool use_restart_index);
			~Triangles() throw();

			void get_triangles(const Uint32 sub_mesh_index,
				Uint32Vector &indices);
			bool start(const Uint32 sub_mesh_index,
				const bool use_base_vertex);
			bool next_triangle();

			inline const Uint32Array3 &get_current_indices() const
			{
				return m_current_indices;
			}

			inline Uint32 get_current_index(const Uint32 index)
				const
			{
				return m_current_indices[index];
			}

			static Uint32 get_index_count(
				const PrimitiveType primitive,
				const Uint32 count);

	};

}

#endif	/* UUID_d6fa9a54_47b0_48ca_a706_386a8678befb */
