/****************************************************************************
 *            vertexelement.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_391f1863_20f2_46a8_af7d_4cd5b6ffa619
#define	UUID_391f1863_20f2_46a8_af7d_4cd5b6ffa619

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "packtool.hpp"

/**
 * @file
 * @brief The @c class VertexElement.
 * This file contains the @c class VertexElement.
 */
namespace eternal_lands
{

	/**
	 * Different vertex element semantic types.
	 * @{
	 */
	enum VertexSemanticType
	{
		vst_position = 0,
		vst_normal = 1,
		vst_tangent = 2,
		vst_color = 3,
		vst_bone_weight = 4,
		vst_bone_index = 5,
		vst_extra_bone_weight = 6,
		vst_extra_bone_index = 7,
		vst_texture_coordinate = 8,
		vst_morph_position = 9,
		vst_morph_normal = 10,
		vst_morph_tangent = 11,
		vst_morph_texture_coordinate = 12,
		vst_world_matrix_0 = 13,
		vst_world_matrix_1 = 14,
		vst_world_matrix_2 = 15
	};
	/**
	 * @}
	 */

	/**
	 * @enum VertexElementType
	 * Types of vertex elements.
	 * @{
	 */
	enum VertexElementType
	{
		/**
		 * One float
		 */
		vet_float1 = 0,
		/**
		 * Two floats
		 */
		vet_float2,
		/**
		 * Three floats
		 */
		vet_float3,
		/**
		 * Four floats
		 */
		vet_float4,
		/**
		 * Two half floats
		 */
		vet_half2,
		/**
		 * Four half floats
		 */
		vet_half4,
		/**
		 * Two signed shorts
		 */
		vet_short2,
		/**
		 * Two signed shorts
		 */
		vet_short2_normalized,
		/**
		 * Four signed shorts
		 */
		vet_short4,
		/**
		 * Four signed shorts
		 */
		vet_short4_normalized,
		/**
		 * Two unsigned shorts
		 */
		vet_ushort2,
		/**
		 * Two unsigned shorts
		 */
		vet_ushort2_normalized,
		/**
		 * Four unsigned shorts
		 */
		vet_ushort4,
		/**
		 * Four unsigned shorts
		 */
		vet_ushort4_normalized,
		/**
		 * Four signed bytes
		 */
		vet_byte4,
		/**
		 * Four signed bytes
		 */
		vet_byte4_normalized,
		/**
		 * Four unsigned bytes
		 */
		vet_ubyte4,
		/**
		 * Four unsigned bytes
		 */
		vet_ubyte4_normalized,
		/**
		 * Three 10 bit signed integers and one two bit signed integer.
		 */
		vet_signed_xyz10_w2,
		/**
		 * Three 10 bit signed integers and one two bit signed integer.
		 */
		vet_signed_xyz10_w2_normalized,
		/**
		 * Three 10 bit unsigned integers and one two bit unsigned
		 * integer.
		 */
		vet_unsigned_xyz10_w2,
		/**
		 * Three 10 bit unsigned integers and one two bit unsigned
		 * integer.
		 */
		vet_unsigned_xyz10_w2_normalized
	};
	/**
	 * @}
	 */

	typedef std::set<VertexSemanticType> VertexSemanticTypeSet;

	/**
	 * @brief This class describes a vertex element.
	 *
	 * A vertex element is defined throught its semantic, type and offset.
	 * The semantic defines what the data is, like position, normal
	 * etc. The type defines if the data are floats, shorts etc. The offset
	 * is used for accese of these data in the buffer.
	 */
	class VertexElement
	{
		private:
			VertexSemanticType m_semantic;
			VertexElementType m_type;
			PackFormatType m_pack_format;
			GLenum m_gl_type;
			Uint16 m_offset;
			Uint16 m_count;
			Uint16 m_size;
			GLboolean m_gl_normalized;

		public:
			/**
			 * @brief Default constructor.
			 * Default constructor initializes the vertex element.
			 * @param semantic The semantic for the vertex element.
			 * @param type The type for the vertex element.
			 * @param normalize_integer True for normalized,
			 * false else.
			 */
			VertexElement(const VertexSemanticType semantic,
				const VertexElementType type,
				const Uint16 offset);

			/**
			 * Default destructor.
			 */
			~VertexElement() noexcept;

			/**
			 * Returns the semantic of the vertex element.
			 * @return The semantic of the vertex element.
			 */
			inline VertexSemanticType get_semantic() const noexcept
			{
				return m_semantic;
			}

			/**
			 * Returns the type of the vertex element.
			 * @return The type of the vertex element.
			 */
			inline VertexElementType get_type() const noexcept
			{
				return m_type;
			}

			/**
			 * Returns the offset of the vertex element in the
			 * vertex buffer.
			 * @return The offset of the vertex element.
			 */
			inline Uint32 get_offset() const noexcept
			{
				return m_offset;
			}

			/**
			 * Returns the count of values the vertex element
			 * contains.
			 * @return The count of values the vertex element
			 * contains.
			 */
			inline Uint32 get_count() const noexcept
			{
				return m_count;
			}

			/**
			 * Returns the size of the vertex element (in bytes).
			 * @return The size of the vertex element (in bytes).
			 */
			inline Uint32 get_size() const noexcept
			{
				return m_size;
			}

			/**
			 * Returns the OpenGL tpye of the vertex element.
			 * @return The OpenGL tpye of the vertex element.
			 */
			inline GLenum get_gl_type() const noexcept
			{
				return m_gl_type;
			}

			/**
			 * Returns true if the signed integer values get scaled
			 * so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. False otherwise.
			 * @return True if normalization is used, else false.
			 */
			inline bool get_normalized() const noexcept
			{
				return m_gl_normalized == GL_TRUE;
			}

			/**
			 * Returns GL_TRUE if the signed integer values get
			 * scaled so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. GL_FALSE otherwise.
			 * @return GL_TRUE if normalization is used, else
			 * GL_FALSE.
			 */
			inline GLboolean get_gl_normalized() const noexcept
			{
				return m_gl_normalized;
			}

			/**
			 * Returns the pack format of the vertex element type.
			 * @return The pack format of the vertex element type.
			 */
			inline PackFormatType get_pack_format() const noexcept
			{
				return m_pack_format;
			}

			/**
			 * Returns the string representation of the given enum
			 * value.
			 * @param vertex_element The enum to use.
			 * @return The string representation of the given enum
			 * value.
			 */
			static const String &get_str(
				const VertexElementType vertex_element);

			/**
			 * Returns the string representation of the given enum
			 * value.
			 * @param vertex_semantic_type The enum to use.
			 * @return The string representation of the given enum
			 * value.
			 */
			static const String &get_str(
				const VertexSemanticType vertex_semantic_type);

			/**
			 * Returns the vertex element type represented by the
			 * string.
			 * @param str The string to use.
			 * @return The vertex element type represented by the
			 * string
			 */
			static VertexElementType get_vertex_element(
				const String &str);

			/**
			 * Returns the vertex semantic type represented by the
			 * string.
			 * @param str The string to use.
			 * @return The vertex semantic type represented by the
			 * string
			 */
			static VertexSemanticType get_vertex_semantic(
				const String &str);

			/**
			 * Returns the size of the vertex element type in bytes.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return The size of the vertex element type in bytes.
			 */
			static Uint32 get_size(
				const VertexElementType vertex_element);

			/**
			 * Returns the element count that the vertex element
			 * type contains.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return The count of floats/shorts/bytes the vertex
			 * element type contains.
			 */
			static Uint32 get_count(
				const VertexElementType vertex_element);

			/**
			 * Returns true if the elements of the given type are
			 * signed, false else.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return True if its a signed type, false else.
			 */
			static bool get_signed_elements(
				const VertexElementType vertex_element);

			/**
			 * Returns true if its a type with dynamic range, false
			 * else.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return True if its a type with dynamic range, false
			 * else.
			 */
			static bool get_dynamic_range(
				const VertexElementType vertex_element);

			/**
			 * Returns the OpenGL tpye of the vertex element type.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return The OpenGL tpye of the vertex element type.
			 */
			static GLenum get_gl_type(
				const VertexElementType vertex_element);

			/**
			 * Returns GL_TRUE if the signed integer values get
			 * scaled so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. GL_FALSE otherwise.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return GL_TRUE if normalization is used, else
			 * GL_FALSE.
			 */
			static GLboolean get_gl_normalized(
				const VertexElementType vertex_element);

			/**
			 * Returns true if the signed integer values get
			 * scaled so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. false otherwise.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return true if normalization is used, else
			 * false.
			 */
			static bool get_normalized(
				const VertexElementType vertex_element);

			/**
			 * Returns true if the vertex element type is supported,
			 * else false.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return True if supported, false else.
			 */
			static bool get_supported(
				const VertexElementType vertex_element);

			/**
			 * Returns true if the string is the name of a vertex
			 * semantic type and sets the vertex_semantic parameter
			 * to the vertex semantic type, else returns false.
			 * @param str The string to use.
			 * @param vertex_semantic The vertex semantic.
			 * @return True if the string is the name of a vertex
			 * semantic type, else false.
			 */
			static bool get_vertex_semantic(const String &str,
				VertexSemanticType &vertex_semantic);

			/**
			 * Returns the pack format of the vertex element type.
			 * @param vertex_element The type for the vertex
			 * element to use.
			 * @return The pack format of the vertex element type.
			 */
			static PackFormatType get_pack_format(
				const VertexElementType vertex_element);

			static Uint32 get_vertex_semantic_count() noexcept;
			static Uint32 get_vertex_element_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const VertexElementType value);
	OutStream& operator<<(OutStream &str, const VertexSemanticType value);
	InStream& operator>>(InStream &str, VertexElementType &value);
	InStream& operator>>(InStream &str, VertexSemanticType &value);

}

#endif	/* UUID_391f1863_20f2_46a8_af7d_4cd5b6ffa619 */
