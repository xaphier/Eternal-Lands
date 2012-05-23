/****************************************************************************
 *            terraininstancesbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraininstancesbuffer.hpp"
#include "uniformbufferutil.hpp"
#include "autoparameterutil.hpp"
#include "abstractwritememory.hpp"

namespace eternal_lands
{

	TerrainInstancesBuffer::TerrainInstancesBuffer()
	{
	}

	TerrainInstancesBuffer::~TerrainInstancesBuffer() noexcept
	{
	}

	void TerrainInstancesBuffer::write(const String &indent, OutStream &str)
	{
		str << indent << UTF8("uniform ");
		str << UniformBufferUtil::get_identifier(ubt_terrain_instances);
		str << UTF8("\n") << indent << UTF8("{\n") << indent;
		str << UTF8("\t");
		str << AutoParameterUtil::get_type(apt_terrain_instances);
		str << UTF8(" ") << apt_terrain_instances << UTF8("[");
		str << AutoParameterUtil::get_size(apt_terrain_instances);
		str << UTF8("];\n") << indent << UTF8("};\n");
	}

	void TerrainInstancesBuffer::set_terrain_instance(
		const glm::mat2x4 &data, const Uint16 index)
	{
		memcpy(static_cast<Uint8*>(m_memory->get_ptr())
			+ index * sizeof(data), glm::value_ptr(data),
			sizeof(data));
	}

}

