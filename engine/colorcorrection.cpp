/****************************************************************************
 *            colorcorrection.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "colorcorrection.hpp"
#include "script/scriptengine.hpp"
#include "script/imagescript.hpp"
#include "image.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	ColorCorrection::ColorCorrection(
		const ScriptEngineSharedPtr &script_engine):
		m_script_engine(script_engine)
	{
	}

	ColorCorrection::~ColorCorrection() noexcept
	{
	}

	TextureSharedPtr ColorCorrection::get_color_correction(
		const String name)
	{
		TextureSharedPtr texture;
		ImageSharedPtr image;
		boost::scoped_ptr<ImageScript> image_script;

		image = boost::make_shared<Image>(name, false, tft_rgb10_a2,
			glm::uvec3(get_lut_size()), 0);

		m_context = get_script_engine()->get_context();

		image_script.reset(new ImageScript(get_script_engine(),
			name));

		image_script->execute(image, 0, 0, m_context);

		texture = boost::make_shared<Texture>(name);

		texture->set_wrap_s(twt_clamp);
		texture->set_wrap_t(twt_clamp);
		texture->set_wrap_r(twt_clamp);
		texture->set_target(ttt_texture_3d);
		texture->set_format(image->get_texture_format());
		texture->set_mipmap_count(0);
		texture->set_image(image);

		return texture;
	}

}
