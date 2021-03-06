/****************************************************************************
 *            meshbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshbuilder.hpp"
#include "mesh/opengl2mesh.hpp"
#include "mesh/opengl3mesh.hpp"
#include "mesh/opengl31mesh.hpp"
#include "mesh/opengl32mesh.hpp"
#include "mesh/opengl33mesh.hpp"
#include "vertexformat.hpp"
#include "abstractmesh.hpp"
#include "exceptions.hpp"
#include "globalvars.hpp"
#include "vertexbuffersbuilder.hpp"

namespace eternal_lands
{

	namespace
	{

		const String vertex_format_type_names[] =
		{
			String(UTF8("mesh")),
			String(UTF8("animated_mesh_2_bones")),
			String(UTF8("animated_mesh_4_bones")),
			String(UTF8("animated_mesh_6_bones")),
			String(UTF8("animated_mesh_8_bones")),
			String(UTF8("morph_mesh")),
			String(UTF8("instanced_mesh")),
			String(UTF8("mesh_extra_uv")),
			String(UTF8("animated_mesh_2_bones_extra_uv")),
			String(UTF8("animated_mesh_4_bones_extra_uv")),
			String(UTF8("animated_mesh_6_bones_extra_uv")),
			String(UTF8("animated_mesh_8_bones_extra_uv")),
			String(UTF8("morph_mesh_extra_uv")),
			String(UTF8("instanced_mesh_extra_uv")),
			String(UTF8("simple_terrain")),
			String(UTF8("cdlod_terrain")),
			String(UTF8("sprite")),
			String(UTF8("font"))
		};

		const Uint32 vertex_format_type_names_count =
			sizeof(vertex_format_type_names) / sizeof(String);

	}

	MeshBuilder::MeshBuilder(const GlobalVarsConstSharedPtr &global_vars,
		const HardwareBufferMapperConstWeakPtr &hardware_buffer_mapper):
		m_global_vars(global_vars),
		m_hardware_buffer_mapper(hardware_buffer_mapper)
	{
		VertexDescriptionMap mesh, morph_mesh;
		VertexDescriptionMap instanced_mesh_0, instanced_mesh_1;
		VertexDescriptionMap animated_mesh_2_bones;
		VertexDescriptionMap animated_mesh_4_bones;
		VertexDescriptionMap animated_mesh_6_bones;
		VertexDescriptionMap animated_mesh_8_bones;
		VertexDescriptionMap simple_terrain_0, simple_terrain_1;
		VertexDescriptionMap cdlod_terrain_0, cdlod_terrain_1;
		VertexDescriptionMap sprite, font;
		VertexElementsVector simple_terrain, instanced_mesh;
		VertexElementsVector cdlod_terrain;
		VertexElementType position, position4, texture_coordinate;
		VertexElementType normal, extra_texture_coordinate;

		if (get_global_vars()->get_opengl_3_0() ||
			GLEW_ARB_half_float_vertex)
		{
			position = vet_float3;
			position4 = vet_float4;
			texture_coordinate = vet_half2;
			extra_texture_coordinate = vet_half4;
			font[vst_position] = vet_half2;
			simple_terrain_0[vst_position] = vet_half2;
			cdlod_terrain_0[vst_position] = vet_half2;
		}
		else
		{
			position = vet_float3;
			position4 = vet_float4;
			texture_coordinate = vet_float2;
			extra_texture_coordinate = vet_float4;
			font[vst_position] = vet_float2;
			simple_terrain_0[vst_position] = vet_float2;
			cdlod_terrain_0[vst_position] = vet_float2;
		}

		if (get_global_vars()->get_opengl_3_3())
		{
			normal = vet_signed_xyz10_w2_normalized;
		}
		else
		{
			normal = vet_short4_normalized;
		}

		mesh[vst_position] = position;
		mesh[vst_texture_coordinate] = texture_coordinate;
		mesh[vst_normal] = normal;
		mesh[vst_tangent] = normal;
		mesh[vst_color] = vet_ubyte4_normalized;

		animated_mesh_2_bones[vst_position] = position4;
		animated_mesh_2_bones[vst_normal] = normal;
		animated_mesh_2_bones[vst_tangent] = normal;
		animated_mesh_2_bones[vst_texture_coordinate] =
			texture_coordinate;
		animated_mesh_2_bones[vst_bone_index] = vet_ubyte4;
		animated_mesh_2_bones[vst_bone_weight] =
			vet_ushort2_normalized;

		animated_mesh_4_bones[vst_position] = position4;
		animated_mesh_4_bones[vst_normal] = normal;
		animated_mesh_4_bones[vst_tangent] = normal;
		animated_mesh_4_bones[vst_texture_coordinate] =
			texture_coordinate;
		animated_mesh_4_bones[vst_bone_index] = vet_ubyte4;
		animated_mesh_4_bones[vst_bone_weight] =
			vet_ushort4_normalized;

		animated_mesh_6_bones[vst_position] = position4;
		animated_mesh_6_bones[vst_normal] = normal;
		animated_mesh_6_bones[vst_tangent] = normal;
		animated_mesh_6_bones[vst_texture_coordinate] =
			texture_coordinate;
		animated_mesh_6_bones[vst_bone_index] = vet_ubyte4;
		animated_mesh_6_bones[vst_bone_weight] =
			vet_ushort4_normalized;
		animated_mesh_6_bones[vst_extra_bone_index] = vet_ubyte4;
		animated_mesh_6_bones[vst_extra_bone_weight] =
			vet_ushort2_normalized;

		animated_mesh_8_bones[vst_position] = position4;
		animated_mesh_8_bones[vst_normal] = normal;
		animated_mesh_8_bones[vst_tangent] = normal;
		animated_mesh_8_bones[vst_texture_coordinate] =
			texture_coordinate;
		animated_mesh_8_bones[vst_bone_index] = vet_ubyte4;
		animated_mesh_8_bones[vst_bone_weight] =
			vet_ushort4_normalized;
		animated_mesh_8_bones[vst_extra_bone_index] = vet_ubyte4;
		animated_mesh_8_bones[vst_extra_bone_weight] =
			vet_ushort4_normalized;

		morph_mesh[vst_position] = position;
		morph_mesh[vst_normal] = normal;
		morph_mesh[vst_tangent] = normal;
		morph_mesh[vst_texture_coordinate] = texture_coordinate;
		morph_mesh[vst_morph_position] = position;
		morph_mesh[vst_morph_normal] = normal;
		morph_mesh[vst_morph_tangent] = normal;
		morph_mesh[vst_morph_texture_coordinate] = texture_coordinate;

		instanced_mesh_0[vst_position] = position;
		instanced_mesh_0[vst_normal] = normal;
		instanced_mesh_0[vst_tangent] = normal;
		instanced_mesh_0[vst_texture_coordinate] = texture_coordinate;

		instanced_mesh_1[vst_instance_data_0] = vet_float4;
		instanced_mesh_1[vst_instance_data_1] = vet_float4;
		instanced_mesh_1[vst_instance_data_2] = vet_float4;

		cdlod_terrain_1[vst_instance_data_0] = vet_float4;
		cdlod_terrain_1[vst_instance_data_1] = vet_float4;

		sprite[vst_position] = position;
		sprite[vst_texture_coordinate] = vet_ushort2_normalized;

		font[vst_texture_coordinate] = vet_ushort2_normalized;
		font[vst_color] = vet_ubyte4_normalized;

		simple_terrain_1[vst_morph_position] = vet_ushort4_normalized;
		simple_terrain_1[vst_morph_normal] = vet_short4_normalized;

		set_format(vft_sprite, VertexElements(sprite));
		set_format(vft_font, VertexElements(font));

		set_format(vft_mesh, VertexElements(mesh));
		set_format(vft_animated_mesh_2_bones,
			VertexElements(animated_mesh_2_bones));
		set_format(vft_animated_mesh_4_bones,
			VertexElements(animated_mesh_4_bones));
		set_format(vft_animated_mesh_6_bones,
			VertexElements(animated_mesh_6_bones));
		set_format(vft_animated_mesh_8_bones,
			VertexElements(animated_mesh_8_bones));
		set_format(vft_morph_mesh, VertexElements(morph_mesh));

		simple_terrain.push_back(VertexElements(simple_terrain_0));
		simple_terrain.push_back(VertexElements(simple_terrain_1));

		set_format(vft_simple_terrain, simple_terrain);

		instanced_mesh.push_back(VertexElements(instanced_mesh_0));
		instanced_mesh.push_back(VertexElements(instanced_mesh_1));

		set_format(vft_instanced_mesh, instanced_mesh);

		cdlod_terrain.push_back(VertexElements(cdlod_terrain_0));
		cdlod_terrain.push_back(VertexElements(cdlod_terrain_1, 1));

		set_format(vft_cdlod_terrain, cdlod_terrain);

		mesh[vst_texture_coordinate] = extra_texture_coordinate;
		morph_mesh[vst_texture_coordinate] = extra_texture_coordinate;
		morph_mesh[vst_morph_texture_coordinate] =
			extra_texture_coordinate;
		animated_mesh_2_bones[vst_texture_coordinate] =
			extra_texture_coordinate;
		animated_mesh_4_bones[vst_texture_coordinate] =
			extra_texture_coordinate;
		animated_mesh_6_bones[vst_texture_coordinate] =
			extra_texture_coordinate;
		animated_mesh_8_bones[vst_texture_coordinate] =
			extra_texture_coordinate;
		instanced_mesh_0[vst_texture_coordinate] =
			extra_texture_coordinate;

		set_format(vft_mesh_extra_uv, VertexElements(mesh));
		set_format(vft_animated_mesh_2_bones_extra_uv,
			VertexElements(animated_mesh_2_bones));
		set_format(vft_animated_mesh_4_bones_extra_uv,
			VertexElements(animated_mesh_4_bones));
		set_format(vft_animated_mesh_6_bones_extra_uv,
			VertexElements(animated_mesh_6_bones));
		set_format(vft_animated_mesh_8_bones_extra_uv,
			VertexElements(animated_mesh_8_bones));
		set_format(vft_morph_mesh_extra_uv,
			VertexElements(morph_mesh));

		instanced_mesh.clear();
		instanced_mesh.push_back(VertexElements(instanced_mesh_0));
		instanced_mesh.push_back(VertexElements(instanced_mesh_1));

		set_format(vft_instanced_mesh_extra_uv, instanced_mesh);
	}

	MeshBuilder::~MeshBuilder() noexcept
	{
	}

	void MeshBuilder::set_format(const VertexFormatType format,
		const VertexElements &elements)
	{
		m_vertex_formats[format] = boost::make_shared<VertexFormat>(
			get_str(format), elements);
	}

	void MeshBuilder::set_format(const VertexFormatType format,
		const VertexElementsVector &elements)
	{
		m_vertex_formats[format] = boost::make_shared<VertexFormat>(
			get_str(format), elements);
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh(const String &name,
		const bool static_indices, const bool static_vertices,
		const bool static_instances) const
	{
		if (get_global_vars()->get_opengl_3_3())
		{
			return boost::make_shared<OpenGl33Mesh>(
				get_hardware_buffer_mapper(), name,
				static_indices, static_vertices,
				static_instances,
				get_global_vars()->get_use_simd());
		}

		if (get_global_vars()->get_opengl_3_2())
		{
			return boost::make_shared<OpenGl32Mesh>(
				get_hardware_buffer_mapper(), name,
				static_indices, static_vertices,
				static_instances,
				get_global_vars()->get_use_simd());
		}

		if (get_global_vars()->get_opengl_3_1())
		{
			return boost::make_shared<OpenGl31Mesh>(
				get_hardware_buffer_mapper(), name,
				static_indices, static_vertices,
				static_instances,
				get_global_vars()->get_use_simd());
		}

		if (get_global_vars()->get_opengl_3_0() ||
			GLEW_ARB_vertex_array_object)
		{
			return boost::make_shared<OpenGl3Mesh>(
				get_hardware_buffer_mapper(), name,
				static_indices, static_vertices,
				static_instances,
				get_global_vars()->get_use_simd());
		}

		return boost::make_shared<OpenGl2Mesh>(
			get_hardware_buffer_mapper(), name, static_indices,
			static_vertices, static_instances,
			get_global_vars()->get_use_simd());
	}

	AbstractMeshSharedPtr MeshBuilder::get_mesh(
		const VertexFormatType vertex_format,
		const MeshDataToolSharedPtr &mesh_data_tool, const String &name,
		const Uint32 instances, const bool static_indices,
		const bool static_vertices, const bool static_instances) const
	{
		AbstractMeshSharedPtr result;

		result = get_mesh(name, static_indices, static_vertices,
			static_instances);

		result->init(get_vertex_format(vertex_format), mesh_data_tool,
			instances, static_indices, static_vertices,
			static_instances);

		return result;
	}

	VertexFormatConstSharedPtr MeshBuilder::get_vertex_format(
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

	VertexBuffersSharedPtr MeshBuilder::get_vertex_buffers(
		const VertexFormatType vertex_format,
		const Uint32 vertex_count) const
	{
		return VertexBuffersBuilder::get_vertex_buffers(
			get_vertex_format(vertex_format),
			vertex_count, get_global_vars()->get_use_simd());
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

	OutStream &operator<<(OutStream &str,
		const VertexFormatType value)
	{
		str << MeshBuilder::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, VertexFormatType &value)
	{
		std::string string;

		str >> string;

		value = MeshBuilder::get_type(String(string));

		return str;
	}

}
