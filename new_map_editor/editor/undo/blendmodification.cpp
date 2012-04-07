/****************************************************************************
 *            blendmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		const ImageValueVector &blend_values,
		const ImageSharedPtr &blend_image, const Uint16 id):
		m_blend_values(blend_values), m_blend_image(blend_image),
		m_id(id)
	{
	}

	BlendModification::~BlendModification() throw()
	{
	}

	ModificationType BlendModification::get_type() const
	{
		return mt_blend_values_changed;
	}

	bool BlendModification::merge(Modification* modification)
	{
		BlendModification* blend_modification;
		ImageValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		if (get_type() == modification->get_type())
		{
			blend_modification = dynamic_cast<BlendModification*>(
				modification);

			assert(blend_modification != 0);

			if (blend_modification->m_id == m_id)
			{
				size = m_blend_values.size();
				begin = m_blend_values.begin();
				end = begin + size;

				std::sort(begin, end, CompareImageValueIndex());

				BOOST_FOREACH(const ImageValue &image_value,
					blend_modification->m_blend_values)
				{
					found = std::binary_search(begin, end,
						image_value,
						CompareImageValueIndex());

					if (!found)
					{
						m_blend_values.push_back(
							image_value);
						begin = m_blend_values.begin();
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

	bool BlendModification::undo(EditorMapData &editor)
	{
		BOOST_FOREACH(const ImageValue &value, m_blend_values)
		{
			m_blend_image->set_pixel(value.get_x(), value.get_y(),
				0, 0, 0, value.get_value());
		}

		return false;
	}

}
