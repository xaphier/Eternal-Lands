/****************************************************************************
 *            materiallock.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_dbe2afc9_afa1_45b9_badd_a8196e9d97f5
#define	UUID_dbe2afc9_afa1_45b9_badd_a8196e9d97f5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "material.hpp"

namespace eternal_lands
{

	class MaterialLock
	{
		private:
			const MaterialConstSharedPtr m_material;

		public:
			inline MaterialLock(
				const MaterialConstSharedPtr &material):
				m_material(material)
			{
				m_material->lock();
			}

			inline ~MaterialLock() noexcept
			{
				m_material->unlock();
			}

			inline const MaterialConstSharedPtr &operator->() const
				noexcept
			{
				return m_material;
			}

			inline const MaterialConstSharedPtr &get_material()
				const noexcept
			{
				return m_material;
			}

	};

}

#endif	/* UUID_dbe2afc9_afa1_45b9_badd_a8196e9d97f5 */
