#include "objectsdialog.hpp"
#include <QMessageBox>
#include "xml/xmldoc.hpp"
#include "xml/xmlnode.hpp"

using namespace eternal_lands;

ObjectsDialog::ObjectsDialog(QWidget *parent, const QGLWidget *shareWidget): QDialog(parent)
{
	setupUi(this);

	try
	{
		load_objects();
	}
	catch (const boost::exception &exception)
	{
		QMessageBox::critical(0, "Error", boost::diagnostic_information(exception).c_str());
	}
	catch (const std::exception &exception)
	{
		QMessageBox::critical(0, "Error", exception.what());
	}

	QObject::connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(selected()));

	widget = new ELPreviewWidget(shareWidget, this);
	widget->setObjectName(QString::fromUtf8("widget"));

	gridLayout->addWidget(widget, 0, 3, 1, 3);
}

void ObjectsDialog::selected()
{
	if (tree->selectedItems().size() > 0)
	{
		m_object = (*tree->selectedItems().begin())->data(0, Qt::UserRole).toString().toStdString();
		widget->set_object((*tree->selectedItems().begin())->data(0, Qt::UserRole).toString());
	}
}

namespace eternal_lands
{

	namespace
	{

		void load_sub_category(const XmlNode &node, QTreeWidgetItem* qsub_category)
		{
			XmlNode item;
			QTreeWidgetItem* qobject;
			String str;
			QString qstr;

			for (item = node.get_children(); item.get_has_next(); item = item.get_next())
			{
				if (item.get_type() == XML_ELEMENT_NODE)
				{
					if (item.get_name() == "object")
					{
						str = item.get_property_as_string("name");
						qstr = QString::fromStdString(str);
						qobject = new QTreeWidgetItem((QTreeWidgetItem*)0,
							QStringList(qstr));

						qsub_category->addChild(qobject);

						str = item.get_property_as_string("file");
						qstr = QString::fromStdString(str);

						qobject->setData(0, Qt::UserRole, QVariant(qstr));

		/*
						str = Xml::get_string_property(item, "type");
						qstr = QString::fromStdString(str);

						qobject->setData(1, Qt::UserRole, QVariant(qstr));

						str = Xml::get_string_property(item, "server_id");
						qstr = QString::fromStdString(str);

						qobject->setData(2, Qt::UserRole, QVariant(qstr));
		*/
						qobject->setData(1, Qt::UserRole, QVariant(0));
						qobject->setData(2, Qt::UserRole, QVariant(0));

						qsub_category->addChild(qobject);
					}
				}
				else
				{
					if (item.get_type() == XML_ENTITY_REF_NODE)
					{
						load_sub_category(item.get_children(), qsub_category);
					}
				}
			}
		}

		void load_category(const XmlNode &node, QTreeWidgetItem* qcategory)
		{
			XmlNode item;
			QTreeWidgetItem* qsub_category;
			String str;
			QString qstr;

			for (item = node.get_children(); item.get_has_next(); item = item.get_next())
			{
				if (item.get_type() == XML_ELEMENT_NODE)
				{
					if (item.get_name() == "sub_category")
					{
						str = item.get_property_as_string("name");
						qstr = QString::fromStdString(str);
						qsub_category = new QTreeWidgetItem((QTreeWidgetItem*)0,
							QStringList(qstr));

						qcategory->addChild(qsub_category);

						load_sub_category(item, qsub_category);
					}
				}
				else
				{
					if (item.get_type() == XML_ENTITY_REF_NODE)
					{
						load_category(item.get_children(), qcategory);
					}
				}
			}
		}

		void load(const XmlNode &node, QList<QTreeWidgetItem*> &items)
		{
			XmlNode item;
			QTreeWidgetItem* qcategory;
			String str;
			QString qstr;

			for (item = node.get_children(); item.get_has_next(); item = item.get_next())
			{
				if (item.get_type() == XML_ELEMENT_NODE)
				{
					if (item.get_name() == "category")
					{
						str = item.get_property_as_string("name");
						qstr = QString::fromStdString(str);
						qcategory = new QTreeWidgetItem((QTreeWidgetItem*)0,
							QStringList(qstr));

						items.append(qcategory);

						load_category(item, qcategory);
					}
				}
				else
				{
					if (item.get_type() == XML_ENTITY_REF_NODE)
					{
						load(item.get_children(), items);
					}
				}
			}
		}

	}

}

void ObjectsDialog::load_objects()
{
	QList<QTreeWidgetItem*> items;
	eternal_lands::XmlNode root;
	eternal_lands::XmlDoc doc;

	doc.load("objects.xml");

	root = doc.get_root_node("objects");

	eternal_lands::load(root, items);

	tree->addTopLevelItems(items);
}
