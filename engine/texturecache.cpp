/****************************************************************************
 *            texturecache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturecache.hpp"
#include "texture.hpp"
#include "image.hpp"
#include "exceptions.hpp"
#include "codec/codecmanager.hpp"
#include "logging.hpp"
#include "filesystem.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "globalvars.hpp"
#include "texturearray.hpp"
#include "texturearraycache.hpp"

namespace eternal_lands
{

	namespace
	{

		/* GIMP RGB C-Source image dump 1-byte-run-length-encoded */
		const Uint32 image_width = 32;
		const Uint32 image_height = 32;
		const Uint8 rle_pixel_data[460] =
		{
			0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 
			0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x84, 0xff, 
			0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x83, 0xff, 0x00, 
			0x00, 0x83, 0x00, 0x00, 0x00, 0x83, 0xff, 0x00, 0x00, 
			0x84, 0x00, 0x00, 0x00, 0x84, 0xff, 0x00, 0x00, 0x83, 
			0x00, 0x00, 0x00, 0x83, 0xff, 0x00, 0x00, 0x83, 0x00, 
			0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x85, 0x00, 0x00, 
			0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 
			0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 
			0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 
			0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 
			0x00, 0x84, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
			0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 
			0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 
			0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x85, 0x00, 0x00, 
			0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 
			0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 
			0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 
			0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 
			0x00, 0x84, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
			0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 
			0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 
			0x00, 0x00, 0x83, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 
			0x00, 0x83, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 
			0x83, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x01, 
			0xff, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x01, 0xff, 
			0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x83, 0xff, 0x00, 
			0x00, 0x83, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
			0x85, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 
			0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
			0x00, 0x83, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
			0x84, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 
			0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x01, 
			0xff, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x01, 0xff, 
			0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 
			0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 
			0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 
			0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x84, 0x00, 
			0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 
			0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 
			0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x84, 
			0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x01, 0xff, 
			0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x83, 0x00, 
			0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x82, 0x00, 0x00, 
			0x00, 0x84, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 
			0x01, 0xff, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x01, 
			0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 
			0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xa3, 0x00, 0x00, 
			0x00
		};

		void rle_decode(const Uint8* input,
			AbstractWriteMemoryBuffer &buffer)
		{
			Uint64 src, dst, len, size;
			Uint8* ptr;

			src = 0;
			dst = 0;

			ptr = static_cast<Uint8*>(buffer.get_ptr());
			size = buffer.get_size();

			while (dst < size)
			{
				len = input[src];
				src++;

				if (len & 128)
				{
					len = len - 128;

					while (len > 0)
					{
						memcpy(&ptr[dst], &input[src],
							3);
						dst += 3;
						len--;
					}

					src += 3;
				}
				else
				{
					len *= 3;

					memcpy(&ptr[dst], &input[src], len);

					dst += len;
					src += len;
				}
			}
		}

		TextureSharedPtr build_error_texture()
		{
			Uint32Array3 sizes;
			ImageSharedPtr image;
			TextureSharedPtr texture;

			sizes[0] = image_width;
			sizes[1] = image_height;
			sizes[2] = 1;

			image = boost::make_shared<Image>(String(UTF8("error")),
				false, tft_rgb8, sizes, 0);

			rle_decode(rle_pixel_data, image->get_buffer());

			texture = boost::make_shared<Texture>(String(UTF8(
				"error")));
			texture->set_format(image->get_texture_format());
			texture->set_image(image);

			return texture;
		}

		StringVector load_file_names_xml(const xmlNodePtr node)
		{
			StringVector file_names;
			xmlNodePtr it;

			if (xmlStrcmp(node->name, BAD_CAST UTF8("file_names")) != 0)
			{
				return file_names;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return file_names;
			}

			it = XmlUtil::children(node, true);

			do
			{
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("file_name")) == 0)
				{
					file_names.push_back(
						XmlUtil::get_string_value(it));
				}
			}
			while (XmlUtil::next(it, true));

			return file_names;
		}

	}

	class TextureCache::TextureArrayLayer
	{
		private:
			TextureSharedPtr m_texture;
			Uint16 m_layer;

		public:
			inline TextureArrayLayer():
				m_layer(std::numeric_limits<Uint16>::max())
			{
			}

			inline TextureArrayLayer(
				const TextureSharedPtr &texture,
				const Uint16 layer): m_texture(texture),
				m_layer(layer)
			{
				assert(texture.get() != 0);
			}

			inline ~TextureArrayLayer() throw()
			{
			}

			inline const TextureSharedPtr &get_texture() const
			{
				return m_texture;
			}

			inline Uint16 get_layer() const
			{
				return m_layer;
			}

	};

	TextureCache::TextureCache(const CodecManagerWeakPtr &codec_manager,
		const TextureArrayCacheWeakPtr &texture_array_cache,
		const FileSystemWeakPtr &file_system,
		const GlobalVarsSharedPtr &global_vars):
		m_codec_manager(codec_manager),
		m_texture_array_cache(texture_array_cache),
		m_file_system(file_system), m_global_vars(global_vars)
	{
		assert(!m_codec_manager.expired());
		assert(!m_texture_array_cache.expired());
		assert(!m_file_system.expired());
		assert(m_global_vars.get() != 0);
	}

	TextureCache::~TextureCache() throw()
	{
	}

	const TextureSharedPtr &TextureCache::get_error_texture()
	{
		if (m_error_texture.get() == 0)
		{
			m_error_texture = build_error_texture();
		}

		return m_error_texture;
	}

	TextureSharedPtr TextureCache::do_load_texture(const String &name,
		const String &index) const
	{
		ImageSharedPtr image;
		TextureSharedPtr texture;
		ReaderSharedPtr reader;
		ImageCompressionTypeSet compressions;

		if (GLEW_EXT_texture_compression_s3tc)
		{
			compressions.insert(ict_s3tc);
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			compressions.insert(ict_rgtc);
		}

		reader = get_file_system()->get_file(name);

		image = get_codec_manager()->load_image(reader, compressions);
		texture = boost::make_shared<Texture>(index);

		if (get_global_vars()->get_opengl_3_0())
		{
			texture->set_target(ttt_2d_texture_array);
		}

		texture->set_format(image->get_texture_format());
		texture->set_image(image);

		return texture;
	}

	TextureSharedPtr TextureCache::load(const TextureArray &texture_array)
		const
	{
		ImageSharedPtrVector images;
		TextureSharedPtr texture;
		ReaderSharedPtr reader;
		ImageCompressionTypeSet compressions;

		if (GLEW_EXT_texture_compression_s3tc)
		{
			compressions.insert(ict_s3tc);
		}

		compressions.insert(ict_rgtc);

		BOOST_FOREACH(const String &file_name,
			texture_array.get_file_names())
		{
			reader = get_file_system()->get_file(file_name);

			images.push_back(get_codec_manager()->load_image(
				reader, compressions));
		}

		texture = boost::make_shared<Texture>(texture_array.get_name());
		texture->set_target(ttt_2d_texture_array);
		texture->set_format(texture_array.get_format());
		texture->set_width(texture_array.get_width());
		texture->set_height(texture_array.get_height());
		texture->set_images(texture_array.get_mipmaps(), images);

		return texture;
	}

	void TextureCache::add(const TextureArray &array)
	{
		TextureSharedPtr texture;
		String index;
		Uint16 layer;

		if (!get_global_vars()->get_opengl_3_0())
		{
			return;
		}

		texture = load(array);

		assert(texture.get() != 0);
		assert(array.get_name() == texture->get_name());

		m_texture_cache[array.get_name()] = texture;

		layer = 0;

		BOOST_FOREACH(const String &file_name, array.get_file_names())
		{
			index = FileSystem::get_file_name_without_extension(
				file_name);

			m_texture_arrays[index] = TextureArrayLayer(texture,
				layer);

			++layer;
		}
	}

	TextureSharedPtr TextureCache::load_texture(const String &name,
		const String &index)
	{
		try
		{
			return do_load_texture(name, index);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		return get_error_texture();
	}

	const TextureSharedPtr &TextureCache::get_texture(const String &name)
	{
		TextureSharedPtr texture;
		TextureCacheMap::iterator found;
		String index;

		index = FileSystem::get_file_name_without_extension(name);

		found = m_texture_cache.find(index);

		if (found == m_texture_cache.end())
		{
			texture = load_texture(name, index);

			m_texture_cache[index] = texture;

			return m_texture_cache[index];
		}
		else
		{
			return found->second;
		}
	}

	const TextureSharedPtr &TextureCache::get_texture(
		const String &name, float &layer)
	{
		StringTextureArrayLayerMap::const_iterator found;
		String index;

		index = FileSystem::get_file_name_without_extension(name);

		found = m_texture_arrays.find(index);

		if (found != m_texture_arrays.end())
		{
			layer = found->second.get_layer();

			return found->second.get_texture();
		}

		return get_texture(name);
	}

	void TextureCache::add_texture_arrays()
	{
		BOOST_FOREACH(const TextureArray &array,
			get_texture_array_cache()->get_texture_arrays())
		{
			add(array);
		}
	}

}