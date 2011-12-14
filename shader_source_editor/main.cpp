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

const int max_parameter_pages = 5;

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
	GtkComboBox* name_values;
	GtkComboBox* type_values;
	GtkComboBox* qualifier_values;
	GtkComboBox* size_values;
	GtkSpinButton* scale_values;
	GtkListStore* sampler_types;
	GtkListStore* no_sampler_types;
} ParameterData;

G_MODULE_EXPORT gboolean get_index(GtkTreeModel* model, GtkTreePath *path,
	GtkTreeIter* it, gpointer data)
{
	gchar* name_str;
	gint id, index;

	gtk_tree_model_get(model, it,
		0, &name_str,
		1, &id,
		2, &index,
		-1);

	g_free(name_str);

	if (id == *((gint*)data))
	{
		*((gint*)data) = index;
		return TRUE;
	}

	return FALSE;
}

G_MODULE_EXPORT void name_changed(GtkComboBox* name_values,
	ParameterData* data)
{
	gboolean sensitive, type_sensitive, qualifier_sensitive;
	gboolean sampler_types;
	gint type, qualifier, size, scale;
	const gchar* str;

	sensitive = TRUE;

	type_sensitive = TRUE;
	qualifier_sensitive = TRUE;

	str = gtk_entry_get_text(data->name_value);

	if (el::EnumUtils::get_parameter_data(str, type, qualifier,
			size, scale, type_sensitive, qualifier_sensitive,
			sampler_types))
	{
		sensitive = FALSE;

		if (sampler_types == TRUE)
		{
			gtk_combo_box_set_model(data->type_values,
				GTK_TREE_MODEL(data->sampler_types));
		}
		else
		{
			gtk_combo_box_set_model(data->type_values,
				GTK_TREE_MODEL(data->no_sampler_types));
		}

		gtk_tree_model_foreach(gtk_combo_box_get_model(
			data->type_values), get_index, &type);

		gtk_combo_box_set_active(data->type_values, type);
		gtk_combo_box_set_active(GTK_COMBO_BOX(data->qualifier_values),
			qualifier);
		gtk_combo_box_set_active(GTK_COMBO_BOX(data->size_values),
			size);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(data->scale_values),
			scale);
	}
	else
	{
		if (gtk_combo_box_get_model(data->type_values) !=
			GTK_TREE_MODEL(data->no_sampler_types))
		{
			gtk_combo_box_set_model(data->type_values,
				GTK_TREE_MODEL(data->no_sampler_types));
			gtk_combo_box_set_active(data->type_values, 0);
		}
	}

	gtk_widget_set_sensitive(GTK_WIDGET(data->type_values),
		type_sensitive);
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
	GtkTreeIter it, type_it, qualifier_it, size_it;
	gint type, qualifier, size, scale;
	gboolean sampler_type;
	gchar* name;
	gchar* type_str;
	gchar* qualifier_str;
	gchar* size_str;
	const gchar* str;

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

	type = el::EnumUtils::get_parameter_type(type_str, sampler_type);
	qualifier = el::EnumUtils::get_parameter_qualifier_type(qualifier_str);
	size = el::EnumUtils::get_parameter_size_type(size_str);

	if (sampler_type == TRUE)
	{
		gtk_combo_box_set_model(data->parameter->type_values,
			GTK_TREE_MODEL(data->parameter->sampler_types));
	}
	else
	{
		gtk_combo_box_set_model(data->parameter->type_values,
			GTK_TREE_MODEL(data->parameter->no_sampler_types));
	}

	gtk_tree_model_foreach(gtk_combo_box_get_model(
		data->parameter->type_values), get_index, &type);

	gtk_combo_box_set_active(data->parameter->type_values, type);

	gtk_combo_box_set_active(GTK_COMBO_BOX(
		data->parameter->qualifier_values), qualifier);
	gtk_combo_box_set_active(GTK_COMBO_BOX(
		data->parameter->size_values), size);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(
		data->parameter->scale_values), scale);
	gtk_entry_set_text(data->parameter->name_value, name);

	g_free(name);
	g_free(type_str);
	g_free(qualifier_str);
	g_free(size_str);

	if (gtk_dialog_run(data->dialog) == 1)
	{
		str = gtk_entry_get_text(data->parameter->name_value);

		gtk_combo_box_get_active_iter(data->parameter->type_values,
			&type_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->type_values), &type_it,
			0, &type_str, -1);

		gtk_combo_box_get_active_iter(data->parameter->qualifier_values,
			&qualifier_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->qualifier_values), &qualifier_it,
			0, &qualifier_str, -1);

		gtk_combo_box_get_active_iter(data->parameter->size_values,
			&size_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->size_values), &size_it,
			0, &size_str, -1);

		gtk_list_store_set(data->parameters_list, &it,
			0, str,
			1, type_str,
			2, qualifier_str,
			3, size_str,
			4, gtk_spin_button_get_value_as_int(
				data->parameter->scale_values),
			-1);

		g_free(type_str);
		g_free(qualifier_str);
		g_free(size_str);
	}

	gtk_widget_hide(GTK_WIDGET(data->dialog));
}

G_MODULE_EXPORT void parameter_add(GtkButton* button, ParametersData* data)
{
	GtkTreeIter it, type_it, qualifier_it, size_it;
	gchar* type_str;
	gchar* qualifier_str;
	gchar* size_str;
	const gchar* str;

	if (gtk_dialog_run(data->dialog) == 1)
	{
		str = gtk_entry_get_text(data->parameter->name_value);

		gtk_combo_box_get_active_iter(data->parameter->type_values,
			&type_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->type_values), &type_it,
			0, &type_str, -1);

		gtk_combo_box_get_active_iter(data->parameter->qualifier_values,
			&qualifier_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->qualifier_values), &qualifier_it,
			0, &qualifier_str, -1);

		gtk_combo_box_get_active_iter(data->parameter->size_values,
			&size_it);

		gtk_tree_model_get(gtk_combo_box_get_model(
			data->parameter->size_values), &size_it,
			0, &size_str, -1);

		gtk_list_store_insert_with_values(data->parameters_list, &it,
		 	-1,
			0, str,
			1, type_str,
			2, qualifier_str,
			3, size_str,
			4, gtk_spin_button_get_value_as_int(
				data->parameter->scale_values),
			-1);

		g_free(type_str);
		g_free(qualifier_str);
		g_free(size_str);
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

G_MODULE_EXPORT void close_main_window(GtkAction* action, GtkWidget* dest)
{
	gtk_widget_hide(dest);
	gtk_widget_destroy(dest);
}

G_MODULE_EXPORT void show_about(GtkAction* action, GtkAboutDialog* about)
{
	gtk_dialog_run(GTK_DIALOG(about));
	gtk_widget_hide(GTK_WIDGET(about));
}

typedef struct
{
	GtkTreeView* parameters;
	GtkListStore* parameters_list;
	GtkTextView* source;
	GtkTextBuffer* text_buffer;
	GtkToggleButton* glsl_120;
	GtkToggleButton* glsl_130;
	GtkToggleButton* glsl_140;
	GtkToggleButton* glsl_150;
	GtkToggleButton* glsl_330;
} PageData;

typedef struct
{
	PageData pages[max_parameter_pages];
} PagesData;

G_MODULE_EXPORT void clear_pages(PagesData* pages)
{
	gint i;

	for (i = 0; i < max_parameter_pages; i++)
	{
		gtk_list_store_clear(pages->pages[i].parameters_list);

		gtk_text_buffer_set_text(pages->pages[i].text_buffer,
			"", -1);

		gtk_toggle_button_set_active(pages->pages[i].glsl_120,
			FALSE);
		gtk_toggle_button_set_active(pages->pages[i].glsl_130,
			FALSE);
		gtk_toggle_button_set_active(pages->pages[i].glsl_140,
			FALSE);
		gtk_toggle_button_set_active(pages->pages[i].glsl_150,
			FALSE);
		gtk_toggle_button_set_active(pages->pages[i].glsl_330,
			FALSE);
	}
}

typedef struct
{
	GtkToggleButton* add_default_parameter;
	GtkSpinButton* new_pages;
	GtkComboBox* new_type;
	GtkEntry* new_name;
	GtkDialog* dialog;
	GtkSpinButton* pages;
	GtkComboBox* type;
	GtkEntry* name;
	PagesData* data;
} NewData;

G_MODULE_EXPORT void new_file(GtkAction* action, NewData* data)
{
	GtkTreeIter it;
	std::string name_str, type_str, qualifier_str, size_str;
	gint i, count, type, index, scale;

	if (gtk_dialog_run(data->dialog) != 1)
	{
		gtk_widget_hide(GTK_WIDGET(data->dialog));
		return;
	}

	gtk_widget_hide(GTK_WIDGET(data->dialog));

	clear_pages(data->data);

	count = gtk_spin_button_get_value_as_int(data->new_pages);
	gtk_spin_button_set_value(data->pages, count);

	type = gtk_combo_box_get_active(data->new_type);
	gtk_combo_box_set_active(data->type, type);

	gtk_entry_set_text(data->name, gtk_entry_get_text(data->new_name));

	switch (count)
	{
		case 1:
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_120, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_130, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_140, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_150, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_330, TRUE);
			break;
		case 2:
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_120, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_130, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_140, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_150, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_330, TRUE);
			break;
		case 3:
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_120, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_130, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[2].glsl_140, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[2].glsl_150, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[2].glsl_330, TRUE);
			break;
		case 4:
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_120, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_130, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[2].glsl_140, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[3].glsl_150, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[3].glsl_330, TRUE);
			break;
		case 5:
			gtk_toggle_button_set_active(
				data->data->pages[0].glsl_120, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[1].glsl_130, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[2].glsl_140, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[3].glsl_150, TRUE);
			gtk_toggle_button_set_active(
				data->data->pages[4].glsl_330, TRUE);
			break;
	}

	if (gtk_toggle_button_get_active(data->add_default_parameter) != TRUE)
	{
		return;
	}

	index = 0;

	while (el::EnumUtils::get_default_parameter_data(type, index, name_str,
		type_str, qualifier_str, size_str, scale))
	{
		for (i = 0; i < max_parameter_pages; i++)
		{
			gtk_list_store_insert_with_values(
				data->data->pages[i].parameters_list,
				&it, -1,
				0, name_str.c_str(),
				1, type_str.c_str(),
				2, qualifier_str.c_str(),
				3, size_str.c_str(),
				4, scale,
				-1);
		}

		index++;
	}
}

typedef struct
{
	PagesData* data;
	GtkSpinButton* pages;
	GtkComboBox* type;
	GtkEntry* name;
	GtkWidget *save_dialog;
	GtkWidget *load_dialog;
	el::String file_name;
} ShaderData;

G_MODULE_EXPORT void load_file(GtkAction* action, ShaderData* data)
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

	data->file_name = el::String(file_name);
	g_free(file_name);

	try
	{
		shader_source.load_xml(data->file_name);
	}
	catch (const boost::exception &exception)
	{
		std::cout << boost::diagnostic_information(exception) << std::endl;
		return;
	}
	catch (const std::exception &exception)
	{
		std::cout << exception.what() << std::endl;
		return;
	}
	catch (...)
	{
		return;
	}

	clear_pages(data->data);

	gtk_entry_set_text(data->name, shader_source.get_name().get().c_str());

	count = shader_source.get_datas().size();

	if (count > max_parameter_pages)
	{
		count = max_parameter_pages;
	}

	gtk_spin_button_set_value(data->pages, count);

	gtk_combo_box_set_active(data->type, shader_source.get_type());

	for (i = 0; i < count; i++)
	{
		gtk_toggle_button_set_active(data->data->pages[i].glsl_120,
			shader_source.get_datas()[i].get_glsl_120());
		gtk_toggle_button_set_active(data->data->pages[i].glsl_130,
			shader_source.get_datas()[i].get_glsl_130());
		gtk_toggle_button_set_active(data->data->pages[i].glsl_140,
			shader_source.get_datas()[i].get_glsl_140());
		gtk_toggle_button_set_active(data->data->pages[i].glsl_150,
			shader_source.get_datas()[i].get_glsl_150());
		gtk_toggle_button_set_active(data->data->pages[i].glsl_330,
			shader_source.get_datas()[i].get_glsl_330());

		gtk_text_buffer_set_text(data->data->pages[i].text_buffer,
			shader_source.get_datas()[i].get_source().get().c_str(),
			-1);

		gtk_list_store_clear(data->data->pages[i].parameters_list);

		BOOST_FOREACH(const el::ShaderSourceParameter &parameter,
			shader_source.get_datas()[i].get_parameters())
		{
			name_str = parameter.get_name();
			type_str = el::ParameterUtil::get_str(
				parameter.get_type());
			qualifier_str = el::ParameterQualifierUtil::get_str(
				parameter.get_qualifier());
			size_str = el::ParameterSizeUtil::get_str(
				parameter.get_size());
			scale = parameter.get_scale();

			gtk_list_store_insert_with_values(
				data->data->pages[i].parameters_list,
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
	gboolean sampler_type;
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

	type = el::EnumUtils::get_parameter_type(type_str, sampler_type);
	qualifier = el::EnumUtils::get_parameter_qualifier_type(qualifier_str);
	size = el::EnumUtils::get_parameter_size_type(size_str);
	name = el::String(name_str);

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

	shader_source.set_name(el::String(gtk_entry_get_text(data->name)));

	count = gtk_spin_button_get_value_as_int(data->pages);

	shader_source.set_type(static_cast<el::ShaderSourceType>(
		gtk_combo_box_get_active(GTK_COMBO_BOX(data->type))));

	for (i = 0; i < count; i++)
	{
		shader_source_data.set_glsl_120(gtk_toggle_button_get_active(
			data->data->pages[i].glsl_120) == TRUE);
		shader_source_data.set_glsl_130(gtk_toggle_button_get_active(
			data->data->pages[i].glsl_130) == TRUE);
		shader_source_data.set_glsl_140(gtk_toggle_button_get_active(
			data->data->pages[i].glsl_140) == TRUE);
		shader_source_data.set_glsl_150(gtk_toggle_button_get_active(
			data->data->pages[i].glsl_150) == TRUE);
		shader_source_data.set_glsl_330(gtk_toggle_button_get_active(
			data->data->pages[i].glsl_330) == TRUE);

		gtk_text_buffer_get_bounds(data->data->pages[i].text_buffer,
			&start, &end);

		str = gtk_text_buffer_get_text(
			data->data->pages[i].text_buffer, &start, &end, TRUE);
		source_str = el::String(str);
		g_free(str);

		shader_source_data.set_source(source_str);

		shader_source_parameters.clear();

		gtk_tree_model_foreach(gtk_tree_view_get_model(
			data->data->pages[i].parameters), set_parameters,
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
			data->file_name.get().c_str());
	}

	if (gtk_dialog_run(GTK_DIALOG(data->save_dialog)) ==
		GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_hide(GTK_WIDGET(data->save_dialog));

		file_name = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(data->save_dialog));

		data->file_name = el::String(file_name);

		g_free(file_name);

		return true;
	}

	gtk_widget_hide(GTK_WIDGET(data->save_dialog));

	return false;
}

G_MODULE_EXPORT void save_file(GtkAction* action, ShaderData* data)
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

G_MODULE_EXPORT void save_as_file(GtkAction* action, ShaderData* data)
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
	GtkAction* action = 0;
	GtkSpinButton* spin_button = 0;
	GtkComboBox* combo_box = 0;
	GtkButton* button = 0;
	GtkTreeSelection* tree_selection = 0;
	GtkFileFilter* filter_xml = 0;
	GtkFileFilter* filter_all = 0;
	GtkListStore* list_store = 0;
	GtkEntry* entry = 0;
	GError *error = 0;
	NoteBookPages pages;
	ParametersData parameters[max_parameter_pages];
	ParameterData parameter;
	ShaderData shader_data;
	NewData new_data;
	PagesData page_datas;
	gint i;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();

	gtk_builder_add_from_file(builder, "main.xml", &error);
	gtk_builder_connect_signals(builder, 0);

	window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
	action = GTK_ACTION(gtk_builder_get_object(builder, "quit"));
	g_signal_connect(action, "activate", G_CALLBACK(close_main_window),
		GTK_WIDGET(window));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

	action = GTK_ACTION(gtk_builder_get_object(builder, "about"));
	dialog = GTK_DIALOG(gtk_builder_get_object(builder, "about_dialog"));
	g_signal_connect(action, "activate", G_CALLBACK(show_about), dialog);

	spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,
		"pages"));

	shader_data.pages = spin_button;
	new_data.pages = spin_button;

	g_signal_connect(spin_button, "value-changed",
		G_CALLBACK(set_pages_visible), &pages);

	combo_box = GTK_COMBO_BOX(gtk_builder_get_object(builder, "type"));

	shader_data.type = combo_box;
	new_data.type = combo_box;

	list_store = GTK_LIST_STORE(gtk_builder_get_object(builder,
		"source_types"));

	el::EnumUtils::append_source_types(list_store);

	gtk_combo_box_set_active(combo_box, 0);

	parameter.name_value = GTK_ENTRY(gtk_builder_get_object(builder,
		"name_value"));
	parameter.name_values = GTK_COMBO_BOX(gtk_builder_get_object(
		builder, "name_values"));
	parameter.type_values = GTK_COMBO_BOX(gtk_builder_get_object(
		builder, "type_values"));
	parameter.qualifier_values = GTK_COMBO_BOX(gtk_builder_get_object(
		builder, "qualifier_values"));
	parameter.size_values = GTK_COMBO_BOX(gtk_builder_get_object(
		builder, "size_values"));
	parameter.scale_values = GTK_SPIN_BUTTON(gtk_builder_get_object(
		builder, "scale_values"));

	list_store = GTK_LIST_STORE(gtk_builder_get_object(builder,
		"names"));

	el::EnumUtils::append_parameter_names(list_store);

	g_signal_connect(parameter.name_values, "changed",
		G_CALLBACK(name_changed), &parameter);

	parameter.sampler_types = GTK_LIST_STORE(gtk_builder_get_object(
		builder, "sampler_types"));

	el::EnumUtils::append_parameter_types(TRUE, parameter.sampler_types);

	parameter.no_sampler_types = GTK_LIST_STORE(gtk_builder_get_object(
		builder, "no_sampler_types"));

	el::EnumUtils::append_parameter_types(FALSE,
		parameter.no_sampler_types);

	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.type_values), 0);

	list_store = GTK_LIST_STORE(gtk_builder_get_object(builder,
		"qualifier_types"));

	el::EnumUtils::append_parameter_qualifier_types(list_store);

	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.qualifier_values), 0);

	list_store = GTK_LIST_STORE(gtk_builder_get_object(builder,
		"size_types"));

	el::EnumUtils::append_parameter_size_types(list_store);

	gtk_combo_box_set_active(GTK_COMBO_BOX(parameter.size_values), 0);

	dialog = GTK_DIALOG(gtk_builder_get_object(builder,
		"parameter_dialog"));

	entry = GTK_ENTRY(gtk_builder_get_object(builder, "name"));

	shader_data.name = entry;
	new_data.name = entry;

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

		page_datas.pages[i].parameters = GTK_TREE_VIEW(
			gtk_builder_get_object(builder, name.c_str()));

		tree_selection = gtk_tree_view_get_selection(
			page_datas.pages[i].parameters);

		name = "parameters_list_";
		name += str.str();

		page_datas.pages[i].parameters_list = GTK_LIST_STORE(
			gtk_builder_get_object(builder, name.c_str()));

		name = "add_";
		name += str.str();

		button = GTK_BUTTON(gtk_builder_get_object(builder,
			name.c_str()));

		parameters[i].parameters = page_datas.pages[i].parameters;
		parameters[i].parameters_list =
			page_datas.pages[i].parameters_list;
		parameters[i].dialog = dialog;
		parameters[i].parameter = &parameter;

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

		page_datas.pages[i].source =
			GTK_TEXT_VIEW(gtk_builder_get_object(builder,
				name.c_str()));

		name = "glsl_120_";
		name += str.str();

		page_datas.pages[i].glsl_120 = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "glsl_130_";
		name += str.str();

		page_datas.pages[i].glsl_130 = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "glsl_140_";
		name += str.str();

		page_datas.pages[i].glsl_140 = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "glsl_150_";
		name += str.str();

		page_datas.pages[i].glsl_150 = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "glsl_330_";
		name += str.str();

		page_datas.pages[i].glsl_330 = GTK_TOGGLE_BUTTON(
			gtk_builder_get_object(builder, name.c_str()));

		name = "text_buffer_";
		name += str.str();

		page_datas.pages[i].text_buffer = GTK_TEXT_BUFFER(
			gtk_builder_get_object(builder,	name.c_str()));
	}

	shader_data.data = &page_datas;
	new_data.data = &page_datas;

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

	action = GTK_ACTION(gtk_builder_get_object(builder, "file_open"));
	g_signal_connect(action, "activate", G_CALLBACK(load_file),
		&shader_data);

	action = GTK_ACTION(gtk_builder_get_object(builder, "file_save"));
	g_signal_connect(action, "activate", G_CALLBACK(save_file),
		&shader_data);

	action = GTK_ACTION(gtk_builder_get_object(builder, "file_save_as"));
	g_signal_connect(action, "activate", G_CALLBACK(save_as_file),
		&shader_data);

	new_data.dialog = GTK_DIALOG(gtk_builder_get_object(builder,
		"new_dialog"));

	entry = GTK_ENTRY(gtk_builder_get_object(builder,
		"new_name"));

	new_data.new_name = entry;

	combo_box = GTK_COMBO_BOX(gtk_builder_get_object(builder, "new_type"));

	new_data.new_type = combo_box;

	gtk_combo_box_set_active(combo_box, 0);

	spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(
		builder, "new_pages"));

	new_data.new_pages = spin_button;

	new_data.add_default_parameter = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "add_default_parameter"));

	action = GTK_ACTION(gtk_builder_get_object(builder, "file_new"));
	g_signal_connect(action, "activate", G_CALLBACK(new_file), &new_data);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();

	return 0;
}
