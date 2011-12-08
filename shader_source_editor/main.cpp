#include <gtk/gtk.h>
#include <string>
#include <sstream>
#include "enumutils.hpp"
#include "../engine/prerequisites.hpp"
#include "../engine/shader/shadersource.hpp"
#include "../engine/shader/shadersourcedata.hpp"
#include "../engine/shader/shadersourceparameter.hpp"
#include "../engine/exceptions.hpp"
#include "../engine/shader/parameterutil.hpp"
#include "../engine/shader/parameterqualifierutil.hpp"
#include "../engine/shader/parametersizeutil.hpp"
#include "../engine/shader/shadersourceutil.hpp"

namespace el = eternal_lands;

const int max_parameter_pages = 4;

typedef struct
{
	GtkWidget* pages[max_parameter_pages];
} NoteBookPages;

G_MODULE_EXPORT void set_pages_visible(GtkSpinButton* spin_button,
	NoteBookPages* data)
{
	gint i;

	for (i = 0; i < max_parameter_pages; i++)
	{
		if (i < gtk_spin_button_get_value_as_int(spin_button))
		{
			gtk_widget_show(data->pages[i]);
		}
		else
		{
			gtk_widget_hide(data->pages[i]);
		}
	}
}

typedef struct
{
	GtkEntry* name_value;
	GtkComboBoxText* name_values;
	GtkComboBoxText* type_values;
	GtkComboBoxText* qualifier_values;
	GtkComboBoxText* size_values;
	GtkSpinButton* scale_values;
} ParameterData;

G_MODULE_EXPORT void name_changed(GtkComboBoxText* name_values,
	ParameterData* data)
{
	gboolean sensitive, qualifier_sensitive;
	gint type, qualifier, size, scale;
	gchar* str;

	sensitive = TRUE;

	qualifier_sensitive = TRUE;

	str = gtk_combo_box_text_get_active_text(name_values);

	if (el::EnumUtils::get_parameter_data(str, type, qualifier,
			size, scale, qualifier_sensitive))
	{
		sensitive = FALSE;

		gtk_combo_box_set_active(GTK_COMBO_BOX(data->type_values),
			type);
		gtk_combo_box_set_active(GTK_COMBO_BOX(data->qualifier_values),
			qualifier);
		gtk_combo_box_set_active(GTK_COMBO_BOX(data->size_values),
			size);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(data->scale_values),
			scale);
	}

	gtk_widget_set_sensitive(GTK_WIDGET(data->type_values), sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(data->qualifier_values),
		qualifier_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(data->size_values), sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(data->scale_values), sensitive);
}

typedef struct
{
	GtkTreeView* parameters;
	GtkListStore* parameters_list;
	GtkDialog* dialog;
	ParameterData* parameter;
} ParametersData;

G_MODULE_EXPORT void parameter_edit(GtkButton* button, ParametersData* data)
{
	GtkTreeIter it;
	gint type, qualifier, size, scale;
	gchar* name;
	gchar* type_str;
	gchar* qualifier_str;
	gchar* size_str;

	if (!gtk_tree_selection_get_selected(gtk_tree_view_get_selection(
		data->parameters), 0, &it))
	{
		return;
	}

	gtk_tree_model_get(GTK_TREE_MODEL(data->parameters_list), &it,
		0, &name,
		1, &type_str,
		2, &qualifier_str,
		3, &size_str,
		4, &scale, -1);

	type = el::EnumUtils::get_parameter_type(type_str);
	qualifier = el::EnumUtils::get_parameter_qualifier_type(qualifier_str);
	size = el::EnumUtils::get_parameter_size_type(size_str);

	gtk_combo_box_set_active(GTK_COMBO_BOX(data->parameter->type_values),
		type);
	gtk_combo_box_set_active(GTK_COMBO_BOX(
		data->parameter->qualifier_values), qualifier);
	gtk_combo_box_set_active(GTK_COMBO_BOX(
		data->parameter->size_values), size);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(
		data->parameter->scale_values), scale);
	gtk_entry_set_text(data->parameter->name_value, name);

	if (gtk_dialog_run(data->dialog) == 1)
	{
		gtk_list_store_set(data->parameters_list, &it,
			0, gtk_combo_box_text_get_active_text(
				data->parameter->name_values),
			1, gtk_combo_box_text_get_active_text(
				data->parameter->type_values),
			2, gtk_combo_box_text_get_active_text(
				data->parameter->qualifier_values),
			3, gtk_combo_box_text_get_active_text(
				data->parameter->size_values),
			4, gtk_spin_button_get_value_as_int(
				data->parameter->scale_values),
                          -1);
	}

	gtk_widget_hide(GTK_WIDGET(data->dialog));
}

G_MODULE_EXPORT void parameter_add(GtkButton* button, ParametersData* data)
{
	GtkTreeIter it;

	if (gtk_dialog_run(data->dialog) == 1)
	{
		gtk_list_store_insert_with_values(data->parameters_list, &it,
			-1,
			0, gtk_combo_box_text_get_active_text(
				data->parameter->name_values),
			1, gtk_combo_box_text_get_active_text(
				data->parameter->type_values),
			2, gtk_combo_box_text_get_active_text(
				data->parameter->qualifier_values),
			3, gtk_combo_box_text_get_active_text(
				data->parameter->size_values),
			4, gtk_spin_button_get_value_as_int(
				data->parameter->scale_values),
			-1);
	}

	gtk_widget_hide(GTK_WIDGET(data->dialog));
}

G_MODULE_EXPORT void parameter_remove(GtkButton* button, ParametersData* data)
{
	GtkTreeIter it;

	if (gtk_tree_selection_get_selected(gtk_tree_view_get_selection(
		data->parameters), 0, &it))
	{
		gtk_list_store_remove(data->parameters_list, &it);
	}
}

G_MODULE_EXPORT void selection_changed(GtkTreeSelection* tree_selection,
	GtkWidget* dest)
{
	gtk_widget_set_sensitive(dest, gtk_tree_selection_get_selected(
		tree_selection, 0, 0));
}

G_MODULE_EXPORT void close_main_window(GtkMenuItem* menu_item, GtkWidget* dest)
{
	gtk_widget_hide(dest);
	gtk_widget_destroy(dest);
}

G_MODULE_EXPORT void show_about(GtkMenuItem* menuitem, GtkAboutDialog* about)
{
	gtk_dialog_run(GTK_DIALOG(about));
	gtk_widget_hide(GTK_WIDGET(about));
}

typedef struct
{
	GtkTreeView* parameters[4];
	GtkListStore* parameters_lists[4];
	GtkTextView* sources[4];
	GtkTextBuffer* text_buffers[4];
	GtkToggleButton* toggle_button_glsl_120[4];
	GtkToggleButton* toggle_button_glsl_150[4];
	GtkToggleButton* toggle_button_material_default[4];
	GtkToggleButton* toggle_button_material_merged[4];
	GtkSpinButton* spin_button_pages;
	GtkComboBoxText* type;
	GtkEntry* name;
	GtkWidget *save_dialog;
	GtkWidget *load_dialog;
	el::String file_name;
} ShaderData;

G_MODULE_EXPORT void load_file(GtkMenuItem* menu_item, ShaderData* data)
{
	el::ShaderSource shader_source;
	el::String name;
	gchar* file_name;
	std::string name_str, type_str, qualifier_str, size_str;
	gint scale;
	GtkTreeIter it;
	gint i, count;

	if (gtk_dialog_run(GTK_DIALOG(data->load_dialog)) !=
		GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_hide(GTK_WIDGET(data->load_dialog));
		return;
	}

	gtk_widget_hide(GTK_WIDGET(data->load_dialog));

	file_name = gtk_file_chooser_get_filename(
		GTK_FILE_CHOOSER(data->load_dialog));

	data->file_name = el::String(el::utf8_to_string(file_name));
	g_free(file_name);

	try
	{
		shader_source.load_xml(data->file_name);
	}
	catch (...)
	{
		return;
	}

	for (i = 0; i < max_parameter_pages; i++)
	{
		gtk_list_store_clear(data->parameters_lists[i]);

		gtk_text_buffer_set_text(data->text_buffers[i], "", -1);

		gtk_toggle_button_set_active(data->toggle_button_glsl_120[i],
			FALSE);
		gtk_toggle_button_set_active(data->toggle_button_glsl_150[i],
			FALSE);
		gtk_toggle_button_set_active(
			data->toggle_button_material_default[i], FALSE);
		gtk_toggle_button_set_active(
			data->toggle_button_material_merged[i], FALSE);
	}

	gtk_entry_set_text(data->name, el::string_to_utf8(
		shader_source.get_name()).c_str());

	count = shader_source.get_datas().size();

	if (count > max_parameter_pages)
	{
		count = max_parameter_pages;
	}

	gtk_spin_button_set_value(data->spin_button_pages, count);

	gtk_combo_box_set_active(GTK_COMBO_BOX(data->type),
		shader_source.get_type());

	for (i = 0; i < count; i++)
	{
		gtk_toggle_button_set_active(data->toggle_button_glsl_120[i],
			shader_source.get_datas()[i].get_glsl_120());
		gtk_toggle_button_set_active(data->toggle_button_glsl_150[i],
			shader_source.get_datas()[i].get_glsl_150());
		gtk_toggle_button_set_active(
			data->toggle_button_material_default[i],
			shader_source.get_datas()[i].get_material_default());
		gtk_toggle_button_set_active(
			data->toggle_button_material_merged[i],
			shader_source.get_datas(
				)[i].get_material_merged());

		gtk_text_buffer_set_text(data->text_buffers[i],
			el::string_to_utf8(shader_source.get_datas(
				)[i].get_source()).c_str(), -1);

		gtk_list_store_clear(data->parameters_lists[i]);

		BOOST_FOREACH(const el::ShaderSourceParameter &parameter,
			shader_source.get_datas()[i].get_parameters())
		{
			name_str = el::string_to_utf8(parameter.get_name());
			type_str = el::string_to_utf8(
				el::ParameterUtil::get_str(
					parameter.get_type()));
			qualifier_str = el::string_to_utf8(
				el::ParameterQualifierUtil::get_str(
					parameter.get_qualifier()));
			size_str = el::string_to_utf8(
				el::ParameterSizeUtil::get_str(
					parameter.get_size()));
			scale = parameter.get_scale();

			gtk_list_store_insert_with_values(
				data->parameters_lists[i],
				&it, -1,
				0, name_str.c_str(),
				1, type_str.c_str(),
				2, qualifier_str.c_str(),
				3, size_str.c_str(),
				4, scale,
				-1);
		}
	}
}

G_MODULE_EXPORT gboolean set_parameters(GtkTreeModel* model, GtkTreePath *path,
	GtkTreeIter* it, gpointer data)
{
	el::ShaderSourceParameter shader_source_parameter;
	gint type, qualifier, size, scale;
	el::String name;
	gchar* name_str;
	gchar* type_str;
	gchar* qualifier_str;
	gchar* size_str;

	gtk_tree_model_get(model, it,
		0, &name_str,
		1, &type_str,
		2, &qualifier_str,
		3, &size_str,
		4, &scale, -1);

	type = el::EnumUtils::get_parameter_type(type_str);
	qualifier = el::EnumUtils::get_parameter_qualifier_type(qualifier_str);
	size = el::EnumUtils::get_parameter_size_type(size_str);
	name = el::String(el::utf8_to_string(name_str));

	g_free(type_str);
	g_free(qualifier_str);
	g_free(size_str);
	g_free(name_str);

	if ((type == -1) || (qualifier == -1) || (size == -1))
	{
		return FALSE;
	}

	shader_source_parameter.set_name(name);
	shader_source_parameter.set_type(static_cast<el::ParameterType>(type));
	shader_source_parameter.set_qualifier(
		static_cast<el::ParameterQualifierType>(qualifier));
	shader_source_parameter.set_size(
		static_cast<el::ParameterSizeType>(size));
	shader_source_parameter.set_scale(scale);

	static_cast<el::ShaderSourceParameterVector*>(data)->push_back(
		shader_source_parameter);

	return FALSE;
}

void do_save_file(ShaderData* data)
{
	el::ShaderSource shader_source;
	el::ShaderSourceData shader_source_data;
	el::ShaderSourceDataVector shader_source_datas;
	el::ShaderSourceParameterVector shader_source_parameters;
	el::String name, source_str;
	gchar* str;
	GtkTextIter start, end;
	gint i, count;

	shader_source.set_name(el::String(el::utf8_to_string(
		gtk_entry_get_text(data->name))));

	count = gtk_spin_button_get_value_as_int(data->spin_button_pages);

	shader_source.set_type(static_cast<el::ShaderSourceType>(
		gtk_combo_box_get_active(GTK_COMBO_BOX(data->type))));

	for (i = 0; i < count; i++)
	{
		shader_source_data.set_glsl_120(gtk_toggle_button_get_active(
			data->toggle_button_glsl_120[i]) == TRUE);
		shader_source_data.set_glsl_150(gtk_toggle_button_get_active(
			data->toggle_button_glsl_150[i]) == TRUE);
		shader_source_data.set_material_default(
			gtk_toggle_button_get_active(
				data->toggle_button_material_default[i])
					== TRUE);
		shader_source_data.set_material_merged(
			gtk_toggle_button_get_active(
				data->toggle_button_material_merged[i])
					== TRUE);

		gtk_text_buffer_get_bounds(data->text_buffers[i], &start, &end);

		str = gtk_text_buffer_get_text(data->text_buffers[i], &start,
			&end, TRUE);
		source_str = el::String(el::utf8_to_string(str));
		g_free(str);

		shader_source_data.set_source(source_str);

		shader_source_parameters.clear();

		gtk_tree_model_foreach(gtk_tree_view_get_model(
			data->parameters[i]), set_parameters,
			&shader_source_parameters);

		shader_source_data.set_parameters(shader_source_parameters);
		shader_source_datas.push_back(shader_source_data);
	}

	shader_source.set_datas(shader_source_datas);

	try
	{
		shader_source.save_xml(data->file_name);
	}
	catch (...)
	{
	}
}

bool get_file_name(ShaderData* data)
{
	gchar *file_name;

	if (data->file_name.get().empty())
	{
		gtk_file_chooser_set_current_name(
			GTK_FILE_CHOOSER(data->save_dialog), "new.xml");
	}
	else
	{
		gtk_file_chooser_set_filename(
			GTK_FILE_CHOOSER(data->save_dialog),
			el::string_to_utf8(data->file_name).c_str());
	}

	if (gtk_dialog_run(GTK_DIALOG(data->save_dialog)) ==
		GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_hide(GTK_WIDGET(data->save_dialog));

		file_name = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(data->save_dialog));

		data->file_name = el::String(el::utf8_to_string(file_name));

		g_free(file_name);

		return true;
	}

	gtk_widget_hide(GTK_WIDGET(data->save_dialog));

	return false;
}

G_MODULE_EXPORT void save_file(GtkMenuItem* menu_item, ShaderData* data)
{
	if (data->file_name.get().empty())
	{
		if (!get_file_name(data))
		{
			return;
		}
	}

	do_save_file(data);
}

G_MODULE_EXPORT void save_as_file(GtkMenuItem* menu_item, ShaderData* data)
{
	if (get_file_name(data))
	{
		do_save_file(data);
	}
}

int main(int argc, char *argv[])
{
	GtkBuilder *builder = 0;
	GtkWindow *window = 0;
	GtkDialog* dialog = 0;
	GtkMenuItem* menu_item = 0;
	GtkSpinButton* spin_button = 0;
	GtkComboBoxText* combo_box_text = 0;
	GtkButton* button = 0;
	GtkTreeSelection* tree_selection = 0;
	GtkFileFilter* filter_xml = 0;
	GtkFileFilter* filter_all = 0;
	GError *error = 0;
	NoteBookPages pages;
	ParametersData parameters[max_parameter_pages];
	ParameterData parameter;
	ShaderData shader_data;
	gint i;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();

	gtk_builder_add_from_file(builder, "main.xml", &error);
	gtk_builder_connect_signals(builder, 0);

	window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
	menu_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,
		"menu_item_quit"));
	g_signal_connect(menu_item, "activate", G_CALLBACK(close_main_window),
		GTK_WIDGET(window));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

	menu_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,
		"menu_item_about"));
	dialog = GTK_DIALOG(gtk_builder_get_object(builder, "about_dialog"));
	g_signal_connect(menu_item, "activate", G_CALLBACK(show_about), dialog);

	spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,
		"spin_button_pages"));

	shader_data.spin_button_pages = spin_button;

	g_signal_connect(spin_button, "value-changed",
		G_CALLBACK(set_pages_visible), &pages);

	combo_box_text = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder,
		"type"));
	shader_data.type = combo_box_text;

	el::EnumUtils::append_source_types(combo_box_text);
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box_text), 0);

	parameter.name_value = GTK_ENTRY(gtk_builder_get_object(builder,
		"name_value"));
	parameter.name_values = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(
		builder, "name_values"));
	parameter.type_values = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(
		builder, "type_values"));
	parameter.qualifier_values = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(
		builder, "qualifier_values"));
	parameter.size_values = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(
		builder, "size_values"));
	parameter.scale_values = GTK_SPIN_BUTTON(gtk_builder_get_object(
		builder, "scale_values"));

	el::EnumUtils::append_parameter_names(parameter.name_values);
	g_signal_connect(parameter.name_values, "changed",
		G_CALLBACK(name_changed), &parameter);

	el::EnumUtils::append_parameter_types(parameter.type_values);
	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.type_values), 0);

	el::EnumUtils::append_parameter_qualifier_types(
		parameter.qualifier_values);
	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.qualifier_values), 0);

	el::EnumUtils::append_parameter_size_types(parameter.size_values);
	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.size_values), 0);

	dialog = GTK_DIALOG(gtk_builder_get_object(builder,
		"parameter_dialog"));

	shader_data.name = GTK_ENTRY(gtk_builder_get_object(builder, "name"));

	for (i = 0; i < max_parameter_pages; i++)
	{
		std::stringstream str;
		std::string name;

		str << i;

		name = "page_";
		name += str.str();

		pages.pages[i] = GTK_WIDGET(gtk_builder_get_object(builder,
			name.c_str()));

		name = "parameters_";
		name += str.str();

		parameters[i].parameters = GTK_TREE_VIEW(
			gtk_builder_get_object(builder, name.c_str()));

		shader_data.parameters[i] = parameters[i].parameters;

		tree_selection = gtk_tree_view_get_selection(
			parameters[i].parameters);

		name = "parameters_list_";
		name += str.str();

		parameters[i].parameters_list = GTK_LIST_STORE(
			gtk_builder_get_object(builder, name.c_str()));
		shader_data.parameters_lists[i] = parameters[i].parameters_list;
		parameters[i].dialog = dialog;
		parameters[i].parameter = &parameter;

		name = "add_";
		name += str.str();

		button = GTK_BUTTON(gtk_builder_get_object(builder,
			name.c_str()));
		g_signal_connect(button, "clicked", G_CALLBACK(parameter_add),
			&parameters[i]);

		name = "edit_";
		name += str.str();

		button = GTK_BUTTON(gtk_builder_get_object(builder,
			name.c_str()));
		g_signal_connect(button, "clicked", G_CALLBACK(parameter_edit),
			&parameters[i]);
		g_signal_connect(tree_selection, "changed",
			G_CALLBACK(selection_changed), button);

		name = "remove_";
		name += str.str();

		button = GTK_BUTTON(gtk_builder_get_object(builder,
			name.c_str()));
		g_signal_connect(button, "clicked",
			G_CALLBACK(parameter_remove), &parameters[i]);
		g_signal_connect(tree_selection, "changed",
			G_CALLBACK(selection_changed), button);

		name = "sources_";
		name += str.str();

		shader_data.sources[i] = GTK_TEXT_VIEW(gtk_builder_get_object(
			builder, name.c_str()));

		name = "toggle_button_glsl_120_";
		name += str.str();

		shader_data.toggle_button_glsl_120[i] = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "toggle_button_glsl_150_";
		name += str.str();

		shader_data.toggle_button_glsl_150[i] = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "toggle_button_material_default_";
		name += str.str();

		shader_data.toggle_button_material_default[i] =
			GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,
				name.c_str()));

		name = "toggle_button_material_merged_";
		name += str.str();

		shader_data.toggle_button_material_merged[i] =
			GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,
				name.c_str()));

		name = "text_buffer_";
		name += str.str();

		shader_data.text_buffers[i] = GTK_TEXT_BUFFER(
			gtk_builder_get_object(builder,	name.c_str()));
	}

	filter_xml = GTK_FILE_FILTER(gtk_builder_get_object(builder,
		"xml_file_filter"));

	filter_all = GTK_FILE_FILTER(gtk_builder_get_object(builder,
		"all_file_filter"));

	gtk_file_filter_set_name(filter_xml, "xml files");
	gtk_file_filter_set_name(filter_all, "all files");

	shader_data.save_dialog = gtk_file_chooser_dialog_new ("Save File",
		window, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(shader_data.save_dialog),
		filter_xml);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(shader_data.save_dialog),
		filter_all);
	gtk_file_chooser_set_do_overwrite_confirmation(
		GTK_FILE_CHOOSER(shader_data.save_dialog), TRUE);

	shader_data.load_dialog = gtk_file_chooser_dialog_new("Open File",
		window, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(shader_data.load_dialog),
		filter_xml);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(shader_data.load_dialog),
		filter_all);

	menu_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,
		"menu_item_open_file"));
	g_signal_connect(menu_item, "activate", G_CALLBACK(load_file),
		&shader_data);

	menu_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,
		"menu_item_save_file"));
	g_signal_connect(menu_item, "activate", G_CALLBACK(save_file),
		&shader_data);

	menu_item = GTK_MENU_ITEM(gtk_builder_get_object(builder,
		"menu_item_save_as_file"));
	g_signal_connect(menu_item, "activate", G_CALLBACK(save_as_file),
		&shader_data);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();

	return 0;
}
