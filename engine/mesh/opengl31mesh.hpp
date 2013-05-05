/****************************************************************************
 *            opengl31mesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6
#define	UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "opengl3mesh.hpp"

/**
 * @file
 * @brief The @c class OpenGl31Mesh.
 * This file contains the @c class OpenGl31Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 3.1.
	 *
	 * @c class for meshs in a format for OpenGL 3.1 using hardware
	 * buffers, shaders and instancing.
	 */
	class OpenGl31Mesh: public OpenGl3Mesh
	{
		public:
			/**
			 * Default constructor.
			 */
			OpenGl31Mesh(const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper,
				const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl31Mesh() noexcept;

			virtual void bind(BitSet32 &used_attributes);
			virtual void unbind();
			/**
			 * Clones the data of the mesh. Used for animated
			 * actors and terrain.
			 * @param shared_vertex_datas If true for a stream,
			 * the data is shared, else the data is cloned.
			 * @param shared_index_data If true, the index data is
			 * shared, else the data is cloned.
			 * @return The new mesh.
			 */
			virtual AbstractMeshSharedPtr clone(
				const VertexStreamBitset shared_vertex_datas,
				const bool shared_index_data) const;
			virtual bool get_supports_restart_index() const;

			/**
			 * Draws the mesh using the given draw data.
			 */
			virtual void draw(const MeshDrawData &draw_data,
				const Uint32 instances,
				const PrimitiveType primitive);

	};

}

#endif	/* UUID_44cee017_6c76_4f62_ba04_4752efb0a6b6 */

