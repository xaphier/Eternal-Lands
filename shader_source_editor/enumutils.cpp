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
#include "../engine/exceptions.hpp"

namespace eternal_lands
{

	void EnumUtils::append_source_types(GtkComboBoxText* combo_box_text)
	{
		std::string name;
		Uint32 i, count;

		count = ShaderSourceUtil::get_shader_source_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(ShaderSourceUtil::get_str(
				static_cast<ShaderSourceType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}
	}

	void EnumUtils::append_parameter_names(GtkComboBoxText* combo_box_text)
	{
		std::string name;
		Uint32 i, count;

		count = AutoParameterUtil::get_auto_parameter_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(AutoParameterUtil::get_str(
				static_cast<AutoParameterType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}

		count = CommonParameterUtil::get_common_parameter_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(CommonParameterUtil::get_str(
				static_cast<CommonParameterType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}
	}

	void EnumUtils::append_parameter_types(GtkComboBoxText* combo_box_text)
	{
		std::string name;
		Uint32 i, count;

		count = ParameterUtil::get_parameter_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(ParameterUtil::get_str(
				static_cast<ParameterType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}
	}

	void EnumUtils::append_parameter_qualifier_types(
		GtkComboBoxText* combo_box_text)
	{
		std::string name;
		Uint32 i, count;

		count = ParameterQualifierUtil::get_parameter_qualifier_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(ParameterQualifierUtil::get_str(
				static_cast<ParameterQualifierType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}
	}

	void EnumUtils::append_parameter_size_types(
		GtkComboBoxText* combo_box_text)
	{
		std::string name;
		Uint32 i, count;

		count = ParameterSizeUtil::get_parameter_size_count();

		for (i = 0; i < count; i++)
		{
			name = string_to_utf8(ParameterSizeUtil::get_str(
				static_cast<ParameterSizeType>(i)));

			gtk_combo_box_text_append_text(combo_box_text,
				name.c_str());
		}
	}

	bool EnumUtils::get_parameter_data(const gchar* str, gint &type,
		gint &qualifier, gint &size, gint &scale,
		gboolean &qualifier_selection)
	{
		String name;
		AutoParameterType auto_parameter;
		CommonParameterType common_parameter;

		name = String(utf8_to_string(str));

		if (AutoParameterUtil::get_auto_parameter(name,
			auto_parameter))
		{
			type = AutoParameterUtil::get_type(auto_parameter);
			qualifier = pqt_in;
			size = AutoParameterUtil::get_size(auto_parameter);
			scale = AutoParameterUtil::get_scale(auto_parameter);

			qualifier_selection = FALSE;

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

			return true;
		}

		return false;
	}

	gint EnumUtils::get_parameter_type(const gchar* str)
	{
		String name;
		ParameterType parameter;

		name = String(utf8_to_string(str));

		if (ParameterUtil::get_parameter(name, parameter))
		{
			return parameter;
		}

		return -1;
	}

	gint EnumUtils::get_parameter_qualifier_type(const gchar* str)
	{
		String name;
		ParameterQualifierType parameter_qualifier;

		name = String(utf8_to_string(str));

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

		name = String(utf8_to_string(str));

		if (ParameterSizeUtil::get_parameter_size(name, parameter_size))
		{
			return parameter_size;
		}

		return -1;
	}

}
