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
				GtkComboBoxText *combo_box_text);
			static void append_parameter_names(
				GtkComboBoxText *combo_box_text);
			static void append_parameter_types(
				GtkComboBoxText *combo_box_text);
			static void append_parameter_qualifier_types(
				GtkComboBoxText *combo_box_text);
			static void append_parameter_size_types(
				GtkComboBoxText *combo_box_text);
			static bool get_parameter_data(const gchar* str,
				gint &type, gint &qualifier, gint &size,
				gint &scale, gboolean &qualifier_selection);
			static gint get_parameter_type(const gchar* str);
			static gint get_parameter_qualifier_type(
				const gchar* str);
			static gint get_parameter_size_type(
				const gchar* str);

	};

}

#endif	/* UUID_01bf7335_6b07_48d2_a532_c7dfbde4dc6f */
