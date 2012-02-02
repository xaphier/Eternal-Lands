/****************************************************************************
 *            enumutils.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "enumutils.hpp"
#include "../engine/prerequisites.hpp"
#include "../engine/shader/autoparameterutil.hpp"
#include "../engine/shader/commonparameterutil.hpp"
#include "../engine/shader/parameterutil.hpp"
#include "../engine/shader/parameterqualifierutil.hpp"
#include "../engine/shader/parametersizeutil.hpp"
#include "../engine/shader/shadersourceutil.hpp"
#include "../engine/shader/shadertextureutil.hpp"
#include "../engine/exceptions.hpp"
#include "../engine/vertexelements.hpp"

namespace eternal_lands
{

	namespace
	{

		void get_data(const CommonParameterType common_parameter,
			const ParameterQualifierType parameter_qualifier,
			std::string &name, std::string &type,
			std::string &qualifier, std::string &size, gint &scale)
		{
			name = CommonParameterUtil::get_str(common_parameter);
			type = ParameterUtil::get_str(
				CommonParameterUtil::get_type(
					common_parameter));
			qualifier = ParameterQualifierUtil::get_str(
				parameter_qualifier);
			size = ParameterSizeUtil::get_str(pst_one);
			scale = 1;
		}

		void get_data(const AutoParameterType auto_parameter,
			std::string &name, std::string &type,
			std::string &qualifier, std::string &size, gint &scale)
		{
			name = AutoParameterUtil::get_str(auto_parameter);
			type = ParameterUtil::get_str(
				AutoParameterUtil::get_type(auto_parameter));
			qualifier = ParameterQualifierUtil::get_str(pqt_in);
			size = ParameterSizeUtil::get_str(pst_one);
			scale = AutoParameterUtil::get_scale(auto_parameter);
		}

		void get_data(const VertexSemanticType vertex_element,
			const ParameterType parameter_type,
			std::string &name, std::string &type,
			std::string &qualifier, std::string &size, gint &scale)
		{
			name = VertexElement::get_str(vertex_element);
			type = ParameterUtil::get_str(parameter_type);
			qualifier = ParameterQualifierUtil::get_str(pqt_in);
			size = ParameterSizeUtil::get_str(pst_one);
			scale = 1;
		}

	}

	void EnumUtils::append_source_types(GtkListStore *list_store)
	{
		GtkTreeIter it;
		std::string name;
		Uint32 i, count;

		count = ShaderSourceUtil::get_shader_source_count();

		for (i = 0; i < count; i++)
		{
			name = ShaderSourceUtil::get_str(
				static_cast<ShaderSourceType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}
	}

	void EnumUtils::append_parameter_names(GtkListStore *list_store)
	{
		GtkTreeIter it;
		std::string name;
		Uint32 i, count;

		count = AutoParameterUtil::get_auto_parameter_count();

		for (i = 0; i < count; i++)
		{
			name = AutoParameterUtil::get_str(
				static_cast<AutoParameterType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}

		count = CommonParameterUtil::get_common_parameter_count();

		for (i = 0; i < count; i++)
		{
			name = CommonParameterUtil::get_str(
				static_cast<CommonParameterType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}

		count = ShaderTextureUtil::get_shader_texture_count();

		for (i = 0; i < count; i++)
		{
			name = ShaderTextureUtil::get_str(
				static_cast<ShaderTextureType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}

		count = VertexElement::get_vertex_semantic_count();

		for (i = 0; i < count; i++)
		{
			name = VertexElement::get_str(
				static_cast<VertexSemanticType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}
	}

	void EnumUtils::append_parameter_types(const gboolean sampler_types,
		GtkListStore* list_store)
	{
		GtkTreeIter it;
		std::string name;
		Uint32 i, count, index;
		bool sampler;

		count = ParameterUtil::get_parameter_count();

		sampler = sampler_types == TRUE;
		index = 0;

		for (i = 0; i < count; i++)
		{
			if (sampler == ParameterUtil::get_sampler(
				static_cast<ParameterType>(i)))
			{
				name = ParameterUtil::get_str(
					static_cast<ParameterType>(i));

				gtk_list_store_insert_with_values(list_store,
					&it,
					-1,
					0, name.c_str(),
					1, i,
					2, index,
					-1);

				index++;
			}
		}
	}

	void EnumUtils::append_parameter_qualifier_types(
		GtkListStore* list_store)
	{
		GtkTreeIter it;
		std::string name;
		Uint32 i, count;

		count = ParameterQualifierUtil::get_parameter_qualifier_count();

		for (i = 0; i < count; i++)
		{
			name = ParameterQualifierUtil::get_str(
				static_cast<ParameterQualifierType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}
	}

	void EnumUtils::append_parameter_size_types(GtkListStore* list_store)
	{
		GtkTreeIter it;
		std::string name;
		Uint32 i, count;

		count = ParameterSizeUtil::get_parameter_size_count();

		for (i = 0; i < count; i++)
		{
			name = ParameterSizeUtil::get_str(
				static_cast<ParameterSizeType>(i));

			gtk_list_store_insert_with_values(list_store, &it,
				-1,
				0, name.c_str(),
				-1);
		}
	}

	bool EnumUtils::get_parameter_data(const gchar* str, gint &type,
		gint &qualifier, gint &size, gint &scale,
		gboolean &type_selection, gboolean &qualifier_selection,
		gboolean &sampler_types)
	{
		String name;
		AutoParameterType auto_parameter;
		CommonParameterType common_parameter;
		ShaderTextureType shader_texture;
		VertexSemanticType vertex_semantic;

		name = String(str);

		if (AutoParameterUtil::get_auto_parameter(name,
			auto_parameter))
		{
			type = AutoParameterUtil::get_type(auto_parameter);
			qualifier = pqt_in;
			size = AutoParameterUtil::get_size(auto_parameter);
			scale = AutoParameterUtil::get_scale(auto_parameter);

			qualifier_selection = FALSE;
			type_selection = FALSE;
			sampler_types = FALSE;

			return true;
		}

		if (CommonParameterUtil::get_common_parameter(name,
			common_parameter))
		{
			type = CommonParameterUtil::get_type(common_parameter);
			qualifier = pqt_in;
			size = CommonParameterUtil::get_size(common_parameter);
			scale = CommonParameterUtil::get_scale(common_parameter);

			qualifier_selection = TRUE;
			type_selection = FALSE;
			sampler_types = FALSE;

			return true;
		}

		if (ShaderTextureUtil::get_shader_texture(name, shader_texture))
		{
			type = pt_sampler2D;
			qualifier = pqt_in;
			size = pst_one;
			scale = 1;

			qualifier_selection = FALSE;
			type_selection = TRUE;
			sampler_types = TRUE;

			return true;
		}

		if (VertexElement::get_vertex_semantic(name, vertex_semantic))
		{
			type = pt_vec4;
			qualifier = pqt_in;
			size = pst_one;
			scale = 1;

			qualifier_selection = FALSE;
			type_selection = TRUE;
			sampler_types = FALSE;

			return true;
		}

		return false;
	}

	gint EnumUtils::get_parameter_type(const gchar* str,
		gboolean &sampler_type)
	{
		String name;
		ParameterType parameter;

		name = String(str);

		if (ParameterUtil::get_parameter(name, parameter))
		{
			if (ParameterUtil::get_sampler(parameter))
			{
				sampler_type = TRUE;
			}
			else
			{
				sampler_type = FALSE;
			}

			return parameter;
		}

		return -1;
	}

	gint EnumUtils::get_parameter_qualifier_type(const gchar* str)
	{
		String name;
		ParameterQualifierType parameter_qualifier;

		name = String(str);

		if (ParameterQualifierUtil::get_parameter_qualifier(name,
			parameter_qualifier))
		{
			return parameter_qualifier;
		}

		return -1;
	}

	gint EnumUtils::get_parameter_size_type(const gchar* str)
	{
		String name;
		ParameterSizeType parameter_size;

		name = String(str);

		if (ParameterSizeUtil::get_parameter_size(name, parameter_size))
		{
			return parameter_size;
		}

		return -1;
	}

	bool EnumUtils::get_default_parameter_data(const gint source_type,
		const guint index, std::string &name, std::string &type,
		std::string &qualifier, std::string &size, gint &scale)
	{
		switch (source_type)
		{
			case sst_world_depth_transformation:
				if (index == 0)
				{
					get_data(cpt_world_position, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(vst_position, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(apt_world_transformation, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_world_normal_transformation:
				if (index == 0)
				{
					get_data(cpt_world_position, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_world_normal, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(vst_position, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 3)
				{
					get_data(vst_normal, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 4)
				{
					get_data(apt_world_transformation, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_world_tangent_transformation:
				if (index == 0)
				{
					get_data(cpt_world_position, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_world_normal, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(cpt_world_tangent, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 3)
				{
					get_data(vst_position, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 4)
				{
					get_data(vst_normal, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 5)
				{
					get_data(vst_tangent, pt_vec3,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 6)
				{
					get_data(apt_world_transformation, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_view_direction:
				if (index == 0)
				{
					get_data(cpt_world_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_world_view_direction,
						pqt_out, name, type, qualifier,
						size, scale);
					return true;
				}
				return false;
			case sst_view_transformation:
				if (index == 0)
				{
					get_data(cpt_world_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_view_position, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(apt_view_matrix, name, type,
						qualifier, size, scale);
					return true;
				}
				return false;
			case sst_fog:
				if (index == 0)
				{
					get_data(cpt_view_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_fog, pqt_out, name, type,
						qualifier, size, scale);
					return true;
				}
				if (index == 2)
				{
					get_data(apt_fog_data, name, type,
						qualifier, size, scale);
					return true;
				}
				return false;
			case sst_vertex_light:
				if (index == 0)
				{
					get_data(cpt_light_color, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_light_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(cpt_world_normal, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 3)
				{
					get_data(cpt_world_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 4)
				{
					get_data(cpt_diffuse_color, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_fragment_light:
				if (index == 0)
				{
					get_data(cpt_light_color, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_light_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(cpt_fragment_normal, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 3)
				{
					get_data(cpt_world_position, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 4)
				{
					get_data(cpt_diffuse_color, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 5)
				{
					get_data(cpt_specular_color, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_uv:
				if (index == 0)
				{
					get_data(cpt_world_uv, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(vst_texture_coordinate_0,
						pt_vec2, name, type, qualifier,
						size, scale);
					return true;
				}
				return false;
			case sst_shadow_uv:
				if (index == 0)
				{
					get_data(cpt_shadow_uv, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(apt_shadow_texture_matrix,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_shadow_mapping:
				if (index == 0)
				{
					get_data(cpt_shadow_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_shadow, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 2)
				{
					get_data(apt_shadow_camera,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_normal_mapping:
				if (index == 0)
				{
					get_data(cpt_world_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_tbn_matrix, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 2)
				{
					get_data(cpt_fragment_uv, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 3)
				{
					get_data(cpt_fragment_normal, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_normal_depth_mapping:
				if (index == 0)
				{
					get_data(cpt_world_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_fragment_uv, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_albedo_mapping:
				if (index == 0)
				{
					get_data(cpt_fragment_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_albedo, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_specular_mapping:
				if (index == 0)
				{
					get_data(cpt_fragment_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_specular, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_transparent:
				if (index == 0)
				{
					get_data(cpt_albedo, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			case sst_shadow_map:
				if (index == 0)
				{
					get_data(cpt_shadow_map_data, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_tbn_matrix:
				if (index == 0)
				{
					get_data(cpt_world_normal, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_world_tangent, pqt_in,
						name, type, qualifier, size,
						scale);
					return true;
				}
				if (index == 2)
				{
					get_data(cpt_tbn_matrix, pqt_out,
						name, type, qualifier, size,
						scale);
					return true;
				}
				return false;
			case sst_emission_mapping:
				if (index == 0)
				{
					get_data(cpt_fragment_uv, pqt_in, name,
						type, qualifier, size, scale);
					return true;
				}
				if (index == 1)
				{
					get_data(cpt_emission, pqt_out, name,
						type, qualifier, size, scale);
					return true;
				}
				return false;
			default:
				return false;
		}
	}

}
