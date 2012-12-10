#include "objectsdialog.hpp"
#include <QMessageBox>
#include <xmlreader.hpp>
#include <xmlutil.hpp>
#include <QVector3D>

namespace eternal_lands
{

	namespace
	{

		void load_object(const xmlNodePtr node,
			QTreeWidgetItem* sub_category)
		{
			xmlNodePtr it;
			QTreeWidgetItem* object;
			QString name, file, selection;
			glm::vec3 rotation;
			float scale;

			if (sub_category == nullptr)
			{
				return;
			}

			if (xmlStrcmp(node->name, BAD_CAST UTF8("object")) != 0)
			{
				return;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return;
			}

			it = XmlUtil::children(node, true);

			scale = 1.0f;

			do
			{
				if (xmlStrcmp(it->name, BAD_CAST UTF8("name"))
					== 0)
				{
					name = QString::fromUtf8(
						XmlUtil::get_string_value(it
							).get().c_str());
				}
				if (xmlStrcmp(it->name, BAD_CAST UTF8("file"))
					== 0)
				{
					file = QString::fromUtf8(
						XmlUtil::get_string_value(it
							).get().c_str());
				}
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("rotation")) == 0)
				{
					rotation = XmlUtil::get_vec3_value(it);
				}
				if (xmlStrcmp(it->name, BAD_CAST UTF8("scale"))
					== 0)
				{
					scale = XmlUtil::get_float_value(it);
				}
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("selection")) == 0)
				{
					selection = QString::fromUtf8(
						XmlUtil::get_string_value(it
							).get().c_str());
				}
			}
			while (XmlUtil::next(it, true));

			object = new QTreeWidgetItem((QTreeWidgetItem*)0,
				QStringList(name));

			sub_category->addChild(object);

			object->setText(0, name);
			object->setText(1, file);

			object->setData(0, Qt::UserRole, QVariant(file));
			object->setData(1, Qt::UserRole,
				QVariant(QVector3D(rotation.x, rotation.y,
				rotation.z)));
			object->setData(2, Qt::UserRole, QVariant(scale));
			object->setData(3, Qt::UserRole, QVariant(selection));
		}

		void load_sub_category(const xmlNodePtr node,
			QTreeWidgetItem* category)
		{
			xmlNodePtr it;
			QTreeWidgetItem* sub_category;
			QString str;

			if (category == nullptr)
			{
				return;
			}

			if (xmlStrcmp(node->name,
				BAD_CAST UTF8("sub_category")) != 0)
			{
				return;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return;
			}

			sub_category = nullptr;

			it = XmlUtil::children(node, true);

			do
			{
				if (xmlStrcmp(it->name, BAD_CAST UTF8("name"))
					== 0)
				{
					str = QString::fromUtf8(
						XmlUtil::get_string_value(it
							).get().c_str());

					sub_category = new QTreeWidgetItem(
						(QTreeWidgetItem*)0,
						QStringList(str));

					category->addChild(sub_category);
				}
				if (xmlStrcmp(it->name, BAD_CAST UTF8("object"))
					== 0)
				{
					load_object(it, sub_category);
				}
			}
			while (XmlUtil::next(it, true));
		}

		void load_category(const xmlNodePtr node,
			QList<QTreeWidgetItem*> &items)
		{
			xmlNodePtr it;
			QTreeWidgetItem* category;
			QString str;

			if (xmlStrcmp(node->name, BAD_CAST UTF8("category"))
				!= 0)
			{
				return;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return;
			}

			category = nullptr;

			it = XmlUtil::children(node, true);

			do
			{
				if (xmlStrcmp(it->name, BAD_CAST UTF8("name"))
					== 0)
				{
					str = QString::fromUtf8(
						XmlUtil::get_string_value(it
							).get().c_str());
					category = new QTreeWidgetItem(
						(QTreeWidgetItem*)0,
						QStringList(str));

					items.append(category);
				}
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("sub_category")) == 0)
				{
					load_sub_category(it, category);
				}
			}
			while (XmlUtil::next(it, true));
		}

		void load(const xmlNodePtr node,
			QList<QTreeWidgetItem*> &items)
		{
			xmlNodePtr it;

			if (xmlStrcmp(node->name, BAD_CAST UTF8("objects"))
				!= 0)
			{
				return;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return;
			}

			it = XmlUtil::children(node, true);

			do
			{
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("category")) == 0)
				{
					load_category(it, items);
				}
			}
			while (XmlUtil::next(it, true));
		}

	}

}

ObjectsDialog::ObjectsDialog(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	QObject::connect(tree, SIGNAL(itemSelectionChanged()), this,
		SLOT(selected()));
}

ObjectsDialog::~ObjectsDialog()
{
}

void ObjectsDialog::selected()
{
	if (tree->selectedItems().size() == 0)
	{
		ok->setEnabled(false);

		return;
	}

	if (tree->selectedItems()[0]->text(1).isEmpty())
	{
		ok->setEnabled(false);

		return;
	}

	ok->setEnabled(true);

	m_object = (*tree->selectedItems().begin())->data(0,
		Qt::UserRole).toString();
	el_gl_widget->set_object(m_object, tree->selectedItems()[0]->data(1,
		Qt::UserRole).value<QVector3D>());
}

void ObjectsDialog::load_objects()
{
	QList<QTreeWidgetItem*> items;
	XmlReaderSharedPtr xml_reader;

	xml_reader = boost::make_shared<XmlReader>(
		el_gl_widget->get_file_system(), String("objects.xml"));

	load(xml_reader->get_root_node(), items);

	tree->clear();
	tree->addTopLevelItems(items);
}

void ObjectsDialog::set_dirs(const QStringList &dirs)
{
	el_gl_widget->set_dirs(dirs);

	try
	{
		load_objects();
	}
	catch (const boost::exception &exception)
	{
//		LOG_EXCEPTION(exception);
		QMessageBox::critical(0, "boost::exception",
			boost::diagnostic_information(
				exception).c_str());
	}
	catch (const std::exception &exception)
	{
//		LOG_EXCEPTION(exception);
		QMessageBox::critical(0, "std::exception",
			exception.what());
	}
}

void ObjectsDialog::set_object(const QString &object)
{
	QList<QTreeWidgetItem*> items;
	QString str;

	str = object;
	str.replace("\\", "/");

	items = tree->findItems(str, Qt::MatchFixedString | Qt::MatchWrap |
		Qt::MatchRecursive, 1);

	if (items.size() > 0)
	{
		m_object = object;

		tree->setCurrentItem(items[0]);
	}
	else
	{
		m_object = QString();

		tree->setCurrentItem(0);
	}
}
