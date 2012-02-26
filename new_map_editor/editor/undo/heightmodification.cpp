/****************************************************************************
 *            heightmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "heightmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	class CompareHeightIndex
	{
		public:
			inline bool operator() (const Height &height1,
				const Height &height2) const
			{
				return height1.get_index() <
					height2.get_index();
			}

	};

	HeightModification::HeightModification(const HeightVector &heights,
		const Uint16 page_id, const bool terrain): m_heights(heights),
		m_page_id(page_id), m_terrain(terrain)
	{
	}

	HeightModification::~HeightModification()
	{
	}

	ModificationType HeightModification::get_type() const
	{
		if (m_terrain)
		{
			return mt_terrain_height_changed;
		}
		else
		{
			return mt_height_changed;
		}
	}

	bool HeightModification::merge(Modification* modification)
	{
		HeightModification* height_modification;
		HeightVector::iterator begin, end;
		Uint32 size;
		bool found;

		if (get_type() == modification->get_type())
		{
			height_modification = dynamic_cast<
				HeightModification*>(modification);

			assert(height_modification != 0);

			if (height_modification->m_page_id == m_page_id)
			{
				size = m_heights.size();
				begin = m_heights.begin();
				end = begin + size;

				std::sort(begin, end, CompareHeightIndex());

				BOOST_FOREACH(const Height &height,
					height_modification->m_heights)
				{
					found = std::binary_search(begin, end,
						height, CompareHeightIndex());

					if (!found)
					{
						m_heights.push_back(height);
						begin = m_heights.begin();
						end = begin + size;
					}
				}

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool HeightModification::undo(EditorMapData &editor)
	{
		if (m_terrain)
		{
			editor.set_terrain_heights(m_heights);
		}
		else
		{
			editor.set_heights(m_heights);
		}

		return false;
	}

}
