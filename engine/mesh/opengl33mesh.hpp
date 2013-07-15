/****************************************************************************
 *            opengl33mesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_34cfccfb_1db3_4aef_990e_b6299747c7c9
#define	UUID_34cfccfb_1db3_4aef_990e_b6299747c7c9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl32mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl33Mesh.
 * This file contains the @c class OpenGl33Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.3.
	 *
	 * @c class for meshs in a format for OpenGL 3.3 using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl33Mesh: public OpenGl32Mesh
	{
		private:
			static void inti_divisor(
				const VertexElements &vertex_elements);

		protected:
			void init_vertex_buffers(
				BitSet32 &used_attributes) override;

		public:
			/**
			 * Default constructor.
			 */
			OpenGl33Mesh(const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper,
				const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl33Mesh() noexcept;
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
			bool get_supports_vertex_attribute_divisor() const override;

	};

}

#endif	/* UUID_34cfccfb_1db3_4aef_990e_b6299747c7c9 */
