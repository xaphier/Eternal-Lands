/****************************************************************************
 *            texturecache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
            const AbstractWriteMemorySharedPtr &buffer)
            noexcept
        {
            Uint64 src, dst, len, size;
            Uint8* ptr;

            src = 0;
            dst = 0;

            ptr = static_cast<Uint8*>(buffer->get_ptr());
            size = buffer->get_size();

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

        TextureSharedPtr build_error_texture(const bool rectangle)
            noexcept
        {
            glm::uvec3 size;
            ImageSharedPtr image;
            TextureSharedPtr texture;

            size[0] = image_width;
            size[1] = image_height;
            size[2] = 0;

            image = boost::make_shared<Image>(String(UTF8("error")),
                false, tft_rgb8, size, 0, false);

            rle_decode(rle_pixel_data, image->get_buffer());

            texture = boost::make_shared<Texture>(String(UTF8(
                "error")), image_width, image_height, 0,
                0xFFFF, 0, image->get_texture_format(),
                rectangle ? ttt_texture_rectangle :
                    ttt_texture_2d);

            texture->set_image(image);

            return texture;
        }

    }

    TextureCache::TextureCache(const GlobalVarsConstSharedPtr &global_vars,
        const FileSystemConstSharedPtr &file_system):
        m_global_vars(global_vars), m_file_system(file_system)
    {
        assert(m_global_vars.get() != nullptr);
        assert(m_file_system.get() != nullptr);
    }

    TextureCache::~TextureCache() noexcept
    {
    }

    const TextureSharedPtr &TextureCache::get_error_texture(
        const bool rectangle)
    {
        if (rectangle)
        {
            if (m_error_texture_rectangle.get() == nullptr)
            {
                m_error_texture_rectangle =
                    build_error_texture(true);
            }

            return m_error_texture_rectangle;
        }

        if (m_error_texture.get() == nullptr)
        {
            m_error_texture = build_error_texture(false);
        }

        return m_error_texture;
    }

    TextureSharedPtr TextureCache::do_load_texture(const String &name,
        const String &index, const bool sRGB, const bool rectangle,
        const bool merge_layers) const
    {
        ImageSharedPtr image;
        ReaderSharedPtr reader;
        ImageCompressionTypeSet compressions;
        bool rg_formats;

        if (GLEW_EXT_texture_compression_s3tc)
        {
            compressions.insert(ict_s3tc);
        }

        rg_formats = false;

        if (get_global_vars()->get_opengl_3_0())
        {
            compressions.insert(ict_rgtc);
            rg_formats = true;
        }

        reader = get_file_system()->get_file(name);

        image = CodecManager::load_image(reader, compressions,
            rg_formats, sRGB, merge_layers);

        return do_load_texture(image, index, rectangle);
    }

    TextureSharedPtr TextureCache::do_load_texture(
        const ImageConstSharedPtr &image, const String &name,
        const bool rectangle) const
    {
        TextureSharedPtr texture;
        TextureTargetType target;
        Uint16 mipmap_count;

        target = image->get_texture_target();
        mipmap_count = 0xFFFF;

        if (rectangle && ((target == ttt_texture_1d) ||
            (target == ttt_texture_2d)))
        {
            target = ttt_texture_rectangle;
            mipmap_count = 0;
        }

        texture = boost::make_shared<Texture>(name, image->get_width(),
            image->get_height(), image->get_depth(), mipmap_count, 0,
            image->get_texture_format(), target);

        texture->set_image(image);

        return texture;
    }

    TextureSharedPtr TextureCache::load_texture(const String &name,
        const String &index, const bool sRGB, const bool rectangle,
        const bool merge_layers)
    {
        try
        {
            return do_load_texture(name, index, sRGB, rectangle,
                merge_layers);
        }
        catch (const boost::exception &exception)
        {
            LOG_EXCEPTION(exception);
        }
        catch (const std::exception &exception)
        {
            LOG_EXCEPTION(exception);
        }

        return get_error_texture(rectangle);
    }

    const TextureSharedPtr &TextureCache::get_texture(const String &name,
        const bool sRGB, const bool rectangle)
    {
        TextureSharedPtr texture;
        TextureCacheMap::iterator found;
        String index;

        index = FileSystem::get_name_without_extension(name);

        found = m_texture_cache.find(index);

        if (found != m_texture_cache.end())
        {
            return found->second;
        }

        texture = load_texture(name, index, sRGB, rectangle, false);

        m_texture_cache[index] = texture;

        return m_texture_cache[index];
    }

    TextureSharedPtr TextureCache::get_texture(
        const ImageConstSharedPtr &image, const bool rectangle) const
    {
        return do_load_texture(image, image->get_name(), rectangle);
    }

    TextureSharedPtr TextureCache::get_texture_array(
        const ImageConstSharedPtrVector &images, const String &name)
        const
    {
        TextureSharedPtr texture;

        texture = boost::make_shared<Texture>(name,
            images[0]->get_width(), images[0]->get_height(),
            images.size(), 0xFFFF, 0,
            images[0]->get_texture_format(), ttt_texture_2d_array);

        texture->set_images(images);

        return texture;
    }

    TextureSharedPtr TextureCache::get_texture_array(
        const StringVector &image_names, const String &name,
        const bool sRGB) const
    {
        ImageSharedPtr image;
        ImageConstSharedPtrVector images;
        ReaderSharedPtr reader;
        ImageCompressionTypeSet compressions;
        bool rg_formats;

        RANGE_CECK_MIN(image_names.size(), 1,
            UTF8("not enough images."));

        if (GLEW_EXT_texture_compression_s3tc)
        {
            compressions.insert(ict_s3tc);
        }

        rg_formats = false;

        if (get_global_vars()->get_opengl_3_0())
        {
            compressions.insert(ict_rgtc);
            rg_formats = true;
        }

        BOOST_FOREACH(const String &image_name, image_names)
        {
            reader = get_file_system()->get_file(image_name);

            image = CodecManager::load_image(reader,
                compressions, rg_formats, sRGB, false);

            images.push_back(image);
        }

        return get_texture_array(images, name);
    }

    TextureSharedPtr TextureCache::get_texture_array(
        const ImageConstSharedPtr &image, const String &name) const
    {
        TextureSharedPtr texture;

        texture = boost::make_shared<Texture>(name,
            image->get_width(), image->get_height(),
            image->get_depth(), 0xFFFF, 0,
            image->get_texture_format(), ttt_texture_2d_array);

        texture->set_image(image);

        return texture;
    }

}
