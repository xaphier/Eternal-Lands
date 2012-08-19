/****************************************************************************
 *            abstractmesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e4486b52_0882_4250_9e47_1c776906bf13
#define	UUID_e4486b52_0882_4250_9e47_1c776906bf13

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "primitiveutil.hpp"
#include "alignedshort8array.hpp"

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
			/**
			 * These min max boxes are used for lod selection.
			 */
			AlignedShort8Array m_min_max_boxes;
			SubMeshVector m_sub_meshs;
			VertexFormatSharedPtr m_vertex_format;
			const String m_name;
			Uint32 m_index_count;
			Uint32 m_vertex_count;
			Uint32 m_instance_count;
			PrimitiveType m_primitive;
			bool m_use_16_bit_indices;
			bool m_use_restart_index;
			bool m_static_indices;
			bool m_static_vertices;
			bool m_static_instances;
			const bool m_use_simd;

		protected:
			/**
			 * Default constructor.
			 */
			AbstractMesh(const String &name,
				const bool static_indices,
				const bool static_vertices,
				const bool static_instances,
				const bool use_simd);

			void copy_data(AbstractMesh &mesh) const;
			virtual void init_vertex_buffers(
				const VertexStreamBitset vertex_buffers) = 0;
			virtual void init_index_buffer() = 0;

			inline GLenum get_index_type() const noexcept
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
				const noexcept
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
			virtual ~AbstractMesh() noexcept;

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
			virtual void draw(const MeshDrawData &draw_data,
				const Uint32 instances,
				const PrimitiveType primitive) = 0;

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
				const bool shared_index_data) const = 0;

			/**
			 * Returns true if the mesh supports restart index
			 * (e.g. OpenGL 3.1)
			 */
			virtual bool get_supports_restart_index() const = 0;

			/**
			 * Returns true if the mesh supports base vertex
			 * (e.g. OpenGL 3.2)
			 */
			virtual bool get_supports_base_vertex() const = 0;

			/**
			 * Returns true if the mesh supports vertex attribute
			 * divisor (e.g. OpenGL 3.3)
			 */
			virtual bool get_supports_vertex_attribute_divisor()
				const = 0;

			/**
			 * Draws the sub mesh with the given index.
			 */
			void draw(const Uint32 index, const Uint32 instances);

			void init(const VertexFormatSharedPtr &vertex_format,
				const MeshDataToolSharedPtr &source,
				const bool static_indices = true,
				const bool static_vertices = true,
				const bool static_instances = true);

			void init(const VertexFormatSharedPtr &vertex_format,
				const Uint32 index_count,
				const Uint32 vertex_count,
				const Uint32 instance_count,
				const bool use_16_bit_indices,
				const bool static_indices = true,
				const bool static_vertices = true,
				const bool static_instances = true);

			void update_vertex_buffer(
				const AbstractReadMemorySharedPtr &buffer,
				const Uint16 index);

			void update_index_buffer(
				const AbstractReadMemorySharedPtr &buffer);

			virtual AbstractWriteMemorySharedPtr
				get_vertex_buffer(const Uint16 index) = 0;

			VertexStreamSharedPtr get_vertex_stream(
				const Uint16 index);

			virtual AbstractWriteMemorySharedPtr get_index_buffer()
				= 0;

			void get_bounding_box(
				const Transformation &transformation,
				BoundingBox &bounding_box);

			inline const VertexFormatSharedPtr &get_vertex_format()
				const noexcept
			{
				return m_vertex_format;
			}

			VertexStreamBitset get_used_vertex_buffers() const;

			/**
			 */
			const VertexElements &get_vertex_elements(
				const Uint16 index) const;

			void set_sub_meshs(const SubMeshVector &sub_meshs);

			/**
			 * Sets the min/max boxes.
			 * @param min_max_boxes The new min/max boxes.
			 */
			inline void set_min_max_boxes(
				const AlignedShort8Array &min_max_boxes)
			{
				m_min_max_boxes = min_max_boxes;
			}

			/**
			 * Sets if the restart index is used.
			 * @param use_restart_index True if the restart index
			 * is used
			 */
			inline void set_use_restart_index(
				const bool use_restart_index)
			{
				assert(!use_restart_index ||
					get_supports_restart_index());

				m_use_restart_index = use_restart_index;
			}

			/**
			 * Returns the sub meshs.
			 * @result The sub meshs.
			 */
			inline const SubMeshVector &get_sub_meshs() const
				noexcept
			{
				return m_sub_meshs;
			}

			/**
			 * Returns the min/max boxes.
			 * @result The min/max boxes.
			 */
			inline const AlignedShort8Array &get_min_max_boxes()
				const noexcept
			{
				return m_min_max_boxes;
			}

			/**
			 * Returns the number of vertexes.
			 * @result The vertex count.
			 */
			inline Uint32 get_vertex_count() const noexcept
			{
				return m_vertex_count;
			}

			/**
			 * Returns the number of indexes.
			 * @result The index count.
			 */
			inline Uint32 get_index_count() const noexcept
			{
				return m_index_count;
			}

			/**
			 * Returns the number of instances.
			 * @result The instance count.
			 */
			inline Uint32 get_instance_count() const noexcept
			{
				return m_instance_count;
			}

			/**
			 * Sets the OpenGL primitive type (points, lines, ..)
			 */
			inline void set_primitive(const PrimitiveType primitive)
				noexcept
			{
				m_primitive = primitive;
			}

			/**
			 * Returns the OpenGL primitive type (points, lines, ..)
			 */
			inline PrimitiveType get_primitive() const noexcept
			{
				return m_primitive;
			}

			/**
			 * Returns true if 16 bit indices are used.
			 */
			inline bool get_use_16_bit_indices() const noexcept
			{
				return m_use_16_bit_indices;
			}

			/**
			 * Return the restart index.
			 * @result The restart index.
			 */
			inline Uint32 get_restart_index() const noexcept
			{
				if (get_use_16_bit_indices() &&
					get_use_restart_index())
				{
					return std::numeric_limits<
						Uint16>::max();
				}
				else
				{
					return std::numeric_limits<
						Uint32>::max();
				}
			}

			/**
			 * Returns if the restart index is used.
			 * @result If the restart index is used.
			 */
			inline bool get_use_restart_index() const noexcept
			{
				return m_use_restart_index;
			}

			/**
			 * Returns if the indices are mostly static.
			 * @result If the indices are mostly static.
			 */
			inline bool get_static_indices() const noexcept
			{
				return m_static_indices;
			}

			/**
			 * Returns if the vertices are mostly static.
			 * @result If the vertices are mostly static.
			 */
			inline bool get_static_vertices() const noexcept
			{
				return m_static_vertices;
			}

			/**
			 * Returns if the instanced vertices are mostly static.
			 * @result If the instanced vertices are mostly static.
			 */
			inline bool get_static_instances() const noexcept
			{
				return m_static_instances;
			}

			/**
			 */
			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			/**
			 * Using SIMD (SSE/SSE2) instruction.
			 * @result Using SIMD (SSE/SSE2) instruction.
			 */
			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

	};

}

#endif	/* UUID_e4486b52_0882_4250_9e47_1c776906bf13 */
