/****************************************************************************
 *            codecmanager.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
		private:
			class GlFastLoadInfo
			{
				private:
					Uint32 m_red_mask;
					Uint32 m_blue_mask;
					Uint32 m_green_mask;
					Uint32 m_alpha_mask;
					Uint32 m_size;
					Uint32 m_swap_size;
					GLenum m_type;
					GLenum m_format;
					Uint16 m_channels;
					bool m_rg_format;

				public:
					inline Uint32 get_red_mask() const
					{
						return m_red_mask;
					}

					inline Uint32 get_blue_mask() const
					{
						return m_blue_mask;
					}

					inline Uint32 get_green_mask() const
					{
						return m_green_mask;
					}

					inline Uint32 get_alpha_mask() const
					{
						return m_alpha_mask;
					}

					inline Uint32 get_size() const
					{
						return m_size;
					}

					inline Uint32 get_swap_size() const
					{
						return m_swap_size;
					}

					inline GLenum get_type() const
					{
						return m_type;
					}

					inline GLenum get_format() const
					{
						return m_format;
					}

					inline Uint16 get_channels() const
					{
						return m_channels;
					}

					inline bool get_rg_format() const
					{
						return m_rg_format;
					}

					inline void set_red_mask(
						const Uint32 red_mask)
					{
						m_red_mask = red_mask;
					}

					inline void set_blue_mask(
						const Uint32 blue_mask)
					{
						m_blue_mask = blue_mask;
					}

					inline void set_green_mask(
						const Uint32 green_mask)
					{
						m_green_mask = green_mask;
					}

					inline void set_alpha_mask(
						const Uint32 alpha_mask)
					{
						m_alpha_mask = alpha_mask;
					}

					inline void set_size(const Uint32 size)
					{
						m_size = size;
					}

					inline void set_swap_size(
						const Uint32 swap_size)
					{
						m_swap_size = swap_size;
					}

					inline void set_type(const GLenum type)
					{
						m_type = type;
					}

					inline void set_format(
						const GLenum format)
					{
						m_format = format;
					}

					inline void set_rg_format(
						const bool rg_format)
					{
						m_rg_format = rg_format;
					}

					inline void set_channels(
						const Uint16 channels)
					{
						m_channels = channels;
					}

			};

			class GlPackedPixelType
			{
				private:
					Uint32 m_red_mask;
					Uint32 m_blue_mask;
					Uint32 m_green_mask;
					Uint32 m_alpha_mask;
					GLenum m_type;

					Uint32 get_size() const;

				public:
					GlPackedPixelType(const Uint32 red_mask,
						const Uint32 green_mask,
						const Uint32 blue_mask,
						const Uint32 alpha_mask,
						const GLenum type);
					bool has_alpha() const;
					GlFastLoadInfo build(
						const GLenum format,
						const bool swap_colors,
						const bool ignore_alpha) const;
			};

			std::vector<GlFastLoadInfo> m_flast_load_infos;

			void add_packed_gl_pixel_type(
				const GlPackedPixelType &pixel_type);
			void add_red_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_red_green_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_luminance_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_alpha_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_luminance_alpha_gl_pixel_type(
				const GLenum type, const Uint32 mask);
			void add_color_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_color_alpha_gl_pixel_type(const GLenum type,
				const Uint32 mask);
			void add_gl_pixel_types();

		public:
			CodecManager();
			~CodecManager() throw();

			/**
			 * Tries to find a combination of opengl format and
			 * type that matchs the given color bit masks. If a
			 * maching combination one is found, true is returned.
			 * If no matching combination is found, false is
			 * returned.
			 * @param red_mask The bit mask for red.
			 * @param green_mask The bit mask for green.
			 * @param blue_mask The bit mask for blue.
			 * @param alpha_mask The bit mask for alpha.
			 * @see glTexImage1D
			 * @see glTexImage2D
			 * @see glTexImage3D
			 */
			bool is_fast_load_supported(const Uint32 red_mask,
				const Uint32 green_mask,
				const Uint32 blue_mask,
				const Uint32 alpha_mask, const bool rga_formats)
				const;

			/**
			 * Tries to find a combination of opengl format and
			 * type that matchs the given color bit masks. If a
			 * maching combination is found, true is returned and
			 * the parameter gl_type, gl_format, size and swap_size
			 * are set with the correct values. If no matching
			 * combination is found, only false is returned.
			 * @param red_mask The bit mask for red.
			 * @param green_mask The bit mask for green.
			 * @param blue_mask The bit mask for blue.
			 * @param alpha_mask The bit mask for alpha.
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
			bool is_fast_load_supported(const Uint32 red_mask,
				const Uint32 green_mask,
				const Uint32 blue_mask,
				const Uint32 alpha_mask, const bool rg_formats,
				GLenum &type, GLenum &format, Uint32 &size,
				Uint32 &swap_size) const;

			/**
			 * Tries to find for a combination of opengl format and
			 * type the given color bit masks. If a maching
			 * combination is found, true is returned and the
			 * parameter red_mask, green_mask,  blue_mask,
			 * alpha_mask, size and swap_size are set with the
			 * correct values. If no matching combination is found,
			 * only false is returned.
			 * @param red_mask The bit mask for red.
			 * @param green_mask The bit mask for green.
			 * @param blue_mask The bit mask for blue.
			 * @param alpha_mask The bit mask for alpha.
			 * @param type opengl type.
			 * @param format opengl format.
			 * @param size The size in bytes per pixel.
			 * @param swap_size The size of the data to swap.
			 * @return True if fast load is supported, else false.
			 */
			bool has_color_bit_mask(const GLenum type,
				const GLenum format, Uint32 &red_mask,
				Uint32 &green_mask, Uint32 &blue_mask,
				Uint32 &alpha_mask, Uint32 &size,
				Uint32 &swap_size) const;

			ImageSharedPtr load_image(
				const ReaderSharedPtr &reader,
				const ImageCompressionTypeSet &compressions,
				const bool rg_formats) const;

			ImageSharedPtr load_image(const String &name,
				const FileSystemSharedPtr &file_system,
				const ImageCompressionTypeSet &compressions,
				const bool rg_formats) const;

			void get_image_information(
				const ReaderSharedPtr &reader,
				const bool rg_formats,
				TextureFormatType &texture_format,
				Uint32Array3 &sizes, Uint16 &mipmaps) const;

			void get_image_information(const String &name,
				const FileSystemSharedPtr &file_system,
				const bool rg_formats,
				TextureFormatType &texture_format,
				Uint32Array3 &sizes, Uint16 &mipmaps) const;

			void get_supported_file_extensions(
				StringVector &extensions) const;

	};

}

#endif	/* UUID_c9d96c39_752f_4c94_8ed8_eb277a6ebcba */
