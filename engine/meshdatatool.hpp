/****************************************************************************
 *            meshdatatool.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ea879c22_9289_40a2_8984_ef65232243b0
#define	UUID_ea879c22_9289_40a2_8984_ef65232243b0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelement.hpp"
#include "primitiveutil.hpp"

/**
 * @file
 * @brief The @c class MeshDataTool.
 * This file contains the @c class MeshDataTool.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for mesh data.
	 *
	 * @c class for mesh data.
	 */
	class MeshDataTool
	{
		private:
			typedef std::map<VertexSemanticType, Uint32>
				VertexSemanticTypeUint32Map;

			VertexSemanticTypeUint32Map m_semantic_indices;
			SubMeshVector m_sub_meshs;
			Vec4Vector m_vertices;
			Uint32Vector m_indices;
			std::set<VertexSemanticType> m_semantics;
			Uint32 m_vertex_count;
			Uint32 m_restart_index;
			PrimitiveType m_primitive_type;
			bool m_use_restart_index;

			void vertex_optimize();
			void build_vertex_semantic_indices(
				const VertexSemanticTypeSet &semantics);

			inline Uint32 get_semantic_count() const
			{
				return m_semantic_indices.size();
			}

		public:
			/**
			 * @brief Default constructor.
			 * Default constructor initializes the buffers for
			 * vertex and index data.
			 * @param vertex_count The number of vertices.
			 * @param index_count The number of indices.
			 * @param semantics The vertex semantics that are used.
			 */
			MeshDataTool(const Uint32 vertex_count,
				const Uint32 index_count,
				const Uint32 sub_mesh_count,
				const VertexSemanticTypeSet &semantics,
				const Uint32 restart_index,
				const PrimitiveType primitive_type,
				const bool use_restart_index);

			/**
			 * Default destructor.
			 */
			~MeshDataTool() throw();

			void update_sub_meshs_packed();
			void update_sub_meshs_bounding_box();
			void build_tangent(const bool morph_target,
				const bool second_texture_coord,
				const bool gram_schmidth_orthogonalize);
			void build_normal(const bool morph_target);
			void build_scale_morph(const float scale = 1.0f);
			void optimize();
			void get_bounding_box(const glm::mat4x3 &matrix,
				BoundingBox &bounding_box);
			void write_vertex_semantic_to_stream(
				const VertexSemanticType semantic,
				const Uint32 index, OutStream &str);
			void write_to_stream(OutStream &str);

			void set_vertex_data(
				const VertexSemanticType vertex_semantic,
				const Uint32 index, const glm::vec4 &data);

			glm::vec4 get_vertex_data(
				const VertexSemanticType vertex_semantic,
				const Uint32 index) const;

			inline void resize_vertices(const Uint32 vertex_count)
			{
				m_vertex_count = vertex_count;
				m_vertices.resize(vertex_count *
					get_semantic_count());
			}

			inline void resize_indices(const Uint32 index_count)
			{
				m_indices.resize(index_count);
			}

			inline void append_index_data(const Uint32 data)
			{
				m_indices.push_back(data);
			}

			inline void set_index_data(const Uint32 index,
				const Uint32 data)
			{
				assert(index < get_index_count());
				assert((data < get_vertex_count()) ||
					((get_restart_index() == data) &&
						get_use_restart_index()));

				m_indices[index] = data;
			}

			inline Uint32 get_index_data(const Uint32 index) const
			{
				assert(index < get_index_count());

				return m_indices[index];
			}

			void set_sub_mesh_data(const Uint32 index,
				const SubMesh &data);

			const SubMesh &get_sub_mesh_data(
				const Uint32 index) const;

			/**
			 * Disables the use of restart index and converts the
			 * primitive type to pt_triangles when needed.
			 */
			void disable_restart_index();

			/**
			 * Convert the primitive type to pt_triangles.
			 */
			void convert_to_triangles();

			/**
			 * Returns the index data of the given sub mesh. If
			 * needed, the indices are converted if the primitive
			 * type is not pt_triangles.
			 * @param sub_mesh_index The index of the sub mesh to
			 * use.
			 * @param indices The vector were the indices get
			 * appended.
			 */
			void get_triangle_indices(const Uint32 sub_mesh_index,
				Uint32Vector &indices,
				const bool use_base_vertex) const;

			inline const Uint32Vector &get_indices() const
			{
				return m_indices;
			}

			/**
			 * Write the data to the vertex buffer.
			 * @param vertex_stream The vertex buffer to use.
			 */
			void write_vertex_buffer(
				const VertexElements &vertex_elements,
				const AbstractWriteMemoryBufferSharedPtr &buffer)
				const;

			/**
			 * Write the data to the index buffer.
			 * @param use_16_bit_indices True if 16 bit indices are
			 * used.
			 * @param index_stream The index buffer to use.
			 */
			void write_index_buffer(
				const bool use_16_bit_indices,
				const AbstractWriteMemoryBufferSharedPtr &buffer)
				const;

			/**
			 * Returns the number of vertices.
			 * @result The vertex count.
			 */
			inline Uint32 get_vertex_count() const
			{
				return m_vertex_count;
			}

			/**
			 * Returns the number of indices.
			 * @result The index count.
			 */
			inline Uint32 get_index_count() const
			{
				return m_indices.size();
			}

			/**
			 * Return the primitive type.
			 * @result The primitive type.
			 */
			inline PrimitiveType get_primitive_type() const
			{
				return m_primitive_type;
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
			 * Returns the sub meshs.
			 * @result The sub meshs.
			 */
			inline const SubMeshVector &get_sub_meshs() const
			{
				return m_sub_meshs;
			}

	};

}

#endif	/* UUID_ea879c22_9289_40a2_8984_ef65232243b0 */
