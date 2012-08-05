/****************************************************************************
 *            filter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filter.hpp"
#include "shader/glslprogram.hpp"
#include "shader/glslprogramdescription.hpp"
#include "shader/samplerparameterutil.hpp"
#include "meshcache.hpp"
#include "statemanager.hpp"
#include "abstractmesh.hpp"
#include "globalvars.hpp"
#include "shader/glslprogramcache.hpp"

namespace eternal_lands
{

	namespace
	{

		const float weights_5[] = { 6, 4, 1 };
		const float weights_9[] = { 70, 56, 28, 8, 1 };
		const float weights_13[] = { 924, 792, 495, 220, 66, 12, 1 };
		const float weights_17[] = { 12870, 11440, 8008, 4368, 1820,
			560, 120, 16, 1 };

		void get_linear_gauss_5_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_5[0];
			scale.y = weights_5[1] + weights_5[2];
			scale.z = 0.0f;
			scale.w = 0.0f;

			offset.x = (weights_5[1] + 2 * weights_5[2]) / scale.y;
			offset.y = 0.0f;
			offset.z = 0.0f;
			offset.w = 0.0f;

			div = weights_5[1] + weights_5[2];
			div = weights_5[0] + 2.0f * div;
			scale /= div;
		}

		void get_linear_gauss_9_short_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_9[0];
			scale.y = weights_9[1] + weights_9[2];
			scale.z = 0.0f;
			scale.w = 0.0f;

			offset.x = (weights_9[1] + 2 * weights_9[2]) / scale.y;
			offset.y = 0.0f;
			offset.z = 0.0f;
			offset.w = 0.0f;

			div = weights_9[1] + weights_9[2];
			div = weights_9[0] + div * 2.0f;
			scale /= div;
		}

		void get_linear_gauss_9_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_9[0];
			scale.y = weights_9[1] + weights_9[2];
			scale.z = weights_9[3] + weights_9[4];
			scale.w = 0.0f;

			offset.x = (weights_9[1] + 2 * weights_9[2]) / scale.y;
			offset.y = (3 * weights_9[3] + 4 * weights_9[4]) /
				scale.z;
			offset.z = 0.0f;
			offset.w = 0.0f;

			div = weights_9[1] + weights_9[2];
			div += weights_9[3] + weights_9[4];
			div = weights_9[0] + div * 2.0f;
			scale /= div;
		}

		void get_linear_gauss_13_short_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_13[0];
			scale.y = weights_13[1] + weights_13[2];
			scale.z = weights_13[3] + weights_13[4];
			scale.w = 0.0f;

			offset.x = (weights_13[1] + 2 * weights_13[2]) /
				scale.y;
			offset.y = (3 * weights_13[3] + 4 * weights_13[4]) /
				scale.z;
			offset.z = 0.0f;
			offset.w = 0.0f;

			div = weights_13[1] + weights_13[2];
			div += weights_13[3] + weights_13[4];
			div = weights_13[0] + 2.0f * div;
			scale /= div;
		}

		void get_linear_gauss_13_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_13[0];
			scale.y = weights_13[1] + weights_13[2];
			scale.z = weights_13[3] + weights_13[4];
			scale.w = weights_13[5] + weights_13[6];

			offset.x = (weights_13[1] + 2 * weights_13[2]) /
				scale.y;
			offset.y = (3 * weights_13[3] + 4 * weights_13[4]) /
				scale.z;
			offset.z = (5 * weights_13[5] + 6 * weights_13[6]) /
				scale.w;
			offset.w = 0.0f;

			div = weights_13[1] + weights_13[2];
			div += weights_13[3] + weights_13[4];
			div += weights_13[5] + weights_13[6];
			div = weights_13[0] + 2.0f * div;
			scale /= div;
		}

		void get_linear_gauss_17_short_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			float div;

			scale.x = weights_17[0];
			scale.y = weights_17[1] + weights_17[2];
			scale.z = weights_17[3] + weights_17[4];
			scale.w = weights_17[5] + weights_17[6];

			offset.x = (weights_17[1] + 2 * weights_17[2]) /
				scale.y;
			offset.y = (3 * weights_17[3] + 4 * weights_17[4]) /
				scale.z;
			offset.z = (5 * weights_17[5] + 6 * weights_17[6]) /
				scale.w;
			offset.w = 0.0f;

			div = weights_17[1] + weights_17[2];
			div += weights_17[3] + weights_17[4];
			div += weights_17[5] + weights_17[6];
			div = weights_17[0] + 2.0f * div;
			scale /= div;
		}

		void get_linear_box_5_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			scale.x = 1.0f;
			scale.y = 2.0f;
			scale.z = 0.0f;
			scale.w = 0.0f;

			offset.x = 1.5f;
			offset.y = 0.0f;
			offset.z = 0.0f;
			offset.w = 0.0f;

			scale /= 5.0f;
		}

		void get_linear_box_9_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			scale.x = 1.0f;
			scale.y = 2.0f;
			scale.z = 2.0f;
			scale.w = 0.0f;

			offset.x = 1.5f;
			offset.y = 3.5f;
			offset.z = 0.0f;
			offset.w = 0.0f;

			scale /= 9.0f;
		}

		void get_linear_box_13_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			scale.x = 1.0f;
			scale.y = 2.0f;
			scale.z = 2.0f;
			scale.w = 2.0f;

			offset.x = 1.5f;
			offset.y = 3.5f;
			offset.z = 5.5f;
			offset.w = 0.0f;

			scale /= 13.0f;
		}

	}

	void Filter::build_filter(
		const GlslProgramCacheSharedPtr &glsl_program_cache,
		const Uint16 version, const Uint16 channel_count,
		const Uint16 half_taps_minus_one, const bool layer,
		const bool vertical)
	{
		StringType name;
		String none;
		Uint16 index;

		index = get_index(channel_count, half_taps_minus_one, layer,
			vertical);

		if (vertical)
		{
			name = UTF8("filter vertical");
		}
		else
		{
			name = UTF8("filter horizontal");
		}

		if (layer)
		{
			name += UTF8(" layer");
		}

		m_programs[index] = glsl_program_cache->get_program(
			GlslProgramDescription(get_vertex_str(version), none,
				none, none, get_fragment_str(version,
					channel_count, half_taps_minus_one,
					layer, vertical)));
	}

	Filter::Filter(const GlslProgramCacheSharedPtr &glsl_program_cache,
		const MeshCacheSharedPtr &mesh_cache,
		const GlobalVarsSharedPtr &global_vars)
	{
		Uint32 i, j;

		mesh_cache->get_mesh(String(UTF8("plane_2")), m_mesh);

		for (i = 1; i < 5; ++i)
		{
			for (j = 0; j < 4; ++j)
			{
				if (!global_vars->get_opengl_3_0())
				{
					build_filter(glsl_program_cache, 120,
						i, j, false, false);
					build_filter(glsl_program_cache, 120,
						i, j, false, true);

					continue;
				}

				build_filter(glsl_program_cache, 130, i, j,
					false, false);
				build_filter(glsl_program_cache, 130, i, j,
					false, true);
				build_filter(glsl_program_cache, 130, i, j,
					true, false);
				build_filter(glsl_program_cache, 130, i, j,
					true, true);
			}
		}

		for (i = 1; i < 5; ++i)
		{
			build_multisample_filter(glsl_program_cache, 130, i, 2);
			build_multisample_filter(glsl_program_cache, 130, i, 4);
		}
	}

	Filter::~Filter() noexcept
	{
	}
	
	String Filter::get_multisample_vertex_str(const Uint16 version)
	{
		StringStream str;

		str << UTF8("#version ") << version << UTF8("\n");
		str << UTF8("\n");
		str << UTF8("attribute vec2 ") << vst_position << UTF8(";\n");
		str << UTF8("\n");
		str << UTF8("\n");
		str << UTF8("void main()\n");
		str << UTF8("{\n");
		str << UTF8("\tgl_Position = vec4(position, 0.5, 1.0);\n");
		str << UTF8("}\n");

		return String(str.str());
	}

	String Filter::get_multisample_fragment_str(const Uint16 version,
		const Uint16 channel_count, const Uint16 sample_count)
	{
		StringStream str;
		StringType channels, offset;
		Uint32 i, j;

		str << UTF8("#version ") << version << UTF8("\n");
		str << UTF8("\n");

		str << UTF8("uniform sampler2DMS ") << spt_effect_0;

		str << UTF8(";\n");
		str << UTF8("\n");
		str << UTF8("void main()\n");
		str << UTF8("{\n");

		switch (channel_count)
		{
			case 1:
				str << UTF8("\tfloat color;\n");
				channels = UTF8(".r");
				break;
			case 2:
				str << UTF8("\tvec2 color;\n");
				channels = UTF8(".rg");
				break;
			case 3:
				str << UTF8("\tvec3 color;\n");
				channels = UTF8(".rgb");
				break;
			case 4:
				str << UTF8("\tvec4 color;\n");
				channels = UTF8("");
				break;
		}

		for (j = 0; j < 9; ++j)
		{
			StringStream tmp;

			tmp << UTF8("vec2(") << ((j % 3) - 1.5f) << UTF8(", ");
			tmp << ((j / 3) - 1.5f) << UTF8(")");
			offset = tmp.str();

			for (i = 0; i < sample_count; ++i)
			{
				if ((i == 0) && (j == 0))
				{
					str << UTF8("\tcolor = ");
				}
				else
				{
					str << UTF8("\tcolor += ");
				}

				str << UTF8("texelFetch(") << spt_effect_0;
				str << UTF8(", ivec2(gl_FragCoord.xy + ");
				str << offset << UTF8("), ") << i << UTF8(")");
				str << channels << UTF8(";\n");
			}
		}

		str << UTF8("\n");
		str << UTF8("\tgl_FragColor = ");

		switch (channel_count)
		{
			case 1:
				str << UTF8("vec4(color, 0.0f, 0.0f, 0.0f);\n");
				break;
			case 2:
				str << UTF8("vec4(color, 0.0f, 0.0f);\n");
				break;
			case 3:
				str << UTF8("vec4(color, 0.0f);\n");
				break;
			case 4:
				str << UTF8("color;\n");
				break;
		}

		str << UTF8("}\n");

		return String(str.str());
	}

	void Filter::build_multisample_filter(
		const GlslProgramCacheSharedPtr &glsl_program_cache,
		const Uint16 version, const Uint16 channel_count,
		const Uint16 sample_count)
	{
		String name, none;
		Uint16 index;

		index = get_multisample_index(channel_count, sample_count);

		name = UTF8("filter multisample");

		m_multisample_programs[index] = glsl_program_cache->get_program(
			GlslProgramDescription(get_multisample_vertex_str(
					version), none, none, none,
				get_multisample_fragment_str(version,
					channel_count, sample_count)));
	}

	String Filter::get_vertex_str(const Uint16 version)
	{
		StringStream str;

		str << UTF8("#version ") << version << UTF8("\n");
		str << UTF8("\n");
		str << UTF8("attribute vec2 ") << vst_position << UTF8(";\n");
		str << UTF8("\n");
		str << UTF8("varying vec2 uv;\n");
		str << UTF8("\n");
		str << UTF8("uniform vec4 source_scale_offset;\n");
		str << UTF8("uniform vec4 dest_scale_offset;\n");
		str << UTF8("\n");
		str << UTF8("void main()\n");
		str << UTF8("{\n");
		str << UTF8("\tgl_Position = vec4(position * ");
		str << UTF8("dest_scale_offset.xy + dest_scale_offset.zw, ");
		str << UTF8("0.5, 1.0);\n");
		str << UTF8("\tuv = position * source_scale_offset.xy + ");
		str << UTF8("source_scale_offset.zw;\n");
		str << UTF8("}\n");

		return String(str.str());
	}

	String Filter::get_fragment_str(const Uint16 version,
		const Uint16 channel_count, const Uint16 half_taps_minus_one,
		const bool layer, const bool vertical)
	{
		StringStream str, tmp;
		boost::array<StringType, 7> textures;
		StringType channels;

		str << UTF8("#version ") << version << UTF8("\n");
		str << UTF8("\n");
		str << UTF8("varying vec2 uv;\n");
		str << UTF8("\n");
		str << UTF8("uniform vec4 scale;\n");
		str << UTF8("uniform vec4 offset;\n");

		if (layer)
		{
			str << UTF8("uniform float layer;\n");
			str << UTF8("uniform sampler2DArray ") << spt_effect_0;
		}
		else
		{
			str << UTF8("uniform sampler2D ") << spt_effect_0;
		}

		str << UTF8(";\n");
		str << UTF8("\n");
		str << UTF8("void main()\n");
		str << UTF8("{\n");

		switch (channel_count)
		{
			case 1:
				str << UTF8("\tfloat color;\n");
				channels = UTF8(".r");
				break;
			case 2:
				str << UTF8("\tvec2 color;\n");
				channels = UTF8(".rg");
				break;
			case 3:
				str << UTF8("\tvec3 color;\n");
				channels = UTF8(".rgb");
				break;
			case 4:
				str << UTF8("\tvec4 color;\n");
				channels = UTF8("");
				break;
		}

		if (layer)
		{
			tmp << UTF8("texture(") << spt_effect_0;
			tmp << UTF8(", vec3(uv");
			textures[0] = tmp.str() + UTF8(", layer))");
			textures[1] = tmp.str();
			textures[2] = tmp.str();
			textures[3] = tmp.str();
			textures[4] = tmp.str();
			textures[5] = tmp.str();
			textures[6] = tmp.str();

			if (vertical)
			{
				textures[1] += UTF8(" + offset.xw, layer))");
				textures[2] += UTF8(" - offset.xw, layer))");
				textures[3] += UTF8(" + offset.yw, layer))");
				textures[4] += UTF8(" - offset.yw, layer))");
				textures[5] += UTF8(" + offset.zw, layer))");
				textures[6] += UTF8(" - offset.zw, layer))");
			}
			else
			{
				textures[1] += UTF8(" + offset.wx, layer))");
				textures[2] += UTF8(" - offset.wx, layer))");
				textures[3] += UTF8(" + offset.wy, layer))");
				textures[4] += UTF8(" - offset.wy, layer))");
				textures[5] += UTF8(" + offset.wz, layer))");
				textures[6] += UTF8(" - offset.wz, layer))");
			}
		}
		else
		{
			if (version >= 130)
			{
				tmp << UTF8("texture(") << spt_effect_0;
				tmp << UTF8(", uv");
			}
			else
			{
				tmp << UTF8("texture2D(") << spt_effect_0;
				tmp << UTF8(", uv");
			}

			textures[0] = tmp.str() + UTF8(")");
			textures[1] = tmp.str();
			textures[2] = tmp.str();
			textures[3] = tmp.str();
			textures[4] = tmp.str();
			textures[5] = tmp.str();
			textures[6] = tmp.str();

			if (vertical)
			{
				textures[1] += UTF8(" + offset.xw)");
				textures[2] += UTF8(" - offset.xw)");
				textures[3] += UTF8(" + offset.yw)");
				textures[4] += UTF8(" - offset.yw)");
				textures[5] += UTF8(" + offset.zw)");
				textures[6] += UTF8(" - offset.zw)");
			}
			else
			{
				textures[1] += UTF8(" + offset.wx)");
				textures[2] += UTF8(" - offset.wx)");
				textures[3] += UTF8(" + offset.wy)");
				textures[4] += UTF8(" - offset.wy)");
				textures[5] += UTF8(" + offset.wz)");
				textures[6] += UTF8(" - offset.wz)");
			}
		}

		str << UTF8("\n");

		str << UTF8("\tcolor = ") << textures[0] << channels;
		str << UTF8(" * scale.x;\n");

		if (half_taps_minus_one > 0)
		{
			str << UTF8("\tcolor += ") << textures[1] << channels;
			str << UTF8(" * scale.y;\n");
			str << UTF8("\tcolor += ") << textures[2] << channels;
			str << UTF8(" * scale.y;\n");
		}

		if (half_taps_minus_one > 1)
		{
			str << UTF8("\tcolor += ") << textures[3] << channels;
			str << UTF8(" * scale.z;\n");
			str << UTF8("\tcolor += ") << textures[4] << channels;
			str << UTF8(" * scale.z;\n");
		}

		if (half_taps_minus_one > 2)
		{
			str << UTF8("\tcolor += ") << textures[5] << channels;
			str << UTF8(" * scale.w;\n");
			str << UTF8("\tcolor += ") << textures[6] << channels;
			str << UTF8(" * scale.w;\n");
		}

		str << UTF8("\n");
		str << UTF8("\tgl_FragColor = ");

		switch (channel_count)
		{
			case 1:
				str << UTF8("vec4(color, 0.0f, 0.0f, 0.0f);\n");
				break;
			case 2:
				str << UTF8("vec4(color, 0.0f, 0.0f);\n");
				break;
			case 3:
				str << UTF8("vec4(color, 0.0f);\n");
				break;
			case 4:
				str << UTF8("color;\n");
				break;
		}

		str << UTF8("}\n");

		return String(str.str());
	}

	Uint16 Filter::get_scale_offset(const FilterType type, glm::vec4 &scale,
		glm::vec4 &offset)
	{
		switch (type)
		{
			case ft_gauss_5_tap:
				get_linear_gauss_5_scale_offset(scale, offset);
				return 1;
			case ft_gauss_9_tap_short:
				get_linear_gauss_9_short_scale_offset(scale,
					offset);
				return 1;
			case ft_gauss_9_tap:
				get_linear_gauss_9_scale_offset(scale, offset);
				return 2;
			case ft_gauss_13_tap_short:
				get_linear_gauss_13_short_scale_offset(scale,
					offset);
				return 2;
			case ft_gauss_13_tap:
				get_linear_gauss_13_scale_offset(scale, offset);
				return 3;
			case ft_gauss_17_tap_short:
				get_linear_gauss_17_short_scale_offset(scale,
					offset);
				return 3;
			case ft_box_5_tap:
				get_linear_box_5_scale_offset(scale, offset);
				return 1;
			case ft_box_9_tap:
				get_linear_box_9_scale_offset(scale, offset);
				return 2;
			case ft_box_13_tap:
				get_linear_box_13_scale_offset(scale, offset);
				return 3;
		}

		return 0;
	}

	void Filter::bind(const glm::vec4 &source, const glm::vec4 &dest,
		const Uint32 width, const Uint32 height, const Uint32 layer,
		const Uint16 channel_count, const FilterType type,
		const bool vertical, StateManager &state_manager)
	{
		glm::vec4 scale, offset, scale_offset;
		glm::vec4 dest_scale_offset, source_scale_offset;
		Uint16 index, half_taps_minus_one;

		half_taps_minus_one = get_scale_offset(type, scale, offset);
		index = get_index(channel_count, half_taps_minus_one, true,
			vertical);

		scale_offset.x = (width - 1) / static_cast<float>(width);
		scale_offset.y = (height - 1) / static_cast<float>(height);
		scale_offset.z = 0.5f / static_cast<float>(width);
		scale_offset.w = 0.5f / static_cast<float>(height);

		dest_scale_offset = dest;
		source_scale_offset = source;

		source_scale_offset.x *= scale_offset.x;
		source_scale_offset.y *= scale_offset.y;
		source_scale_offset.z *= scale_offset.x;
		source_scale_offset.w *= scale_offset.y;
		source_scale_offset.z += scale_offset.z;
		source_scale_offset.w += scale_offset.w;

		dest_scale_offset *= 2.0f;
		dest_scale_offset.z -= 1.0f;
		dest_scale_offset.w -= 1.0f;

		if (vertical)
		{
			offset /= static_cast<float>(height);
		}
		else
		{
			offset /= static_cast<float>(width);
		}

		state_manager.switch_program(m_programs[index]);

		state_manager.get_program()->set_variant_parameter(
			String(UTF8("layer")), static_cast<float>(layer));
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("scale")), scale);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("offset")), offset);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("source_scale_offset")),
			source_scale_offset);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("dest_scale_offset")),
			dest_scale_offset);
		state_manager.switch_mesh(m_mesh);
		state_manager.draw(0, 1);
	}

	void Filter::bind(const glm::vec4 &source, const glm::vec4 &dest,
		const Uint32 width, const Uint32 height,
		const Uint16 channel_count, const FilterType type,
		const bool vertical, StateManager &state_manager)
	{
		glm::vec4 scale, offset, scale_offset;
		glm::vec4 dest_scale_offset, source_scale_offset;
		Uint16 index, half_taps_minus_one;

		half_taps_minus_one = get_scale_offset(type, scale, offset);
		index = get_index(channel_count, half_taps_minus_one, false,
			vertical);

		scale_offset.x = (width - 1) / static_cast<float>(width);
		scale_offset.y = (height - 1) / static_cast<float>(height);
		scale_offset.z = 0.5f / static_cast<float>(width);
		scale_offset.w = 0.5f / static_cast<float>(height);

		dest_scale_offset = dest;
		source_scale_offset = source;

		source_scale_offset.x *= scale_offset.x;
		source_scale_offset.y *= scale_offset.y;
		source_scale_offset.z *= scale_offset.x;
		source_scale_offset.w *= scale_offset.y;
		source_scale_offset.z += scale_offset.z;
		source_scale_offset.w += scale_offset.w;

		dest_scale_offset *= 2.0f;
		dest_scale_offset.z -= 1.0f;
		dest_scale_offset.w -= 1.0f;

		if (vertical)
		{
			offset /= static_cast<float>(height);
		}
		else
		{
			offset /= static_cast<float>(width);
		}

		state_manager.switch_program(m_programs[index]);

		state_manager.get_program()->set_variant_parameter(
			String(UTF8("scale")), scale);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("offset")), offset);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("source_scale_offset")),
			source_scale_offset);
		state_manager.get_program()->set_variant_parameter(
			String(UTF8("dest_scale_offset")),
			dest_scale_offset);
		state_manager.switch_mesh(m_mesh);
		state_manager.draw(0, 1);
	}

	void Filter::bind(const Uint16 channel_count,
		const Uint16 sample_count, StateManager &state_manager)
	{
		Uint32 index;

		index = get_multisample_index(channel_count, sample_count);

		state_manager.switch_program(m_multisample_programs[index]);
		state_manager.switch_mesh(m_mesh);
		state_manager.draw(0, 1);
	}

}
