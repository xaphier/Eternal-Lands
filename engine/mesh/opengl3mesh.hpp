/****************************************************************************
 *            opengl3mesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a
#define	UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl2mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl3Mesh.
 * This file contains the @c class OpenGl3Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.x.
	 *
	 * @c class for meshs in a format for OpenGL 3.x using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl3Mesh: public OpenGl2Mesh
	{
		private:
			class VertexArrayObject;
			/**
			 * Vertex array object
			 */
			boost::shared_ptr<VertexArrayObject> m_id;
			BitSet32 m_used_attributes;

		protected:
			void init_vertex_buffers(
				const VertexStreamBitset vertex_buffers)
				override;
			virtual void init_vertex_buffers(
				BitSet32 &used_attributes);

		public:
			/**
			 * Default constructor.
			 */
			OpenGl3Mesh(const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper,
				const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl3Mesh() noexcept;

			void bind(BitSet32 &used_attributes) const override;
			void unbind() const override;
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

	};

}

#endif	/* UUID_ed58a002_ed47_43c1_acda_0bd30dbb9d7a */

