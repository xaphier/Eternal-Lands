/****************************************************************************
 *            meshdrawdata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b3ced261_24a1_4af4_8f0f_f8871c9bb6cd
#define	UUID_b3ced261_24a1_4af4_8f0f_f8871c9bb6cd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MeshDrawData.
 * This file contains the @c class MeshDrawData.
 */
namespace eternal_lands
{

	class MeshDrawData
	{
		private:
			Uint32 m_offset;
			Uint32 m_count;
			Uint32 m_min_vertex;
			Uint32 m_max_vertex;
			Uint32 m_base_vertex;

		public:
			MeshDrawData();
			MeshDrawData(const Uint32 offset, const Uint32 count,
				const Uint32 min_vertex,
				const Uint32 max_vertex,
				const Uint32 base_vertex = 0);
			~MeshDrawData() throw();

			inline Uint32 get_offset() const
			{
				return m_offset;
			}

			inline Uint32 get_count() const
			{
				return m_count;
			}

			inline Uint32 get_min_vertex() const
			{
				return m_min_vertex;
			}

			inline Uint32 get_max_vertex() const
			{
				return m_max_vertex;
			}

			inline Uint32 get_base_vertex() const
			{
				return m_base_vertex;
			}

			inline void set_offset(const Uint32 offset)
			{
				m_offset = offset;
			}

			inline void set_count(const Uint32 count)
			{
				m_count = count;
			}

			inline void set_min_vertex(const Uint32 min_vertex)
			{
				m_min_vertex = min_vertex;
			}

			inline void set_max_vertex(const Uint32 max_vertex)
			{
				m_max_vertex = max_vertex;
			}

			inline void set_base_vertex(const Uint32 base_vertex)
			{
				m_base_vertex = base_vertex;
			}

	};

	OutStream& operator<<(OutStream &str, const MeshDrawData &value);

}

#endif	/* UUID_b3ced261_24a1_4af4_8f0f_f8871c9bb6cd */
