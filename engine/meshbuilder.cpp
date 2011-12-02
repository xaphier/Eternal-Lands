/****************************************************************************
 *            meshbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshbuilder.hpp"
#include "mesh/opengl2mesh.hpp"
#include "mesh/opengl3mesh.hpp"
#include "vertexformat.hpp"
#include "abstractmesh.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String vertex_format_type_names[] =
		{
			String(L"mesh"),
			String(L"animated_mesh"),
			String(L"morph_mesh"),
			String(L"mesh_tangent"),
			String(L"animated_mesh_tangent"),
			String(L"morph_mesh_tangent"),
			String(L"mesh_extra_uv"),
			String(L"animated_mesh_extra_uv"),
			String(L"morph_mesh_extra_uv"),
			String(L"mesh_tangent_extra_uv"),
			String(L"animated_mesh_tangent_extra_uv"),
			String(L"morph_mesh_tangent_extra_uv"),
			String(L"rect"),
			String(L"sprite"),
		};

		const Uint32 vertex_format_type_names_count =
			sizeof(vertex_format_type_names) / sizeof(String);

	}

	MeshBuilder::MeshBuilder()
	{
		VertexDescriptionMap mesh, animated_mesh, morph_mesh;
		VertexDescriptionMap mesh_tangent, animated_mesh_tangent;
		VertexDescriptionMap morph_mesh_tangent, rect, sprite;

		if (GLEW_ARB_half_float_vertex)
		{
			mesh[vst_position] = vet_half4;
			mesh[vst_texture_coordinate_0] = vet_half2;
			animated_mesh[vst_position] = vet_half4;
			animated_mesh[vst_texture_coordinate_0] = vet_half2;
			morph_mesh[vst_position] = vet_half4;
			morph_mesh[vst_texture_coordinate_0] = vet_half2;
			morph_mesh[vst_morph_position] = vet_half4;
			morph_mesh[vst_morph_texture_coordinate_0] = vet_half2;
			sprite[vst_position] = vet_half4;
			sprite[vst_texture_coordinate_0] = vet_half2;
		}
		else
		{
			mesh[vst_position] = vet_float3;
			mesh[vst_texture_coordinate_0] = vet_float2;
			animated_mesh[vst_position] = vet_float3;
			animated_mesh[vst_texture_coordinate_0] = vet_float2;
			morph_mesh[vst_position] = vet_float3;
			morph_mesh[vst_texture_coordinate_0] = vet_float2;
			morph_mesh[vst_morph_position] = vet_float3;
			morph_mesh[vst_morph_texture_coordinate_0] = vet_float2;
			sprite[vst_position] = vet_float3;
			sprite[vst_texture_coordinate_0] = vet_ushort2;
		}

		rect[vst_position] = vet_ushort2;

		animated_mesh[vst_bone_index] = vet_ubyte4;
		animated_mesh[vst_bone_weight] = vet_ushort4_normalized;

		if (GLEW_ARB_vertex_type_2_10_10_10_rev)
		{
			mesh[vst_normal] = vet_signed_xyz10_w2_normalized;
			animated_mesh[vst_normal] =
				vet_signed_xyz10_w2_normalized;
			morph_mesh[vst_normal] =
				vet_signed_xyz10_w2_normalized;
			morph_mesh[vst_morph_normal] =
				vet_signed_xyz10_w2_normalized;

			mesh_tangent = mesh;
			animated_mesh_tangent = animated_mesh;
			morph_mesh_tangent = morph_mesh;

			mesh_tangent[vst_tangent] =
				vet_signed_xyz10_w2_normalized;
			animated_mesh_tangent[vst_tangent] =
				vet_signed_xyz10_w2_normalized;
			morph_mesh_tangent[vst_tangent] =
				vet_signed_xyz10_w2_normalized;
			morph_mesh_tangent[vst_morph_tangent] =
				vet_signed_xyz10_w2_normalized;
		}
		else
		{
			mesh[vst_normal] = vet_short4_normalized;
			animated_mesh[vst_normal] = vet_short4_normalized;
			morph_mesh[vst_normal] = vet_short4_normalized;
			morph_mesh[vst_morph_normal] = vet_short4_normalized;

			mesh_tangent = mesh;
			animated_mesh_tangent = animated_mesh;
			morph_mesh_tangent = morph_mesh;

			mesh_tangent[vst_tangent] = vet_short4_normalized;
			animated_mesh_tangent[vst_tangent] =
				vet_short4_normalized;
			morph_mesh_tangent[vst_tangent] =
				vet_short4_normalized;
			morph_mesh_tangent[vst_morph_tangent] =
				vet_short4_normalized;
		}

		set_format(vft_rect, VertexElements(rect));

		set_format(vft_sprite, VertexElements(sprite));

		set_format(vft_mesh, VertexElements(mesh));

		set_format(vft_animated_mesh, VertexElements(animated_mesh));

		set_format(vft_morph_mesh, VertexElements(morph_mesh));

		set_format(vft_mesh_tangent, VertexElements(mesh_tangent));

		set_format(vft_animated_mesh_tangent,
			VertexElements(animated_mesh_tangent));

		set_format(vft_morph_mesh_tangent,
			VertexElements(morph_mesh_tangent));

		if (GLEW_ARB_half_float_vertex)
		{
			mesh[vst_texture_coordinate_1] = vet_half2;
			animated_mesh[vst_texture_coordinate_1] = vet_half2;
			morph_mesh[vst_texture_coordinate_1] = vet_half2;
			morph_mesh[vst_morph_texture_coordinate_1] = vet_half2;

			mesh_tangent[vst_texture_coordinate_1] = vet_half2;
			animated_mesh_tangent[vst_texture_coordinate_1] =
				vet_half2;
			morph_mesh_tangent[vst_texture_coordinate_1] =
				vet_half2;
			morph_mesh_tangent[vst_morph_texture_coordinate_1] =
				vet_half2;
		}
		else
		{
			mesh[vst_texture_coordinate_1] = vet_float2;
			animated_mesh[vst_texture_coordinate_1] = vet_float2;
			morph_mesh[vst_texture_coordinate_1] = vet_float2;
			morph_mesh[vst_morph_texture_coordinate_1] = vet_float2;

			mesh_tangent[vst_texture_coordinate_1] = vet_float2;
			animated_mesh_tangent[vst_texture_coordinate_1] =
				vet_float2;
			morph_mesh_tangent[vst_texture_coordinate_1] =
				vet_float2;
			morph_mesh_tangent[vst_morph_texture_coordinate_1] =
				vet_float2;
		}

		set_format(vft_mesh_extra_uv, VertexElements(mesh));

		set_format(vft_animated_mesh_extra_uv,
			VertexElements(animated_mesh));

		set_format(vft_morph_mesh_extra_uv, VertexElements(morph_mesh));

		set_format(vft_mesh_tangent_extra_uv,
			VertexElements(mesh_tangent));

		set_format(vft_animated_mesh_tangent_extra_uv,
			VertexElements(animated_mesh_tangent));

		set_format(vft_morph_mesh_tangent_extra_uv,
			VertexElements(morph_mesh_tangent));
	}

	MeshBuilder::~MeshBuilder() throw()
	{
	}

	void MeshBuilder::set_format(const VertexFormatType format,
		const VertexElements &elements)
	{
		m_vertex_formats[format].reset(new VertexFormat(
			get_str(format), elements));
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh() const
	{
		if (GLEW_ARB_vertex_array_object)
		{
			return boost::make_shared<OpenGl3Mesh>();
		}
		else
		{
			return boost::make_shared<OpenGl2Mesh>();
		}
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh(
		const VertexFormatType vertex_format,
		const MeshDataToolSharedPtr &mesh_data_tool) const
	{
		AbstractMeshSharedPtr result;

		result = get_mesh();

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
				<< errinfo_message(
					L"Invalid vertex format")
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
				<< boost::errinfo_type_info_name(
					"VertexFormatType"));
		}

		return vertex_format_type_names[vertex_format];
	}

	VertexFormatType MeshBuilder::get_type(const String &str)
	{
		Uint32 i;
		VertexFormatType vertex_format_type;

		for (i = 0; i < vertex_format_type_names_count; i++)
		{
			vertex_format_type = static_cast<VertexFormatType>(i);

			if (str == get_str(vertex_format_type))
			{
				return vertex_format_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				"VertexFormatType"));
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
