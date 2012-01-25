/****************************************************************************
 *            objectdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectdescription.hpp"
#include "materialeffectdescription.hpp"

namespace eternal_lands
{

	ObjectDescription::ObjectDescription(const ObjectData &object_data,
		const MaterialEffectDescriptionVector &materials):
		ObjectData(object_data), m_materials(materials)
	{
	}

	ObjectDescription::~ObjectDescription() throw()
	{
	}

}
