/****************************************************************************
 *            opengl31mesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8
#define	UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl31mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl32Mesh.
 * This file contains the @c class OpenGl32Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.2.
	 *
	 * @c class for meshs in a format for OpenGL 3.2 using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl32Mesh: public OpenGl31Mesh
	{
		private:

		public:
			/**
			 * Default constructor.
			 */
			OpenGl32Mesh(const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper,
				const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl32Mesh() noexcept;
			/**
			 * Clones the data of the mesh. Used for animated
			 * actors and terrain.
			 * @param shared_vertex_datas If true for a stream,
			 * the data is shared, else the data is cloned.
			 * @param shared_index_data If true, the index data is
			 * shared, else the data is cloned.
			 * @return The new mesh.
			 */
			AbstractMeshSharedPtr clone(
				const VertexStreamBitset shared_vertex_datas,
				const bool shared_index_data) const override;
			bool get_supports_base_vertex() const override;

			/**
			 * Draws the mesh using the given draw data.
			 */
			void draw(const MeshDrawData &draw_data,
				const Uint32 instances,
				const PrimitiveType primitive) const override;

	};

}

#endif	/* UUID_cfbf3d73_d078_4803_ab3c_bdc9507a04c8 */

