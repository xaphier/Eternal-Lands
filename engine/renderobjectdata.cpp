/****************************************************************************
 *            renderobjectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "renderobjectdata.hpp"
#include "object.hpp"

namespace eternal_lands
{

	RenderObjectData::RenderObjectData(): m_visibility_mask(all_bits_set),
		m_transparency(0.0f), m_distance(0.0f),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(bt_alpha_transparency_source_value),
		m_depth_read(true)
	{
	}

	RenderObjectData::RenderObjectData(const ObjectSharedPtr &object,
		const BitSet64 visibility_mask): m_object(object),
		m_visibility_mask(visibility_mask), m_transparency(0.0f),
		m_distance(0.0f),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(bt_alpha_transparency_source_value),
		m_depth_read(true)
	{
		m_transparency = object->get_transparency();
		m_blend = object->get_blend();
		m_blend_mask = object->get_blend_mask();
	}

	RenderObjectData::RenderObjectData(const ObjectSharedPtr &object,
		const BitSet64 visibility_mask, const BitSet64 blend_mask,
		const float transparency, const BlendType blend):
		m_object(object), m_visibility_mask(visibility_mask),
		m_blend_mask(blend_mask), m_transparency(transparency),
		m_distance(0.0f),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(blend), m_depth_read(true)
	{
	}

	RenderObjectData::RenderObjectData(const ObjectSharedPtr &object,
		const BitSet64 visibility_mask, const BitSet64 blend_mask,
		const float transparency, const BlendType blend,
		const bool depth_pre_pass): m_object(object),
		m_visibility_mask(visibility_mask), m_blend_mask(blend_mask),
		m_transparency(transparency), m_distance(0.0f),
		m_occlusion_culling(std::numeric_limits<Uint32>::max()),
		m_lod(0), m_blend(blend), m_depth_read(depth_pre_pass)
	{
	}

	RenderObjectData::~RenderObjectData() noexcept
	{
	}

}
