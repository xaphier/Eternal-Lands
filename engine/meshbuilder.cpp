/****************************************************************************
 *            meshbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshbuilder.hpp"
#include "mesh/opengl2mesh.hpp"
#include "mesh/opengl3mesh.hpp"
#include "mesh/opengl31mesh.hpp"
#include "vertexformat.hpp"
#include "abstractmesh.hpp"
#include "exceptions.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	namespace
	{

		const bool use_tangent = true;

		const String vertex_format_type_names[] =
		{
			String(UTF8("mesh")),
			String(UTF8("animated_mesh")),
			String(UTF8("morph_mesh")),
			String(UTF8("instanced_mesh")),
			String(UTF8("mesh_extra_uv")),
			String(UTF8("animated_mesh_extra_uv")),
			String(UTF8("morph_mesh_extra_uv")),
			String(UTF8("instanced_mesh_extra_uv")),
			String(UTF8("sprite")),
			String(UTF8("font"))
		};

		const Uint32 vertex_format_type_names_count =
			sizeof(vertex_format_type_names) / sizeof(String);

	}

	MeshBuilder::MeshBuilder(const GlobalVarsSharedPtr &global_vars):
		m_global_vars(global_vars)
	{
		VertexDescriptionMap mesh, animated_mesh, morph_mesh;
		VertexDescriptionMap instanced_mesh, sprite, font;
		VertexElementType position, texture_coordinate, normal;

		if (get_global_vars()->get_opengl_3_0() || GLEW_ARB_half_float_vertex)
		{
			position = vet_half4;
			texture_coordinate = vet_half2;
			font[vst_position] = vet_half2;
		}
		else
		{
			position = vet_float3;
			texture_coordinate = vet_float2;
			font[vst_position] = vet_float2;
		}

		mesh[vst_position] = position;
		mesh[vst_texture_coordinate_0] = texture_coordinate;
		mesh[vst_color] = vet_ubyte4_normalized;
		animated_mesh[vst_position] = position;
		animated_mesh[vst_texture_coordinate_0] = texture_coordinate;
		animated_mesh[vst_color] = vet_ubyte4_normalized;
		morph_mesh[vst_position] = position;
		morph_mesh[vst_texture_coordinate_0] = texture_coordinate;
		morph_mesh[vst_color] = vet_ubyte4_normalized;
		morph_mesh[vst_morph_position] = position;
		morph_mesh[vst_morph_texture_coordinate_0] = texture_coordinate;
		instanced_mesh[vst_position] = position;
		instanced_mesh[vst_texture_coordinate_0] = texture_coordinate;
		instanced_mesh[vst_color] = vet_ubyte4_normalized;

		sprite[vst_position] = position;
		sprite[vst_texture_coordinate_0] = vet_ushort2_normalized;

		font[vst_texture_coordinate_0] = vet_ushort2_normalized;
		font[vst_color] = vet_ubyte4_normalized;

		animated_mesh[vst_bone_index] = vet_ubyte4;
		animated_mesh[vst_bone_weight] = vet_ushort4_normalized;

		if (get_global_vars()->get_opengl_3_3())
		{
			normal = vet_signed_xyz10_w2_normalized;
		}
		else
		{
			normal = vet_short4_normalized;
		}

		mesh[vst_normal] = normal;
		animated_mesh[vst_normal] = normal;
		morph_mesh[vst_normal] = normal;
		morph_mesh[vst_morph_normal] = normal;
		instanced_mesh[vst_normal] = normal;

		if (use_tangent)
		{
			mesh[vst_tangent] = normal;
			animated_mesh[vst_tangent] = normal;
			morph_mesh[vst_tangent] = normal;
			morph_mesh[vst_morph_tangent] = normal;
			instanced_mesh[vst_tangent] = normal;
		}

		set_format(vft_sprite, VertexElements(sprite));
		set_format(vft_font, VertexElements(font));

		set_format(vft_mesh, VertexElements(mesh));
		set_format(vft_animated_mesh, VertexElements(animated_mesh));
		set_format(vft_morph_mesh, VertexElements(morph_mesh));
		set_format(vft_instanced_mesh, VertexElements(instanced_mesh));

		mesh[vst_texture_coordinate_1] = texture_coordinate;
		animated_mesh[vst_texture_coordinate_1] = texture_coordinate;
		morph_mesh[vst_texture_coordinate_1] = texture_coordinate;
		morph_mesh[vst_morph_texture_coordinate_1] = texture_coordinate;
		instanced_mesh[vst_texture_coordinate_1] = texture_coordinate;

		set_format(vft_mesh_extra_uv, VertexElements(mesh));
		set_format(vft_animated_mesh_extra_uv,
			VertexElements(animated_mesh));
		set_format(vft_morph_mesh_extra_uv,
			VertexElements(morph_mesh));
		set_format(vft_instanced_mesh_extra_uv,
			VertexElements(instanced_mesh));
	}

	MeshBuilder::~MeshBuilder() throw()
	{
	}

	void MeshBuilder::set_format(const VertexFormatType format,
		const VertexElements &elements)
	{
		m_vertex_formats[format] = boost::make_shared<VertexFormat>(
			get_str(format), elements);
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh(const String &name,
		const bool static_indices, const bool static_vertices) const
	{
		if (get_global_vars()->get_opengl_3_1() && false)
		{
			return boost::make_shared<OpenGl31Mesh>(name,
				static_indices, static_vertices);
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			return boost::make_shared<OpenGl3Mesh>(name,
				static_indices, static_vertices);
		}

		return boost::make_shared<OpenGl2Mesh>(name, static_indices,
			static_vertices);
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh(
		const VertexFormatType vertex_format,
		const MeshDataToolSharedPtr &mesh_data_tool, const String &name,
		const bool static_indices, const bool static_vertices) const
	{
		AbstractMeshSharedPtr result;

		result = get_mesh(name, static_indices, static_vertices);

		result->init(get_vertex_format(vertex_format), mesh_data_tool);

		return result;
	}

	VertexFormatSharedPtr MeshBuilder::get_vertex_format(
		const VertexFormatType vertex_format) const
	{
		VertexFormatsMap::const_iterator found;

		found = m_vertex_formats.find(vertex_format);

		if (found == m_vertex_formats.end())
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("Invalid vertex "
					"format"))
				<< errinfo_value(vertex_format));
		}

		return found->second;
	}

	const String &MeshBuilder::get_str(const VertexFormatType vertex_format)
	{
		if (vertex_format_type_names_count <= vertex_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_format_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_format))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexFormatType")));
		}

		return vertex_format_type_names[vertex_format];
	}

	VertexFormatType MeshBuilder::get_type(const String &str)
	{
		Uint32 i;
		VertexFormatType vertex_format_type;

		for (i = 0; i < vertex_format_type_names_count; ++i)
		{
			vertex_format_type = static_cast<VertexFormatType>(i);

			if (str == get_str(vertex_format_type))
			{
				return vertex_format_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"VertexFormatType")));
	}

	OutStream& operator<<(OutStream &str,
		const VertexFormatType value)
	{
		str << MeshBuilder::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, VertexFormatType &value)
	{
		StringType string;

		str >> string;

		value = MeshBuilder::get_type(String(string));

		return str;
	}

}
