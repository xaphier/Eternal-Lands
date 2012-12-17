/****************************************************************************
 *            meshdatatool.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "alignedarrays.hpp"

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
			typedef std::map<VertexSemanticType, AlignedVec4Array>
				VertexSemanticTypeAlignedVec4ArrayMap;

			SubMeshVector m_sub_meshs;
			VertexSemanticTypeAlignedVec4ArrayMap m_vertices;
			AlignedSint16Vec8Array m_min_max_boxes;
			Uint32Vector m_indices;
			String m_name;
			Uint32 m_vertex_count;
			PrimitiveType m_primitive;
			bool m_use_restart_index;
			const bool m_use_simd;

			void vertex_optimize();

			inline Uint32 get_semantic_count() const
			{
				return m_vertices.size();
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
			MeshDataTool(const String &name,
				const Uint32 vertex_count,
				const Uint32 index_count,
				const Uint32 sub_mesh_count,
				const VertexSemanticTypeSet &semantics,
				const PrimitiveType primitive,
				const bool use_restart_index,
				const bool use_simd);

			/**
			 * Default destructor.
			 */
			~MeshDataTool() noexcept;
			static glm::vec4
				get_gram_schmidth_orthogonalize_tangent(
					const glm::vec3 &normal,
					const glm::vec3 &tangent,
					const glm::vec3 &bitangent);
			void update_sub_meshs_packed();
			void update_sub_meshs_bounding_box();
			void build_tangent(const bool morph_target,
				const bool gram_schmidth_orthogonalize);
			void build_normal(const bool morph_target);
			void build_scale_morph(const float scale = 1.0f);
			void optimize();
			void get_bounding_box(
				const Transformation &transformation,
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
			void copy_vertics(const MeshDataTool &mesh_data_tool,
				const VertexSemanticType semantic,
				const Uint32 source_index,
				const Uint32 dest_index, const Uint32 count);
			void fill_vertics(const VertexSemanticType semantic,
				const Uint32 index, const Uint32 count,
				const glm::vec4 &data);
			void transform_vertics(
				const MeshDataTool &mesh_data_tool,
				const VertexSemanticType semantic,
				const Uint32 source_index,
				const Uint32 dest_index, const Uint32 count,
				const glm::mat4x3 &matrix);
			void transform_vertics(
				const MeshDataTool &mesh_data_tool,
				const VertexSemanticType semantic,
				const Uint32 source_index,
				const Uint32 dest_index, const Uint32 count,
				const glm::mat3x3 &matrix);
			void transform_vertics(
				const MeshDataTool &mesh_data_tool,
				const VertexSemanticType semantic,
				const Uint32 source_index,
				const Uint32 dest_index, const Uint32 count,
				const glm::vec4 &scale_offset);
			void set_indices(const Uint32Vector &indices,
				const Uint32 source_index,
				const Uint32 dest_index, const Uint32 count,
				const Sint32 offset);
			void resize_vertices(const Uint32 vertex_count);
			void build_quad_map(const Uint16MultiArray2 &types,
				const glm::vec2 &uv_scale,
				const glm::vec2 &quad_size);

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
				assert(index < m_indices.size());
				assert(get_index_count() == m_indices.size());

				m_indices[index] = data;

				assert(m_indices[index] == data);
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

			void enable_use_base_vertex();
			void disable_use_base_vertex();
			bool get_use_base_vertex() const noexcept;
			bool get_16_bit_indices() const noexcept;

			/**
			 * Returns the number of sub meshs.
			 * @result The sub mesh count.
			 */
			Uint32 get_sub_mesh_count() const;

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

			inline const Uint32Vector &get_indices() const noexcept
			{
				return m_indices;
			}

			/**
			 * Write the data to the vertex stream.
			 * @param stream The vertex stream to use.
			 */
			void write_vertex_stream(VertexStream &stream) const;

			/**
			 * Write the data to the vertex stream.
			 * @param stream The vertex stream to use.
			 */
			void write_vertex_streams(VertexStreams &streams) const;

			/**
			 * Write the data to the index buffer.
			 * @param use_16_bit_indices True if 16 bit indices are
			 * used.
			 * @param index_stream The index buffer to use.
			 */
			void write_index_buffer(
				const bool use_16_bit_indices,
				const AbstractWriteMemorySharedPtr &buffer)
				const;

			/**
			 * Build the min/max boxes from the sub mesh data.
			 */
			void build_min_max_boxes();

			/**
			 * Sets the min/max boxes.
			 * @param min_max_boxes The new min/max boxes.
			 */
			inline void set_min_max_boxes(
				const AlignedSint16Vec8Array &min_max_boxes)
			{
				m_min_max_boxes = min_max_boxes;
			}

			/**
			 * Returns the min/max boxes.
			 * @result The min/max boxes.
			 */
			inline const AlignedSint16Vec8Array &get_min_max_boxes()
				const noexcept
			{
				return m_min_max_boxes;
			}

			/**
			 * Returns the number of vertices.
			 * @result The vertex count.
			 */
			inline Uint32 get_vertex_count() const noexcept
			{
				return m_vertex_count;
			}

			/**
			 * Returns the number of indices.
			 * @result The index count.
			 */
			inline Uint32 get_index_count() const noexcept
			{
				return m_indices.size();
			}

			/**
			 * Return the primitive type.
			 * @result The primitive type.
			 */
			inline PrimitiveType get_primitive() const noexcept
			{
				return m_primitive;
			}

			/**
			 * Return the restart index.
			 * @result The restart index.
			 */
			inline Uint32 get_restart_index() const noexcept
			{
				return std::numeric_limits<Uint32>::max();
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
			 * Returns the sub meshs.
			 * @result The sub meshs.
			 */
			inline const SubMeshVector &get_sub_meshs() const
				noexcept
			{
				return m_sub_meshs;
			}

			/**
			 * Using SIMD (SSE/SSE2) instruction for
			 * transformations, conversions etc.
			 * @result Using SIMD (SSE/SSE2) instruction.
			 */
			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

			/**
			 * Returns the name.
			 * @result The name.
			 */
			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_ea879c22_9289_40a2_8984_ef65232243b0 */
