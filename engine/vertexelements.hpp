/****************************************************************************
 *            vertexelements.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed7598a4_93da_4be9_97bd_db2dd1b4a8aa
#define	UUID_ed7598a4_93da_4be9_97bd_db2dd1b4a8aa

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelement.hpp"

/**
 * @file
 * @brief The @c class VertexElements.
 * This file contains the @c class VertexElements.
 */
namespace eternal_lands
{

	typedef std::map<VertexSemanticType, VertexElementType>
		VertexDescriptionMap;

	/**
	 * @brief This class describes vertex elements.
	 *
	 * Vertex elements are defined throught threis semantics, types and
	 * offsets. They all share one stride valua and a divisor. The stride
	 * value are the bytes between every vertex set in the buffer, the
	 * divisor is only needed for instancing.
	 */
	class VertexElements
	{
		private:
			VertexElementVector m_vertex_elements;
			Uint32 m_stride;
			Uint16 m_divisor;

		public:
			/**
			 * @brief Default constructor.
			 * Default constructor initializes the vertex elements.
			 */
			VertexElements();

			/**
			 * @brief Default constructor.
			 * Default constructor initializes the vertex elements.
			 * @param vertex_elements The vertex elements to use.
			 * @param divisor The divisor for the vertex elements.
			 * @param stride The stride of the vertex elements.
			 */
			VertexElements(
				const VertexElementVector &vertex_elements,
				const Uint16 stride, const Uint16 divisor = 0);

			/**
			 * @brief Default constructor.
			 * Default constructor initializes the vertex elements.
			 * @param vertex_elements The vertex elements to use.
			 * @param divisor The divisor for the vertex elements.
			 */
			VertexElements(
				const VertexDescriptionMap &vertex_elements,
				const Uint16 divisor = 0);

			/**
			 * Default destructor.
			 */
			~VertexElements() noexcept;

			/**
			 * Returns the vertex elements.
			 * @return The vertex elements.
			 */
			inline const VertexElementVector &get_vertex_elements()
				const noexcept
			{
				return m_vertex_elements;
			}

			/**
			 * Returns the divisor.
			 * @return The divisor.
			 */
			inline Uint16 get_divisor() const noexcept
			{
				return m_divisor;
			}

			/**
			 * Returns the stride.
			 * @return The stride.
			 */
			inline Uint16 get_stride() const noexcept
			{
				return m_stride;
			}

			/**
			 * Returns the stride.
			 * @return The stride.
			 */
			inline Uint16 get_count() const noexcept
			{
				return m_vertex_elements.size();
			}

			/**
			 * Returns the semantic of the vertex element.
			 * @return The semantic of the vertex element.
			 */
			inline VertexSemanticType get_semantic(
				const Uint16 index) const
			{
				return m_vertex_elements[index].get_semantic();
			}

			/**
			 * Returns the type of the vertex element.
			 * @return The type of the vertex element.
			 */
			inline VertexElementType get_type(const Uint16 index)
				const
			{
				return m_vertex_elements[index].get_type();
			}

			/**
			 * Returns the offset of the vertex element in the
			 * vertex buffer.
			 * @return The offset of the vertex element.
			 */
			inline Uint32 get_offset(const Uint16 index) const
			{
				return m_vertex_elements[index].get_offset();
			}

			/**
			 * Returns the count of values the vertex element
			 * contains.
			 * @return The count of values the vertex element
			 * contains.
			 */
			inline Uint32 get_count(const Uint16 index) const
			{
				return m_vertex_elements[index].get_count();
			}

			/**
			 * Returns the size of the vertex element in bytes.
			 * @return The size of the vertex element in bytes.
			 */
			inline Uint32 get_size(const Uint16 index) const
			{
				return m_vertex_elements[index].get_size();
			}

			/**
			 * Returns the OpenGL tpye of the vertex element.
			 * @return The OpenGL tpye of the vertex element.
			 */
			inline GLenum get_gl_type(const Uint16 index) const
			{
				return m_vertex_elements[index].get_gl_type();
			}

			/**
			 * Returns true if the signed integer values get scaled
			 * so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. False otherwise.
			 * @return True if normalization is used, else false.
			 */
			inline bool get_normalized(const Uint16 index) const
			{
				return m_vertex_elements[
					index].get_normalized();
			}

			/**
			 * Returns GL_TRUE if the signed integer values get
			 * scaled so they fit into -1.0 .. 1.0 range and if the
			 * unsigned inter values get scaled so they fit into
			 * the 0.0 .. 1.0 range. GL_FALSE otherwise.
			 * @return GL_TRUE if normalization is used, else
			 * GL_FALSE.
			 */
			inline GLboolean get_gl_normalized(const Uint16 index)
				const
			{
				return m_vertex_elements[
					index].get_gl_normalized();
			}

	};

}

#endif	/* UUID_ed7598a4_93da_4be9_97bd_db2dd1b4a8aa */
