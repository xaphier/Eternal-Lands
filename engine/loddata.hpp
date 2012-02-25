/****************************************************************************
 *            loddata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0ae1f5f5_26a6_4a7e_abf1_20d8872373b6
#define	UUID_0ae1f5f5_26a6_4a7e_abf1_20d8872373b6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class LodData.
 * This file contains the @c class LodData.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for lod data.
	 *
	 * @c class for lod data.
	 */
	class LodData
	{
		private:
			Uint16Array2Vector m_lods;
			Uint16Array4 m_lods_counts;
			Uint16Array4 m_lods_offsets;
			Uint16Array3 m_lods_distances;

		public:
			/**
			 * Default constructor.
			 */
			LodData(const Uint16 count);

			/**
			 * Default constructor.
			 */
			LodData(const Uint16Array2Vector &lods,
				const Uint16Array4 &lods_counts,
				const Uint16Array4 &lods_offsets,
				const Uint16Array3 &lods_distances);

			/**
			 * Default destructor.
			 */
			~LodData() throw();

			inline Uint16 get_lod(const Uint16 distance) const
			{
				if (distance < m_lods_distances[0])
				{
					return 0;
				}

				if (distance < m_lods_distances[1])
				{
					return 1;
				}

				if (distance < m_lods_distances[2])
				{
					return 2;
				}

				return 3;
			}

			inline Uint16 get_lods_count(const Uint16 lod) const
			{
				return m_lods_counts[lod];
			}

			inline Uint16 get_lods_offset(const Uint16 lod) const
			{
				return m_lods_offsets[lod];
			}

			inline Uint16 get_material_effects_index(
				const Uint16 index) const
			{
				return m_lods[index][0];
			}

			inline Uint16 get_mesh_index(const Uint16 index) const
			{
				return m_lods[index][1];
			}

			inline Uint16 get_material_effects_index(
				const Uint16 lod, const Uint16 index) const
			{
				return m_lods[m_lods_offsets[lod] + index][0];
			}

			inline Uint16 get_mesh_index(const Uint16 lod,
				const Uint16 index) const
			{
				return m_lods[m_lods_offsets[lod] + index][1];
			}

	};

}

#endif	/* UUID_0ae1f5f5_26a6_4a7e_abf1_20d8872373b6 */
