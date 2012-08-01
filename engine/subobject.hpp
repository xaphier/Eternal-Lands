/****************************************************************************
 *            subobject.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			float m_max_distance;
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
				const float max_distance,
				const SelectionType selection);

			/**
			 * Default destructor.
			 */
			~SubObject() noexcept;

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline Uint32 get_material() const noexcept
			{
				return m_material;
			}

			inline Uint32 get_max_distance() const noexcept
			{
				return m_max_distance;
			}

			inline SelectionType get_selection() const noexcept
			{
				return m_selection;
			}

			inline void set_id(const Uint32 id) noexcept
			{
				m_id = id;
			}

			inline void set_material(const Uint32 material) noexcept
			{
				m_material = material;
			}

			inline void set_max_distance(const Uint32 max_distance)
				noexcept
			{
				m_max_distance = max_distance;
			}

			inline void set_selection(const SelectionType selection)
				noexcept
			{
				m_selection = selection;
			}

	};

	OutStream& operator<<(OutStream &str, const SubObject &value);

}

#endif	/* UUID_53c2c46f_d39a_40ef_913e_6bad5143b2eb */
