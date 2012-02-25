/****************************************************************************
 *            loddata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "loddata.hpp"

namespace eternal_lands
{

	LodData::LodData(const Uint16 count)
	{
		Uint16Array2 data;
		Uint16 i;

		for (i = 0; i < count; ++i)
		{
			data[0] = i;
			data[1] = i;

			m_lods.push_back(data);
		}

		for (i = 0; i < 4; ++i)
		{
			m_lods_counts[i] = count;
		}

		for (i = 0; i < 4; ++i)
		{
			m_lods_offsets[i] = 0;
		}

		for (i = 0; i < 3; ++i)
		{
			m_lods_distances[i] =
				std::numeric_limits<Uint16>::max();
		}
	}

	LodData::LodData(const Uint16Array2Vector &lods,
		const Uint16Array4 &lods_counts,
		const Uint16Array4 &lods_offsets,
		const Uint16Array3 &lods_distances): m_lods(lods),
		m_lods_counts(lods_counts), m_lods_offsets(lods_offsets),
		m_lods_distances(lods_distances)
	{
	}

	LodData::~LodData() throw()
	{
	}

}
