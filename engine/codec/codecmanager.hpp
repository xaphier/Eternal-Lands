/****************************************************************************
 *            codecmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c9d96c39_752f_4c94_8ed8_eb277a6ebcba
#define	UUID_c9d96c39_752f_4c94_8ed8_eb277a6ebcba

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class CodecManager.
 * This file contains the @c class CodecManager.
 */
namespace eternal_lands
{

	class CodecManager
	{
		public:
			/**
			 * Tries to find a combination of opengl format and
			 * type that matchs the given color bit masks. If a
			 * maching combination one is found, true is returned.
			 * If no matching combination is found, false is
			 * returned.
			 * @param masks The bit masks for red, green, blue and
			 * alpha.
			 * @param rg_formats True if red only and red-green
			 * only formats should be used.
			 * @param integer_formats True if integer formats should
			 * be used.
			 * @see glTexImage1D
			 * @see glTexImage2D
			 * @see glTexImage3D
			 */
			static bool is_fast_load_supported(
				const glm::uvec4 &masks,
				const bool rg_formats,
				const bool integer_formats);

			/**
			 * Tries to find a combination of opengl format and
			 * type that matchs the given color bit masks. If a
			 * maching combination is found, true is returned and
			 * the parameter gl_type, gl_format, size and swap_size
			 * are set with the correct values. If no matching
			 * combination is found, only false is returned.
			 * @param masks The bit masks for red, green, blue and
			 * alpha.
			 * @param rg_formats True if red only and red-green
			 * only formats should be used.
			 * @param integer_formats True if integer formats should
			 * be used.
			 * @param type If fast load is supported, this
			 * parameter is filled with the needed opengl type.
			 * @param format If fast load is supported, this
			 * parameter is filled with the needed opengl format.
			 * @param size The size in bytes per pixel.
			 * @param swap_size The size of the data to swap.
			 * @return True if fast load is supported, else false.
			 * @see glTexImage1D
			 * @see glTexImage2D
			 * @see glTexImage3D
			 */
			static bool is_fast_load_supported(
				const glm::uvec4 &masks, const bool rg_formats,
				const bool integer_formats, GLenum &type,
				GLenum &format, Uint32 &size,
				Uint32 &swap_size);

			/**
			 * Tries to find for a combination of opengl format and
			 * type the given color bit masks. If a maching
			 * combination is found, true is returned and the
			 * parameter red_mask, green_mask,  blue_mask,
			 * alpha_mask, size and swap_size are set with the
			 * correct values. If no matching combination is found,
			 * only false is returned.
			 * @param type The opengl type.
			 * @param format The opengl format.
			 * @param masks The bit masks for red, green, blue and
			 * alpha.
			 * @param size The size in bytes per pixel.
			 * @param swap_size The size of the data to swap.
			 * @param integer_format True if integer format.
			 * @return True if fast load is supported, else false.
			 */
			static bool has_color_bit_mask(const GLenum type,
				const GLenum format, glm::uvec4 &masks,
				Uint32 &size, Uint32 &swap_size,
				bool &integer_format);

			static ImageSharedPtr load_image(
				const ReaderSharedPtr &reader,
				const ImageCompressionTypeSet &compressions,
				const bool rg_formats, const bool sRGB,
				const bool merge_layers);

			static ImageSharedPtr load_image(const String &name,
				const FileSystemConstSharedPtr &file_system,
				const ImageCompressionTypeSet &compressions,
				const bool rg_formats, const bool sRGB,
				const bool merge_layers);

			static void get_image_information(
				const ReaderSharedPtr &reader,
				const bool rg_formats, const bool sRGB,
				TextureFormatType &texture_format,
				glm::uvec3 &size, Uint16 &mipmaps,
				bool &cube_map, bool &array);

			static void get_image_information(const String &name,
				const FileSystemConstSharedPtr &file_system,
				const bool rg_formats, const bool sRGB,
				TextureFormatType &texture_format,
				glm::uvec3 &size, Uint16 &mipmaps,
				bool &cube_map, bool &array);

			static void get_supported_file_extensions(
				StringVector &extensions);

			static void save_image_as_png(
				const ImageConstSharedPtr &image,
				const WriterSharedPtr &writer);
			static void save_image_as_jpeg(
				const ImageConstSharedPtr &image,
				const WriterSharedPtr &writer);
			static void save_image_as_dds(
				const ImageConstSharedPtr &image,
				const WriterSharedPtr &writer);
			static void save_image_as_dds_dxt10(
				const ImageConstSharedPtr &image,
				const WriterSharedPtr &writer);

			static void save_image_as_png(
				const ImageConstSharedPtr &image,
				const String &name);
			static void save_image_as_jpeg(
				const ImageConstSharedPtr &image,
				const String &name);
			static void save_image_as_dds(
				const ImageConstSharedPtr &image,
				const String &name);
			static void save_image_as_dds_dxt10(
				const ImageConstSharedPtr &image,
				const String &name);

	};

}

#endif	/* UUID_c9d96c39_752f_4c94_8ed8_eb277a6ebcba */
