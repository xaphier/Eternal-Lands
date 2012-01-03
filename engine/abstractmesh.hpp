/****************************************************************************
 *            abstractmesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e4486b52_0882_4250_9e47_1c776906bf13
#define	UUID_e4486b52_0882_4250_9e47_1c776906bf13

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "primitiveutil.hpp"

/**
 * @file
 * @brief The @c class AbstractMesh.
 * This file contains the @c class AbstractMesh.
 */
namespace eternal_lands
{

	/**
	 * @brief Abstract base @c class for meshs.
	 *
	 * Abstract base @c class for meshs.
	 */
	class AbstractMesh: public boost::noncopyable
	{
		private:
			SubMeshVector m_sub_meshs;
			VertexFormatSharedPtr m_vertex_format;
			const String m_name;
			Uint32 m_vertex_count;
			Uint32 m_index_count;
			Uint32 m_restart_index;
			PrimitiveType m_primitive_type;
			bool m_use_16_bit_indices;
			bool m_use_restart_index;
			bool m_static_indices;
			bool m_static_vertices;

		protected:
			/**
			 * Default constructor.
			 */
			AbstractMesh(const String &name,
				const bool static_indices,
				const bool static_vertices);

			void copy_vertex_descriptions(const AbstractMesh &mesh);

			virtual void init_vertices() = 0;
			virtual void init_indices() = 0;

			virtual AbstractWriteMemoryBufferSharedPtr
				get_vertex_buffer(const Uint16 index) = 0;

			virtual void set_vertex_buffer(
				const AbstractReadMemoryBufferSharedPtr &buffer,
				const Uint16 index) = 0;

			virtual void update_vertex_buffer(
				const AbstractReadMemoryBufferSharedPtr &buffer,
				const Uint16 index) = 0;

			virtual AbstractWriteMemoryBufferSharedPtr
				get_index_buffer() = 0;

			inline const VertexFormatSharedPtr &get_vertex_format()
				const
			{
				return m_vertex_format;
			}

			inline GLenum get_index_type() const
			{
				if (get_use_16_bit_indices())
				{
					return GL_UNSIGNED_SHORT;
				}
				else
				{
					return GL_UNSIGNED_INT;
				}
			}

			inline Uint32 get_index_offset(const Uint32 offset)
				const
			{
				if (get_use_16_bit_indices())
				{
					return offset * sizeof(Uint16);
				}
				else
				{
					return offset * sizeof(Uint32);
				}
			}

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractMesh() throw();

			/**
			 * Binds the mesh to OpenGL, using vertex buffers etc.
			 */
			virtual void bind(BitSet32 &used_attributes) = 0;

			/**
			 * Unbinds the mesh from OpenGL.
			 */
			virtual void unbind() = 0;

			/**
			 * Draws the mesh using the given draw data.
			 */
			virtual void draw(const MeshDrawData &draw_data) = 0;

			/**
			 * Clones the vertex data of the mesh. Mainly used for
			 * animated actors.
			 */
			virtual AbstractMeshSharedPtr clone_vertex_data() = 0;

			virtual bool get_supports_restart_index() = 0;

			/**
			 * Draws the sub mesh with the given index.
			 */
			void draw(const Uint32 index);

			void init(const VertexFormatSharedPtr &vertex_format,
				const MeshDataToolSharedPtr &source,
				const bool static_indices = true,
				const bool static_vertices = true);

			void init(const bool use_16_bit_indices,
				const VertexFormatSharedPtr &vertex_format,
				const MeshDataToolSharedPtr &source,
				const bool static_indices = true,
				const bool static_vertices = true);

			void init_vertex(
				const VertexFormatSharedPtr &vertex_format,
				const MeshDataToolSharedPtr &source,
				const bool static_vertices = true);
			void init_vertex(const VertexBuffersSharedPtr &buffers,
				const bool static_vertices = true);
			void init_vertex(
				const VertexFormatSharedPtr &vertex_format,
				const Uint32 vertex_count,
				const bool static_vertices = true);
			void update_vertex(
				const VertexBuffersSharedPtr &buffers);

			void init_indices(const Uint32Set &blocks,
				const IndexUpdateSourceSharedPtr &update,
				const bool static_indices = true);

			void init_indices(const bool use_16_bit_indices,
				const PrimitiveType primitive_type,
				const SubMeshVector &sub_meshs,
				const Uint32Vector &indices,
				const bool static_indices = true);

			void get_bounding_box(const glm::mat4x3 &matrix,
				BoundingBox &bounding_box);

			VertexBuffersSharedPtr get_vertex_buffers() const;

			/**
			 */
			const VertexElements &get_vertex_elements(
				const Uint16 index) const;

			/**
			 * Returns the sub meshs.
			 * @result The sub meshs.
			 */
			inline const SubMeshVector &get_sub_meshs() const
			{
				return m_sub_meshs;
			}

			/**
			 * Returns the number of vertexes.
			 * @result The vertex count.
			 */
			inline Uint32 get_vertex_count() const
			{
				return m_vertex_count;
			}

			/**
			 * Returns the number of indexes.
			 * @result The index count.
			 */
			inline Uint32 get_index_count() const
			{
				return m_index_count;
			}

			/**
			 * Returns the OpenGL primitive type (points, lines, ..)
			 */
			inline PrimitiveType get_primitive_type() const
			{
				return m_primitive_type;
			}

			/**
			 * Returns true if 16 bit indices are used.
			 */
			inline bool get_use_16_bit_indices() const
			{
				return m_use_16_bit_indices;
			}

			/**
			 * Return the restart index.
			 * @result The restart index.
			 */
			inline Uint32 get_restart_index() const
			{
				return m_restart_index;
			}

			/**
			 * Returns if the restart index is used.
			 * @result If the restart index is used.
			 */
			inline bool get_use_restart_index() const
			{
				return m_use_restart_index;
			}

			/**
			 * Returns if the indices are mostly static.
			 * @result If the indices are mostly static.
			 */
			inline bool get_static_indices() const
			{
				return m_static_indices;
			}

			/**
			 * Returns if the vertices are mostly static.
			 * @result If the vertices are mostly static.
			 */
			inline bool get_static_vertices() const
			{
				return m_static_vertices;
			}

			/**
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_e4486b52_0882_4250_9e47_1c776906bf13 */

