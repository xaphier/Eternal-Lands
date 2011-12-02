/****************************************************************************
 *            image.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834
#define	UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "memorybuffer.hpp"

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
	class Image
	{
		private:
			MemoryBuffer m_buffer;
			String m_name;
			TextureFormatType m_texture_format;
			Uint32Array3 m_sizes;
			Uint16 m_mipmap_count;
			Uint16 m_pixel_size;
			GLenum m_format;
			GLenum m_type;
			bool m_sRGB;
			bool m_cube_map;

			Uint32 get_total_size() const;
			Uint32 get_buffer_pixel_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 mipmap_level) const;
			Uint32 get_buffer_block_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 mipmap_level) const;
			Uint32 get_offset(const Uint16 face,
				const Uint16 mipmap_level) const;
			static bool is_valid(const GLenum type,
				const GLenum format, const bool compressed);
			static Uint16 get_channel_count(const GLenum format);
			static Uint16 get_pixel_size(const GLenum type,
				const GLenum format);

		public:
			Image(const String &name, const bool cube_map,
				const TextureFormatType texture_format,
				const Uint32Array3 &sizes,
				const Uint16 mipmap_count,
				const Uint16 pixel_size, const GLenum format,
				const GLenum type, const bool sRGB);

			Image(const String &name, const bool cube_map,
				const TextureFormatType texture_format,
				const Uint32Array3 &sizes,
				const Uint16 mipmap_count);

			/**
			 * The size of a compressed block.
			 * @return Size of a compressed block (4x4) in bytes.
			 */
			inline Uint16 get_block_size() const
			{
				return (get_pixel_size() * 16) / 8;
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
				const Uint16 face, const Uint16 mipmap_level)
				const
			{
				assert(x < get_width());
				assert(y < get_height());
				assert(z < get_depth());
				assert(face < get_face_count());
				assert(mipmap_level <= get_mipmap_count());

				return get_offset(face, mipmap_level) +
					get_buffer_pixel_offset(x, y, z,
					mipmap_level);
			}

			inline Uint32 get_block_offset(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap_level)
				const
			{
				assert(x < (get_width() / 4));
				assert(y < (get_height() / 4));
				assert(z < get_depth());
				assert(face < get_face_count());
				assert(mipmap_level <= get_mipmap_count());

				return get_offset(face, mipmap_level) +
					get_buffer_block_offset(x, y, z,
					mipmap_level);
			}

			inline MemoryBuffer &get_buffer()
			{
				return m_buffer;
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for uncompressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap_level The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline void* get_pixel_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap_level)
			{
				return static_cast<Uint8*>(get_buffer(
					).get_ptr()) + get_pixel_offset(x, y, z,
					face, mipmap_level);
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for block compressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap_level The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline void* get_block_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap_level)
			{
				return static_cast<Uint8*>(get_buffer(
					).get_ptr()) + get_block_offset(x, y, z,
					face, mipmap_level);
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
			 * Returns the width of the image. This is always greater than
			 * zero.
			 * @return The width of the image.
			 */
			inline Uint32 get_width() const
			{
				return m_sizes[0];
			}

			/**
			 * @brief The image height.
			 * Returns the height of the image. This is always greater than
			 * zero and one for 1d images.
			 * @return The height of the image.
			 */
			inline Uint32 get_height() const
			{
				return m_sizes[1];
			}

			/**
			 * @brief The image depth.
			 * Returns the depth of the image. This is always greater than
			 * zero and one for 1d and 2d images.
			 * @return The depth of the image.
			 */
			inline Uint32 get_depth() const
			{
				return m_sizes[2];
			}

			/**
			 * @brief The image sizes.
			 * Returns the width, height and depth of the image.
			 * They are always greater than zero.
			 * @return The sizes of the image.
			 */
			inline const Uint32Array3 &get_sizes() const
			{
				return m_sizes;
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
			 * Returns the opengl format of the image data used for loading.
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
			 * Returns the opengl type of the image data used for loading.
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
			 * Returns the pointer to the data of the given mipmap level and
			 * face.
			 * @param face The face to use.
			 * @param mipmap_level The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_data(const Uint16 face,
				const Uint16 mipmap_level) const
			{
				return static_cast<const Uint8* const>(
					get_buffer().get_ptr()) +
					get_offset(face, mipmap_level);
			}

			/**
			 * Returns the pointer to the data of the given pixel
			 * location. Only valid for uncompressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap_level The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_pixel_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap_level)
				const
			{
				return static_cast<const Uint8* const>(
					get_buffer().get_ptr()) +
					get_pixel_offset(x, y, z, face,
					mipmap_level);
			}

			/**
			 * Returns the pointer to the data of the given block
			 * location. Only valid for block compressed formats.
			 * @param x The x position.
			 * @param y The y position.
			 * @param z The z position.
			 * @param face The face to use.
			 * @param mipmap_level The mipmap level to use.
			 * @return The pointer to the data.
			 */
			inline const void* get_block_data(const Uint32 x,
				const Uint32 y, const Uint32 z,
				const Uint16 face, const Uint16 mipmap_level)
				const
			{
				return static_cast<const Uint8* const>(
					get_buffer().get_ptr()) +
					get_block_offset(x, y, z, face,
					mipmap_level);
			}

			inline const MemoryBuffer &get_buffer() const
			{
				return m_buffer;
			}

			/**
			 * @brief The number of channels.
			 * Returns the number of channels, e.g. 3 for rgb, 4 for rgba etc.
			 * @return The number of channels.
			 */
			inline Uint16 get_channel_count() const
			{
				return get_channel_count(get_format());
			}

			/**
			 * @brief The size of the given mipmap level.
			 * Returns the size of one face of the given mipmap level.
			 * @param mipmap_level The mipmap level to use.
			 * @return The size of the given mipmap level.
			 */
			Uint32 get_size(const Uint16 mipmap_level) const;

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

			glm::vec4 get_pixel(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap_level) const;
			void set_pixel(const Uint32 x, const Uint32 y,
				const Uint32 z, const Uint16 face,
				const Uint16 mipmap_level,
				const glm::vec4 &data);

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

			String get_log_str() const;

	};

}

#endif	/* UUID_316cc2ee_47d7_4a49_a789_596cb9fc3834 */

