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
			typedef boost::array<HardwareBufferSharedPtr,
				vertex_stream_count>
					HardwareBufferSharedPtrArray;

			HardwareBufferSharedPtrArray m_vertex_data;
			HardwareBufferSharedPtr m_index_data;

			HardwareBufferUsageType get_vertices_usage() const
			{
				if (get_static_vertices())
				{
					return hbut_static_draw;
				}

				return hbut_stream_draw;
			}

			HardwareBufferUsageType get_indices_usage() const
			{
				if (get_static_indices())
				{
					return hbut_static_draw;
				}

				return hbut_stream_draw;
			}

			static void bind(const VertexElements &vertex_elements,
				const HardwareBufferSharedPtr &buffer,
				BitSet32 &used_attributes);

			void bind_vertex_buffers(BitSet32 &used_attributes);
			void unbind_vertex_buffers();
			void bind_index_buffers();
			void unbind_index_buffers();

			virtual AbstractWriteMemoryBufferSharedPtr
				get_vertex_buffer(const Uint16 index);
			virtual void set_vertex_buffer(
				const AbstractReadMemoryBufferSharedPtr &buffer,
				const Uint16 index);
			virtual void update_vertex_buffer(
				const AbstractReadMemoryBufferSharedPtr &buffer,
				const Uint16 index);
			virtual AbstractWriteMemoryBufferSharedPtr
				get_index_buffer();
			virtual void init_vertices();
			virtual void init_indices();

		public:
			/**
			 * Default constructor.
			 */
			OpenGl2Mesh(const String &name,
				const bool static_indices,
				const bool static_vertices);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl2Mesh() throw();

			virtual void bind(BitSet32 &used_attributes);
			virtual void unbind();
			virtual void draw(const MeshDrawData &draw_data);
			virtual AbstractMeshSharedPtr clone_vertex_data();
			virtual bool get_supports_restart_index();

	};

}

#endif	/* UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3 */

