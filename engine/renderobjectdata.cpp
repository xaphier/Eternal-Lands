/****************************************************************************
 *            renderobjectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "renderobjectdata.hpp"
#include "object.hpp"

namespace eternal_lands
{

	RenderObjectData::RenderObjectData(): m_transparency(0.0f),
		m_distance(0.0f), m_sub_frustums_mask(0x1),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(bt_disabled), m_depth_pre_pass(true)
	{
	}

	RenderObjectData::RenderObjectData(const ObjectSharedPtr &object,
		const SubFrustumsMask sub_frustums_mask): m_object(object),
		m_transparency(0.0f), m_distance(0.0f),
		m_sub_frustums_mask(sub_frustums_mask),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(bt_disabled), m_depth_pre_pass(true)
	{
		m_transparency = object->get_transparency();
		m_blend = object->get_blend();
	}

	RenderObjectData::RenderObjectData(const ObjectSharedPtr &object,
		const float transparency, const BlendType blend,
		const SubFrustumsMask sub_frustums_mask): m_object(object),
		m_transparency(transparency), m_distance(0.0f),
		m_sub_frustums_mask(sub_frustums_mask),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(blend)
	{
	}

	RenderObjectData::~RenderObjectData() noexcept
	{
	}

}
