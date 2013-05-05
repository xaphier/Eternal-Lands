/****************************************************************************
 *            image.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834
#define	UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "texturetargetutil.hpp"
#include "readwritememory.hpp"

/**
 * @file
 * @brief The @c class Image.
 * This file contains the @c class Image.
 */
namespace eternal_lands
{

	/**
	 * @brief Abstract basic class for images.
	 *
	 * Abstract basic class for images used for images.
	 * @see ImageLoader
	 */
	class Image: public boost::enable_shared_from_this<Image>,
		public boost::noncopyable
	{
		private:
			ReadWriteMemorySharedPtr m_buffer;
			String m_name;
			TextureFormatType m_texture_format;
			glm::uvec3 m_size;
			Uint16 m_mipmap_count;
			Uint16 m_pixel_size;
			GLenum m_format;
			GLenum m_type;
			bool m_sRGB;
			bool m_cube_map;
			bool m_array;

			Uint32 get_total_size() const;
			Uint32 get_buffer_pixel_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 mipmap) const;
			Uint32 get_buffer_block_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 mipmap) const;
			Uint32 get_offset(const Uint16 face,
				const Uint16 mipmap) const;

		public:
			Image(const String &name, const bool cube_map,
				const TextureFormatType texture_format,
				const glm::uvec3 &size,
				const Uint16 mipmap_count,
				const GLenum format, const GLenum type,
				const bool sRGB, const bool array);

			Image(const String &name, const bool cube_map,
				const TextureFormatType texture_format,
				const glm::uvec3 &size,
				const Uint16 mipmap_count,
				const bool array);

			ReadWriteMemorySharedPtr swizzle(
				const glm::uvec4 &position) const;

			/**
			 * The size of a compressed block.
			 * @return Size of a compressed block (4x4) in bytes.
			 */
			static inline Uint16 get_block_size(
				const Uint16 pixel_size)
			{
				return (pixel_size * 16) / 8;
			}

			/**
			 * The size of a compressed block.
			 * @return Size of a compressed block (4x4) in bytes.
			 */
			inline Uint16 get_block_size() const
			{
				return get_block_size(get_pixel_size());
			}

			/**
			 * @brief Extracts the mipmap level.
			 * Extracts the given mipmap level.
			 * @param image The image to use.
			 * @param face The face to extrace.
			 * @param mipmap The mipmap level to use.
			 * @param expand_dxt1_to_dxt5 Expands the image to dxt5
			 * if the source is dxt1 compressed.
			 */
			Image(const Image &image, const Uint16 face,
				const Uint16 mipmap,
				const bool expand_dxt1_to_dxt5);

			/**
			 * Copy constructor.
			 */
			Image(const Image &image);

			/**
			 * @brief Returns decompressed image.
			 * Returns a decompressed copy of the image if the
			 * image is compressed, else the image itself is
			 * returned.
			 * @param copy Returns a copy of the image if the image
			 * is uncompressed.
			 * @param rg_formats Use RG and R as format for
			 * uncompressed image.
			 * @param merge_layers Merge multiple layers (two or
			 * four) of a red or red-green into in a rgba format.
			 * @return the uncompressed image.
			 */
			ImageSharedPtr decompress(const bool copy,
				const bool rg_formats, const bool merge_layers);

			/**
			 * @brief Returns decompressed image.
			 * Returns a decompressed copy of the image if the
			 * image is compressed, else the image itself is
			 * returned.
			 * @param copy Returns a copy of the image if the image
			 * is uncompressed.
			 * @param rg_formats Use RG and R as format for
			 * uncompressed image.
			 * @param merge_layers Merge multiple layers (two or
			 * four) of a red or red-green into in a rgba format.
			 * @return the uncompressed image.
			 */
			ImageConstSharedPtr decompress(const bool copy,
				const bool rg_formats, const bool merge_layers)
				const;

			/**
			 * @brief Returns decompressed image.
			 * Returns a decompressed copy of the image if the
			 * image is compressed, else a copy if the image
			 * itself is returned.
			 * @param rg_formats Use RG and R as format for
			 * uncompressed image.
			 * @param merge_layers Merge multiple layers (two or
			 * four) of a red or red-green into in a rgba format.
			 * @return the uncompressed image.
			 */
			ImageSharedPtr decompressed_copy(const bool rg_formats,
				const bool merge_layers) const;

			void read_framebuffer(const Uint32 x, const Uint32 y);

			/**
			 * Returns if the image is compressed.
			 * @return True if compressed, false else.
			 */
			inline bool get_compressed() const
			{
				return TextureFormatUtil::get_compressed(
					get_texture_format());
			}

			inline Uint32 get_pixel_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const
			{
				return get_offset(face, mipmap) +
					get_buffer_pixel_offset(x, y, z,
						mipmap);
			}

			inline Uint32 get_block_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const
			{
				return get_offset(face, mipmap) +
					get_buffer_block_offset(x, y, z,
						mipmap);
			}

			inline const ReadWriteMemorySharedPtr &get_buffer()
				const
			{
				return m_buffer;
			}

			/**
			 * Returns the pointer to the data of the given
			 * location.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline void* get_data(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap)
			{
				if (get_compressed())
				{
					return static_cast<Uint8*>(get_buffer(
						)->get_ptr()) +
						get_block_offset(x, y,z, face,
							mipmap);
				}

				return static_cast<Uint8*>(get_buffer(
					)->get_ptr()) + get_pixel_offset(x, y,
						z, face, mipmap);
			}

			/**
			 * Returns the pointer to the data of the given
			 * location.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* const get_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const
			{
				if (get_compressed())
				{
					return static_cast<const Uint8*>(
						get_buffer()->get_ptr()) +
						get_block_offset(x, y,z, face,
							mipmap);
				}

				return static_cast<const Uint8*>(get_buffer(
					)->get_ptr()) + get_pixel_offset(x, y,
						z, face, mipmap);
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for uncompressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline void* get_pixel_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap)
			{
				return static_cast<Uint8*>(get_buffer(
					)->get_ptr()) + get_pixel_offset(x, y,
						z, face, mipmap);
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for block compressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline void* get_block_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap)
			{
				return static_cast<Uint8*>(get_buffer(
					)->get_ptr()) + get_block_offset(x, y,
						z, face, mipmap);
			}

			inline void set_name(const String &name)
			{
				m_name = name;
			}

			/**
			 * @brief Returns true if the image has mipmaps.
			 * Returns true if the image has mipmaps, else false.
			 * @return True if the image has mipmaps, else false.
			 * @see get_mipmap_count
			 */
			inline bool has_mipmaps() const
			{
				return get_mipmap_count() > 0;
			}

			/**
			 * @brief The image width.
			 * Returns the width of the image. This is always
			 * greater than zero.
			 * @return The width of the image.
			 */
			inline Uint32 get_width() const
			{
				return m_size.x;
			}

			/**
			 * @brief The image height.
			 * Returns the height of the image. This is always
			 * greater than zero and one for 1d images.
			 * @return The height of the image.
			 */
			inline Uint32 get_height() const
			{
				return m_size.y;
			}

			/**
			 * @brief The image depth.
			 * Returns the depth of the image. This is always
			 * greater than zero and one for 1d and 2d images.
			 * @return The depth of the image.
			 */
			inline Uint32 get_depth() const
			{
				return m_size.z;
			}

			/**
			 * @brief The image size.
			 * Returns the width, height and depth of the image.
			 * They are always greater than zero.
			 * @return The size of the image.
			 */
			inline const glm::uvec3 &get_size() const
			{
				return m_size;
			}

			/**
			 * @brief The image width.
			 * Returns the width of the image at the given mipmap
			 * level. This is always greater than zero.
			 * @param mipmap The mipmap level to use.
			 * @return The width of the image.
			 */
			inline Uint32 get_width(const Uint16 mipmap) const
			{
				return get_size(mipmap).x;
			}

			/**
			 * @brief The image height.
			 * Returns the height of the image at the given mipmap
			 * level. This is always greater than zero and one for
			 * 1d images.
			 * @param mipmap The mipmap level to use.
			 * @return The height of the image.
			 */
			inline Uint32 get_height(const Uint16 mipmap) const
			{
				return get_size(mipmap).y;
			}

			/**
			 * @brief The image depth.
			 * Returns the depth of the image at the given mipmap
			 * level. This is always greater than zero and one for
			 * 1d and 2d images.
			 * @param mipmap The mipmap level to use.
			 * @return The depth of the image.
			 */
			inline Uint32 get_depth(const Uint16 mipmap) const
			{
				return get_size(mipmap).z;
			}

			/**
			 * @brief The image size.
			 * Returns the width, height and depth of the image at
			 * the given mipmap level. This is always greater than
			 * zero.
			 * @return The size of the image.
			 */
			inline glm::uvec3 get_size(const Uint16 mipmap) const
			{
				if (get_array())
				{
					return glm::max(glm::uvec3(glm::uvec2(
						get_size()) >> glm::uvec2(
							mipmap), get_depth()),
						1);
				}

				return glm::max(get_size() >>
					glm::uvec3(mipmap), 1);
			}

			/**
			 * @brief The mipmap count.
			 * Returns the number of mipmaps in the image.
			 * @return The number of mipmaps in the image.
			 */
			inline Uint16 get_mipmap_count() const
			{
				return m_mipmap_count;
			}

			/**
			 * @brief The opengl format of the image.
			 * Returns the opengl format of the image data used for
			 * loading.
			 * @return The opengl format of the image.
			 * @see glTexImage1D
			 * @see glTexImage2D
			 * @see glTexImage3D
			 */
			inline GLenum get_format() const
			{
				return m_format;
			}

			/**
			 * @brief The opengl type of the image.
			 * Returns the opengl type of the image data used for
			 * loading.
			 * @return The opengl type of the image.
			 * @see glTexImage1D
			 * @see glTexImage2D
			 * @see glTexImage3D
			 */
			inline GLenum get_type() const
			{
				return m_type;
			}

			/**
			 * Returns the pointer to the data of the given mipmap
			 * level and face.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_data(const Uint16 face,
				const Uint16 mipmap) const
			{
				return static_cast<const Uint8* const>(
					get_buffer()->get_ptr()) +
					get_offset(face, mipmap);
			}

			/**
			 * Returns the pointer to the data of the given pixel
			 * location. Only valid for uncompressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_pixel_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const
			{
				return static_cast<const Uint8* const>(
					get_buffer()->get_ptr()) +
					get_pixel_offset(x, y, z, face, mipmap);
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for block compressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_block_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const
			{
				return static_cast<const Uint8* const>(
					get_buffer()->get_ptr()) +
					get_block_offset(x, y, z, face, mipmap);
			}

			/**
			 * @brief The number of channels.
			 * Returns the number of channels, e.g. 3 for rgb,
			 * 4 for rgba etc.
			 * @return The number of channels.
			 */
			Uint16 get_channel_count() const;

			/**
			 * @brief The size of the given mipmap level.
			 * Returns the size of one face of the given mipmap
			 * level.
			 * @param mipmap The mipmap level to use.
			 * @return The size of the given mipmap level.
			 */
			Uint32 get_mipmap_size(const Uint16 mipmap) const;

			/**
			 * @brief The size of the given volume.
			 * Returns the size of one face of the given volume
			 * @param width The width to use.
			 * @param height The width to use.
			 * @param depth The width to use.
			 * @return The size of the given volume.
			 */
			Uint32 get_size(const Uint32 width, const Uint32 height,
				const Uint32 depth) const;

			/**
			 * @brief The size of the given volume.
			 * Returns the size of one face of the given volume
			 * @param width The width to use.
			 * @param height The width to use.
			 * @param depth The width to use.
			 * @return The size of the given volume.
			 */
			static Uint32 get_size(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const TextureFormatType texture_format);

			glm::vec4 get_pixel(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap) const;
			void set_pixel(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap, const glm::vec4 &data);
			glm::uvec4 get_pixel_uint(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const;
			Uint8 get_pixel_packed_uint8(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const;
			Uint16 get_pixel_packed_uint16(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const;
			Uint32 get_pixel_packed_uint32(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap) const;
			void set_pixel_uint(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap, const glm::uvec4 &data);
			void set_pixel_packed_uint8(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap,
				const Uint8 data);
			void set_pixel_packed_uint16(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap,
				const Uint16 data);
			void set_pixel_packed_uint32(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap,
				const Uint32 data);
			glm::ivec4 get_pixel_int(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap) const;
			void set_pixel_int(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap, const glm::ivec4 &data);

			/**
			 * @brief Gets the texture target of the image.
			 *
			 * Returns the texture target of the image.
			 * @return The texture target for the image.
			 */
			TextureTargetType get_texture_target() const;

			/**
			 * @brief Gets the texture format of the image.
			 *
			 * Returns the texture format of the image.
			 * @return The texture format for the image.
			 */
			inline TextureFormatType get_texture_format() const
			{
				return m_texture_format;
			}

			/**
			 * @brief Gets the pixel size.
			 *
			 * Returns the size of a pixel in bits.
			 * @return The size of a pixel.
			 */
			inline Uint16 get_pixel_size() const
			{
				return m_pixel_size;
			}

			/**
			 * @brief The name of the image.
			 *
			 * The name of the image, e.g. the filename.
			 * @return The name of the image.
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

			/**
			 * @brief Gets the number of faces.
			 *
			 * Returns the number of faces. This is 6 for cube maps
			 * and 1 for all other image types.
			 * @return The number of faces.
			 */
			inline Uint16 get_face_count() const
			{
				return get_cube_map() ? 6 : 1;
			}

			/**
			 * @brief Gets the sage of the sRGB color space.
			 *
			 * Returns true if the sRGB color space is used, else
			 * false.
			 * @return The sage of the sRGB color space.
			 */
			inline bool get_sRGB() const
			{
				return m_sRGB;
			}

			/**
			 * Returns true if it's a cube map image, false else.
			 * @return True for cube map, false else.
			 */
			inline bool get_cube_map() const
			{
				return m_cube_map;
			}

			/**
			 * Returns true if it's an array image, false else.
			 * @return True for array image, false else.
			 */
			inline bool get_array() const
			{
				return m_array;
			}

			/**
			 * Returns the number of array layers (zero for non
			 * array textures).
			 * @return The number of array layers.
			 */
			inline Uint32 get_layer_count() const
			{
				if (!get_array())
				{
					return 0;
				}

				if (get_depth() == 0)
				{
					return std::max(static_cast<Uint32>(1),
						get_height());
				}

				return std::max(static_cast<Uint32>(1),
					get_depth());
			}

			String get_log_str() const;

	};

}

#endif	/* UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834 */
