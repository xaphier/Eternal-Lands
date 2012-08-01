/****************************************************************************
 *            imagescript.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "imagescript.hpp"
#include "scriptengine.hpp"
#include "image.hpp"

namespace eternal_lands
{

	namespace
	{

		const String material_function = String(UTF8("void main("
			"vec3 & in position, vec4 & out color)"));

	}

	ImageScript::ImageScript(
		const ScriptEngineSharedPtr &script_engine, const String &name):
		BasicScript(name)
	{
		set_function(String(UTF8("Image")), material_function,
			script_engine);
	}

	ImageScript::~ImageScript() noexcept
	{
	}

	bool ImageScript::execute(const ImageSharedPtr &image,
		const Uint16 face, const Uint16 mipmap,
		asIScriptContext* context)
	{
		glm::vec4 color;
		glm::vec3 position;
		glm::uvec3 sizes;
		Uint32 x, y, z;
		int r;

		sizes = image->get_sizes(mipmap);

		for (x = 0; x < sizes.x; ++x)
		{
			for (y = 0; y < sizes.y; ++y)
			{
				for (z = 0; z < sizes.z; ++z)
				{
					position = glm::vec3(x, y, z);
					position /= glm::vec3(sizes);

					prepare(context);

					context->SetArgObject(0,
						glm::value_ptr(position));
					context->SetArgObject(1,
						glm::value_ptr(color));

					r = context->Execute();

					if (r == asEXECUTION_EXCEPTION)
					{
						log_exception(context);

						return false;
					}

					image->set_pixel(x, y, z, face, mipmap,
						color);
				}
			}
		}

		return true;
	}

}
