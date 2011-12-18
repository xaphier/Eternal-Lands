/****************************************************************************
 *            opengl2mesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3
#define	UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractmesh.hpp"
#include "hardwarebuffer.hpp"

/**
 * @file
 * @brief The @c class OpenGl2Mesh.
 * This file contains the @c class OpenGl2Mesh.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for meshs in a format for OpenGL 2.x.
	 *
	 * @c class for meshs in a format for OpenGL 2.x using hardware
	 * buffers and shaders.
	 */
	class OpenGl2Mesh: public AbstractMesh
	{
		protected:
			boost::array<HardwareBufferSharedPtr, 16> m_vertex_data;
			HardwareBufferSharedPtr m_index_data;

		protected:
			static void bind(const VertexElements &vertex_elements,
				const HardwareBufferSharedPtr &buffer);

			void bind_vertex_buffers();
			void unbind_vertex_buffers();
			void bind_index_buffers();
			void unbind_index_buffers();

			virtual AbstractWriteMemoryBufferSharedPtr
				get_vertex_buffer(const Uint16 index);
			virtual AbstractWriteMemoryBufferSharedPtr
				get_index_buffer();
			virtual void init_vertices();
			virtual void init_indices();

		public:
			/**
			 * Default constructor.
			 */
			OpenGl2Mesh();

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl2Mesh() throw();

			virtual void bind();
			virtual void unbind();
			virtual void draw(const MeshDrawData &draw_data);
			virtual AbstractMeshSharedPtr clone_vertex_data();
			virtual bool get_supports_restart_index();

	};

}

#endif	/* UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3 */

