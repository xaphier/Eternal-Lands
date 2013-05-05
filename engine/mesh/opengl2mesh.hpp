/****************************************************************************
 *            opengl2mesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3
#define	UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractmesh.hpp"
#include "hardwarebuffer/hardwarebuffer.hpp"

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
		private:
			typedef boost::array<HardwareBufferSharedPtr,
				vertex_stream_count>
					HardwareBufferSharedPtrArray;

			const HardwareBufferMapperConstWeakPtr
				m_hardware_buffer_mapper;
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

		protected:
			inline HardwareBufferMapperConstSharedPtr
				get_hardware_buffer_mapper() const noexcept
			{
				HardwareBufferMapperConstSharedPtr result;

				result = m_hardware_buffer_mapper.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void clone_buffers(
				const VertexStreamBitset shared_vertex_datas,
				const bool shared_index_data,
				OpenGl2Mesh &mesh) const;
			void bind_vertex_buffers(BitSet32 &used_attributes)
				const;
			void unbind_vertex_buffers() const;
			void bind_index_buffer() const;
			void unbind_index_buffer() const;

			inline bool get_has_index_data() const
			{
				return m_index_data.get() != nullptr;
			}

			virtual AbstractWriteMemorySharedPtr
				get_vertex_buffer(const Uint16 index);
			virtual void set_vertex_buffer(
				const AbstractReadMemoryConstSharedPtr &buffer,
				const Uint16 index);
			virtual AbstractWriteMemorySharedPtr get_index_buffer();
			virtual void init_vertex_buffers(
				const VertexStreamBitset vertex_buffers);
			virtual void init_index_buffer();

		public:
			/**
			 * Default constructor.
			 */
			OpenGl2Mesh(const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper,
				const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			virtual ~OpenGl2Mesh() noexcept;

			virtual void bind(BitSet32 &used_attributes) const;
			virtual void unbind() const;

			/**
			 * Draws the mesh using the given draw data.
			 */
			virtual void draw(const MeshDrawData &draw_data,
				const Uint32 instances,
				const PrimitiveType primitive) const;
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
			virtual void update_vertex_buffer(
				const AbstractReadMemoryConstSharedPtr &buffer,
				 const Uint16 index);
			virtual bool get_supports_base_vertex() const;
			virtual bool get_supports_restart_index() const;
			virtual bool get_supports_vertex_attribute_divisor()
				const;

	};

}

#endif	/* UUID_1e9f39f9_b5d8_4cb4_af34_f37c6d6f51d3 */

