/****************************************************************************
 *            filter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filter.hpp"
#include "glslprogram.hpp"
#include "shader/shadertextureutil.hpp"
#include "meshcache.hpp"
#include "statemanager.hpp"
#include "abstractmesh.hpp"

namespace eternal_lands
{

	namespace
	{

		const float weights[] = { 70, 56, 28, 8, 1 };

		void get_linear_gauss_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			scale.x = weights[0];
			scale.y = weights[1] + weights[2];
			scale.z = weights[3] + weights[4];
			scale.w = 0.0f;

			offset.x = (weights[1] + 2 * weights[2]) / scale.y;
			offset.y = (3 * weights[3] + 4 * weights[4]) / scale.z;
			offset.z = 0.0f;
			offset.w = 0.0f;

			scale /= 256.0f;
		}

		void get_linear_scale_offset(glm::vec4 &scale,
			glm::vec4 &offset)
		{
			scale.x = 1;
			scale.y = 2;
			scale.z = 2;
			scale.w = 0.0f;

			offset.x = 1.5f;
			offset.y = 3.5f;
			offset.z = 0.0f;
			offset.w = 0.0f;

			scale /= 9.0f;
		}

	}

	Filter::Filter(const MeshCacheSharedPtr &mesh_cache,
		const Uint16 channel_count): m_channel_count(channel_count)
	{
		StringVariantMap values;

		mesh_cache->get_mesh(String(L"rect"), m_mesh);

		values[ShaderTextureUtil::get_str(stt_diffuse_0)] =
			Variant(static_cast<Sint64>(stt_diffuse_0));

		m_programs[0] = boost::make_shared<GlslProgram>(
			get_vertex_str(130),
			get_fragment_str(130, false, false), values,
			String(L"filter_horizontal"));

		m_programs[1] = boost::make_shared<GlslProgram>(
			get_vertex_str(130),
			get_fragment_str(130, false, true), values,
			String(L"filter_vertical"));

		m_programs[2] = boost::make_shared<GlslProgram>(
			get_vertex_str(130),
			get_fragment_str(130, true, false), values,
			String(L"filter_horizontal_layer"));

		m_programs[3] = boost::make_shared<GlslProgram>(
			get_vertex_str(130),
			get_fragment_str(130, true, true), values,
			String(L"filter_vertical_layer"));
	}

	Filter::~Filter() throw()
	{
	}

	String Filter::get_vertex_str(const Uint16 version) const
	{
		StringStream str;

		str << L"#version " << version << L"\n";
		str << L"\n";
		str << L"attribute vec2 " << vst_position << L";\n";
		str << L"\n";
		str << L"varying vec2 uv;\n";
		str << L"\n";
		str << L"uniform vec4 scale_offset;\n";
		str << L"\n";
		str << L"void main()\n";
		str << L"{\n";
		str << L"\tgl_Position = vec4(position * 2.0 - 1.0, 0.5, 1.0);";
		str << L"\n";
		str << L"\tuv = position * scale_offset.xy + ";
		str << L"scale_offset.zw;\n";
		str << L"}\n";

		return String(str.str());
	}

	String Filter::get_fragment_str(const Uint16 version, const bool layer,
		const bool vertical) const
	{
		StringStream str, tmp;
		boost::array<StringType, 5> textures;
		StringType channels;

		str << L"#version " << version << L"\n";
		str << L"\n";
		str << L"varying vec2 uv;\n";
		str << L"\n";
		str << L"uniform vec4 scale;\n";
		str << L"uniform vec4 offset;\n";

		if (layer)
		{
			str << L"uniform int layer;\n";
			str << L"uniform sampler2DArray " << stt_diffuse_0;
		}
		else
		{
			str << L"uniform sampler2D " << stt_diffuse_0;
		}

		str << L";\n";
		str << L"\n";
		str << L"void main()\n";
		str << L"{\n";

		switch (get_channel_count())
		{
			case 1:
				str << L"\tfloat color;\n";
				channels = L".r";
				break;
			case 2:
				str << L"\tvec2 color;\n";
				channels = L".rg";
				break;
			case 3:
				str << L"\tvec3 color;\n";
				channels = L".rgb";
				break;
			case 4:
				str << L"\tvec4 color;\n";
				channels = L"";
				break;
		}

		if (layer)
		{
			tmp << L"texture("  << stt_diffuse_0 << L", vec3(uv";
			textures[0] = tmp.str() + L", layer))";
			textures[1] = tmp.str();
			textures[2] = tmp.str();
			textures[3] = tmp.str();
			textures[4] = tmp.str();

			if (vertical)
			{
				textures[1] += L" + offset.xw, layer))";
				textures[2] += L" - offset.xw, layer))";
				textures[3] += L" + offset.yw, layer))";
				textures[4] += L" - offset.yw, layer))";
			}
			else
			{
				textures[1] += L" + offset.wx, layer))";
				textures[2] += L" - offset.wx, layer))";
				textures[3] += L" + offset.wy, layer))";
				textures[4] += L" - offset.wy, layer))";
			}
		}
		else
		{
			if (version >= 130)
			{
				tmp << L"texture("  << stt_diffuse_0;
				tmp << L", uv";
			}
			else
			{
				tmp << L"texture2D("  << stt_diffuse_0;
				tmp << L", uv";
			}

			textures[0] = tmp.str() + L")";
			textures[1] = tmp.str();
			textures[2] = tmp.str();
			textures[3] = tmp.str();
			textures[4] = tmp.str();

			if (vertical)
			{
				textures[1] += L" + offset.xw)";
				textures[2] += L" - offset.xw)";
				textures[3] += L" + offset.yw)";
				textures[4] += L" - offset.yw)";
			}
			else
			{
				textures[1] += L" + offset.wx)";
				textures[2] += L" - offset.wx)";
				textures[3] += L" + offset.wy)";
				textures[4] += L" - offset.wy)";
			}
		}

		str << L"\n";	
		str << L"\tcolor = " << textures[0] << channels;
#if	1
		str << " * scale.x;\n";
		str << L"\tcolor += " << textures[1] << channels;
		str << " * scale.y;\n";
		str << L"\tcolor += " << textures[2] << channels;
		str << " * scale.y;\n";
		str << L"\tcolor += " << textures[3] << channels;
		str << " * scale.z;\n";
		str << L"\tcolor += " << textures[4] << channels;
		str << " * scale.z;\n";
#else
		str << ";\n";
#endif
		str << L"\n";
		str << L"\tgl_FragColor = ";

		switch (get_channel_count())
		{
			case 1:
				str << L"vec4(color, 0.0f, 0.0f, 0.0f);\n";
				break;
			case 2:
				str << L"vec4(color, 0.0f, 0.0f);\n";
				break;
			case 3:
				str << L"vec4(color, 0.0f);\n";
				break;
			case 4:
				str << L"color;\n";
				break;
		}

		str << L"}\n";

		return String(str.str());
	}

	void Filter::bind(const Uint32 width, const Uint32 height,
		const Uint32 layer, const bool vertical,
		StateManager &state_manager)
	{
		glm::vec4 scale, offset, scale_offset;

		get_linear_gauss_scale_offset(scale, offset);

		scale_offset.x = (width - 1) / static_cast<float>(width);
		scale_offset.y = (height - 1) / static_cast<float>(height);
		scale_offset.z = 0.5f / static_cast<float>(width);
		scale_offset.w = 0.5f / static_cast<float>(height);

		if (vertical)
		{
			state_manager.switch_program(m_programs[2]);
			offset /= static_cast<float>(height);
		}
		else
		{
			state_manager.switch_program(m_programs[3]);
			offset /= static_cast<float>(width);
		}

		state_manager.get_program()->set_variant_parameter(
			String(L"layer"), static_cast<Sint64>(layer));
		state_manager.get_program()->set_variant_parameter(
			String(L"scale"), scale);
		state_manager.get_program()->set_variant_parameter(
			String(L"offset"), offset);
		state_manager.get_program()->set_variant_parameter(
			String(L"scale_offset"), scale_offset);
		state_manager.switch_mesh(m_mesh);
		state_manager.get_mesh()->draw(0);
	}

	void Filter::bind(const Uint32 width, const Uint32 height,
		const bool vertical, StateManager &state_manager)
	{
		glm::vec4 scale, offset, scale_offset;

		get_linear_gauss_scale_offset(scale, offset);

		scale_offset.x = (width - 1) / static_cast<float>(width);
		scale_offset.y = (height - 1) / static_cast<float>(height);
		scale_offset.z = 0.5f / static_cast<float>(width);
		scale_offset.w = 0.5f / static_cast<float>(height);

		if (vertical)
		{
			state_manager.switch_program(m_programs[1]);
			offset /= static_cast<float>(height);
		}
		else
		{
			state_manager.switch_program(m_programs[0]);
			offset /= static_cast<float>(width);
		}

		state_manager.get_program()->set_variant_parameter(
			String(L"scale"), scale);
		state_manager.get_program()->set_variant_parameter(
			String(L"offset"), offset);
		state_manager.get_program()->set_variant_parameter(
			String(L"scale_offset"), scale_offset);
		state_manager.switch_mesh(m_mesh);
		state_manager.get_mesh()->draw(0);
	}

}
