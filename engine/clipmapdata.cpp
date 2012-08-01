/****************************************************************************
 *            clipmapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "clipmapdata.hpp"

namespace eternal_lands
{

	ClipmapData::ClipmapData()
	{
	}

	ClipmapData::~ClipmapData() noexcept
	{
	}

	Uint16 ClipmapData::get_albedo_count() const
	{
		Uint16 i;

		for (i = 0; i < m_albedo_maps.size(); ++i)
		{
			if (get_albedo_map(i).get().empty())
			{
				return i;
			}
		}

		return m_albedo_maps.size();
	}

	Uint16 ClipmapData::get_normal_count() const
	{
		Uint16 i;

		for (i = 0; i < m_normal_maps.size(); ++i)
		{
			if (get_normal_map(i).get().empty())
			{
				return i;
			}
		}

		return m_normal_maps.size();
	}

	Uint16 ClipmapData::get_specular_count() const
	{
		Uint16 i;

		for (i = 0; i < m_specular_maps.size(); ++i)
		{
			if (get_specular_map(i).get().empty())
			{
				return i;
			}
		}

		return m_specular_maps.size();
	}

	Uint16 ClipmapData::get_blend_count() const
	{
		Uint16 i;

		for (i = 0; i < m_blend_images.size(); ++i)
		{
			if (get_blend_image(i).get() != nullptr)
			{
				return i;
			}
		}

		return m_blend_images.size();
	}

}
