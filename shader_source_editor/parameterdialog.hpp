/****************************************************************************
 *            parameterdialog.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_adf9ce08_bbbc_43ae_b087_3c62d784aa69
#define UUID_adf9ce08_bbbc_43ae_b087_3c62d784aa69

#include "ui_parameter.h"
#include <QDialog>

class ParameterDialog: public QDialog, public Ui::Parameter
{
	Q_OBJECT

	private:
		QString m_name;

		void set_types(const bool sampler);

	private slots:
		void set_custom_name();
		void set_sampler_name();
		void set_auto_name();
		void set_common_name();
		void set_attribute_name();
		void set_auto_name_value(const QString &name);
		void set_common_name_value(const QString &name);
		void set_ok_button();

	public:
		ParameterDialog(QWidget* parent = 0);
		virtual ~ParameterDialog();
		virtual void setVisible(bool visible);
		bool edit_parameter(const QString &name,
			const QString &type, const QString &qualifier,
			const QString &size, const QString &scale);
		bool add_parameter();
		QString get_parameter_name() const;
		QString get_parameter_type() const;
		QString get_parameter_qualifier() const;
		QString get_parameter_size() const;
		QString get_parameter_scale() const;

	public slots:

	signals:
		void check_name(const QString &name, bool &valid);

};

#endif	/* UUID_adf9ce08_bbbc_43ae_b087_3c62d784aa69 */
