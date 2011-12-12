/****************************************************************************
 *            enumutils.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_01bf7335_6b07_48d2_a532_c7dfbde4dc6f
#define	UUID_01bf7335_6b07_48d2_a532_c7dfbde4dc6f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include <gtk/gtk.h>
#include <string>

/**
 * @file
 * @brief The @c class EnumUtils.
 * This file contains the @c class EnumUtils.
 */
namespace eternal_lands
{

	class EnumUtils
	{
		public:
			static void append_source_types(
				GtkListStore *list_store);
			static void append_parameter_names(
				GtkListStore *list_store);
			static void append_parameter_types(
				const gboolean sampler_types,
				GtkListStore* list_store);
			static void append_parameter_qualifier_types(
				GtkListStore *list_store);
			static void append_parameter_size_types(
				GtkListStore *list_store);
			static bool get_parameter_data(const gchar* str,
				gint &type, gint &qualifier, gint &size,
				gint &scale, gboolean &type_selection,
				gboolean &qualifier_selection,
				gboolean &sampler_types);
			static gint get_parameter_type(const gchar* str,
				gboolean &sampler_type);
			static gint get_parameter_qualifier_type(
				const gchar* str);
			static gint get_parameter_size_type(
				const gchar* str);
			static bool get_default_parameter_data(
				const gint source_type, const guint index,
				std::string &name, std::string &type,
				std::string &qualifier,
				std::string &size, gint &scale);

	};

}

#endif	/* UUID_01bf7335_6b07_48d2_a532_c7dfbde4dc6f */
