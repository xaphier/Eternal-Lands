/****************************************************************************
 *            blendmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blendmodification.hpp"
#include "../editormapdata.hpp"
#include "image.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareImageValueIndex
		{
			public:
				inline bool operator() (
					const ImageValue &image_value1,
					const ImageValue &image_value2) const
				{
					return image_value1.get_index() <
						image_value2.get_index();
				}

		};

	}

	BlendModification::BlendModification(
		const ImageValueVector &blend_values, const Uint32 edit_id):
		Modification(edit_id, 0, mt_blend_values_changed),
		m_blend_values(blend_values)
	{
	}

	BlendModification::~BlendModification() throw()
	{
	}

	bool BlendModification::do_merge(Modification* modification)
	{
		BlendModification* blend_modification;
		ImageValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		blend_modification = boost::polymorphic_downcast<
			BlendModification*>(modification);

		assert(blend_modification != 0);

		size = m_blend_values.size();
		begin = m_blend_values.begin();
		end = begin + size;

		std::sort(begin, end, CompareImageValueIndex());

		BOOST_FOREACH(const ImageValue &image_value,
			blend_modification->m_blend_values)
		{
			found = std::binary_search(begin, end, image_value,
				CompareImageValueIndex());

			if (!found)
			{
				m_blend_values.push_back(image_value);
				begin = m_blend_values.begin();
				end = begin + size;
			}
		}

		return true;
	}

	bool BlendModification::undo(EditorMapData &editor)
	{
/*
		BOOST_FOREACH(const ImageValue &value, m_blend_values)
		{
			m_blend_image->set_pixel(value.get_x(), value.get_y(),
				0, 0, 0, value.get_value());
		}
*/
		return false;
	}

}
