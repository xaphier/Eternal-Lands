/****************************************************************************
 *            subobject.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_53c2c46f_d39a_40ef_913e_6bad5143b2eb
#define	UUID_53c2c46f_d39a_40ef_913e_6bad5143b2eb

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "meshdrawdata.hpp"
#include "selectionutil.hpp"

/**
 * @file
 * @brief The @c class SubObject.
 * This file contains the @c class SubObject.
 */
namespace eternal_lands
{

	class SubObject: public MeshDrawData
	{
		private:
			Uint32 m_id;
			Uint32 m_material;
			SelectionType m_selection;

		public:
			/**
			 * Default constructor.
			 */
			SubObject();

			/**
			 * Default constructor.
			 */
			SubObject(const MeshDrawData &mesh_draw,
				const Uint32 id, const Uint32 material,
				SelectionType m_selection);

			/**
			 * Default destructor.
			 */
			~SubObject() throw();

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline Uint32 get_material() const
			{
				return m_material;
			}

			inline SelectionType get_selection() const
			{
				return m_selection;
			}

			inline void set_id(const Uint32 id)
			{
				m_id = id;
			}

			inline void set_material(const Uint32 material)
			{
				m_material = material;
			}

			inline void set_selection(const SelectionType selection)
			{
				m_selection = selection;
			}

	};

	OutStream& operator<<(OutStream &str, const SubObject &value);

}

#endif	/* UUID_53c2c46f_d39a_40ef_913e_6bad5143b2eb */
