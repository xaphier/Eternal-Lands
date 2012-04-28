/****************************************************************************
 *            vertexformat.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_40fee5ef_43a7_4df0_a770_588fb97b5e4a
#define	UUID_40fee5ef_43a7_4df0_a770_588fb97b5e4a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelements.hpp"

/**
 * @file
 * @brief The @c class VertexFormat.
 * This file contains the @c class VertexFormat.
 */
namespace eternal_lands
{

	/**
	 * @brief This class describes the vertex format.
	 * This class describes the vertex format.
	 */
	class VertexFormat: public boost::noncopyable
	{
		private:
			VertexElementsArray m_vertex_elements;
			String m_name;

		public:
			/**
			 * @brief Default constructor.
			 */
			VertexFormat(const String &name,
				const VertexElements &vertex_elements);

			/**
			 * @brief Default constructor.
			 */
			VertexFormat(const String &name,
				const VertexElementsVector &vertex_elements);

			/**
			 * @brief Default constructor.
			 */
			VertexFormat(const String &name,
				const VertexElementsArray &vertex_elements);

			/**
			 * @brief Default destructor.
			 */
			~VertexFormat() throw();

			inline const VertexElementsArray &get_vertex_elements()
				const
			{
				return m_vertex_elements;
			}

			inline const VertexElements &get_vertex_elements(
				const Uint16 index) const
			{
				return m_vertex_elements[index];
			}

			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_40fee5ef_43a7_4df0_a770_588fb97b5e4a */
