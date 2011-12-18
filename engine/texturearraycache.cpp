/****************************************************************************
 *            texturearraycache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturearraycache.hpp"
#include "exceptions.hpp"
#include "filesystem.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "globalvars.hpp"
#include "texturearray.hpp"

namespace eternal_lands
{

	class TextureArrayCache::TextureArrayLayer
	{
		private:
			String m_texture;
			Uint16 m_layer;

		public:
			inline TextureArrayLayer():
				m_layer(std::numeric_limits<Uint16>::max())
			{
			}

			inline TextureArrayLayer(const String &texture,
				const Uint16 layer): m_texture(texture),
				m_layer(layer)
			{
				assert(!texture.get().empty());
			}

			inline ~TextureArrayLayer() throw()
			{
			}

			inline const String &get_texture() const
			{
				return m_texture;
			}

			inline Uint16 get_layer() const
			{
				return m_layer;
			}

	};

	TextureArrayCache::TextureArrayCache()
	{
	}

	TextureArrayCache::~TextureArrayCache() throw()
	{
	}

	void TextureArrayCache::load_xml(const xmlNodePtr node)
	{
		TextureArray array;
		String index;
		Uint16 layer;

		array.load_xml(node);

		m_texture_arrays.push_back(array);

		layer = 0;

		BOOST_FOREACH(const String &file_name, array.get_file_names())
		{
			index = FileSystem::get_file_name_without_extension(
				file_name);

			m_layers[index] = TextureArrayLayer(array.get_name(),
				layer);

			++layer;
		}
	}

	void TextureArrayCache::load_xml(
		const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr reader;
		xmlNodePtr it, node;

		if (!file_system->get_file_readable(file_name))
		{
			return;
		}

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		node = reader->get_root_node();

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("texture_arrays")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("texture_array"))
				== 0)
			{
				load_xml(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	bool TextureArrayCache::get_texture_array_name(const String &name,
		String &array_name, float &layer) const
	{
		StringTextureArrayLayerMap::const_iterator found;
		String index;

		index = FileSystem::get_file_name_without_extension(name);

		found = m_layers.find(index);

		if (found != m_layers.end())
		{
			layer = found->second.get_layer();
			array_name = found->second.get_texture();

			return true;
		}

		return false;
	}

	Uint16 TextureArrayCache::get_texture_array_layer(const String &name)
		const
	{
		StringTextureArrayLayerMap::const_iterator found;
		String index;

		index = FileSystem::get_file_name_without_extension(name);

		found = m_layers.find(index);

		if (found != m_layers.end())
		{
			return found->second.get_layer();
		}

		return 0;
	}

}
