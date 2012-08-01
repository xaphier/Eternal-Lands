/****************************************************************************
 *            clipmapdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c1d433d5_8dbe_4c13_80c9_d23086d82fe4
#define	UUID_c1d433d5_8dbe_4c13_80c9_d23086d82fe4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class ClipmapData.
 * This file contains the @c class ClipmapData.
 */
namespace eternal_lands
{

	class ClipmapData
	{
		private:
			StringArray16 m_albedo_maps;
			StringArray16 m_normal_maps;
			StringArray16 m_specular_maps;
			ImageSharedPtrArray4 m_blend_images;
			Vec4Array4 m_blend_sizes;
			glm::vec4 m_color;

		public:
			ClipmapData();
			~ClipmapData() noexcept;
			Uint16 get_albedo_count() const;
			Uint16 get_normal_count() const;
			Uint16 get_specular_count() const;
			Uint16 get_blend_count() const;

			inline void set_blend_sizes(
				const Vec4Array4 &blend_sizes) noexcept
			{
				m_blend_sizes = blend_sizes;
			}

			inline void set_albedo_maps(
				const StringArray16 &albedo_maps) noexcept
			{
				m_albedo_maps = albedo_maps;
			}

			inline void set_normal_maps(
				const StringArray16 &normal_maps) noexcept
			{
				m_normal_maps = normal_maps;
			}

			inline void set_specular_maps(
				const StringArray16 &specular_maps) noexcept
			{
				m_specular_maps = specular_maps;
			}

			inline void set_blend_images(
				const ImageSharedPtrArray4 &blend_images)
				noexcept
			{
				m_blend_images = blend_images;
			}

			inline void set_color(const glm::vec4 &color) noexcept
			{
				m_color = color;
			}

			inline const StringArray16 &get_albedo_maps() const
				noexcept
			{
				return m_albedo_maps;
			}

			inline const StringArray16 &get_normal_maps() const
				noexcept
			{
				return m_normal_maps;
			}

			inline const StringArray16 &get_specular_maps() const
				noexcept
			{
				return m_specular_maps;
			}

			inline const ImageSharedPtrArray4 &get_blend_images()
				const noexcept
			{
				return m_blend_images;
			}

			inline const Vec4Array4 &get_blend_sizes() const noexcept
			{
				return m_blend_sizes;
			}

			inline const String &get_albedo_map(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index, m_albedo_maps.size(),
					UTF8("index too big."));

				return m_albedo_maps[index];
			}

			inline const String &get_normal_map(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index, m_normal_maps.size(),
					UTF8("index too big."));

				return m_normal_maps[index];
			}

			inline const String &get_specular_map(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index, m_specular_maps.size(),
					UTF8("index too big."));

				return m_specular_maps[index];
			}

			inline const ImageSharedPtr &get_blend_image(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index, m_blend_images.size(),
					UTF8("index too big."));

				return m_blend_images[index];
			}

			inline const glm::vec4 &get_blend_size(
				const Uint16 index) const
			{
				return m_blend_sizes[index];
			}

			inline const glm::vec4 &get_color() const noexcept
			{
				return m_color;
			}

	};

}

#endif	/* UUID_c1d433d5_8dbe_4c13_80c9_d23086d82fe4 */
