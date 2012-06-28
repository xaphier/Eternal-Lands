/****************************************************************************
 *            materialdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a0597e5a_2b72_4bfe_81f2_d1dfa008fe4e
#define	UUID_a0597e5a_2b72_4bfe_81f2_d1dfa008fe4e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialData.
 * This file contains the @c class MaterialData.
 */
namespace eternal_lands
{

	class MaterialData
	{
		private:
			Vec4Array4 m_blend_sizes;
			Mat2x4Array4 m_albedo_scale_offsets;
			Mat2x3Array2 m_texture_matrices;
			glm::mat2x3 m_emission_scale_offset;
			glm::vec4 m_specular_scale_offset;
			glm::vec4 m_color;
			glm::vec2 m_dudv_scale;
			bool m_cast_shadows;
			bool m_culling;

		public:
			MaterialData();
			~MaterialData() noexcept;

			inline const Vec4Array4 &get_blend_sizes() const noexcept
			{
				return m_blend_sizes;
			}

			inline const glm::vec4 &get_blend_size(
				const Uint16 index) const
			{
				return m_blend_sizes[index];
			}

			inline const Mat2x4Array4 &get_albedo_scale_offsets()
				const noexcept
			{
				return m_albedo_scale_offsets;
			}

			inline const glm::mat2x4 &get_albedo_scale_offset(
				const Uint16 index) const noexcept
			{
				return m_albedo_scale_offsets[index];
			}

			inline const Mat2x3Array2 &get_texture_matrices()
				const noexcept
			{
				return m_texture_matrices;
			}

			inline const glm::mat2x3 &get_texture_matrix(
				const Uint16 index) const
			{
				return m_texture_matrices[index];
			}

			inline const glm::mat2x3 &get_emission_scale_offset()
				const noexcept
			{
				return m_emission_scale_offset;
			}

			inline const glm::vec4 &get_specular_scale_offset()
				const noexcept
			{
				return m_specular_scale_offset;
			}

			inline const glm::vec4 &get_color() const noexcept
			{
				return m_color;
			}

			inline const glm::vec2 &get_dudv_scale() const noexcept
			{
				return m_dudv_scale;
			}

			inline bool get_cast_shadows() const noexcept
			{
				return m_cast_shadows;
			}

			inline bool get_culling() const noexcept
			{
				return m_culling;
			}

			inline void set_blend_sizes(
				const Vec4Array4 &blend_sizes) noexcept
			{
				m_blend_sizes = blend_sizes;
			}

			inline void set_blend_size(const glm::vec4 &blend_size,
				const Uint16 index)
			{
				m_blend_sizes[index] = blend_size;
			}

			inline void set_albedo_scale_offsets(
				const Mat2x4Array4 &albedo_scale_offsets)
				noexcept
			{
				m_albedo_scale_offsets = albedo_scale_offsets;
			}

			inline void set_albedo_scale_offset(
				const glm::mat2x4 &albedo_scale_offset,
				const Uint16 index)
			{
				m_albedo_scale_offsets[index] =
					albedo_scale_offset;
			}

			inline void set_texture_matrices(
				const Mat2x3Array2 &texture_matrices) noexcept
			{
				m_texture_matrices = texture_matrices;
			}

			inline void set_texture_matrix(
				const glm::mat2x3 &texture_matrix,
				const Uint16 index)
			{
				m_texture_matrices[index] =
					texture_matrix;
			}

			inline void set_emission_scale_offset(
				const glm::mat2x3 &emission_scale_offset)
				noexcept
			{
				m_emission_scale_offset =
					emission_scale_offset;
			}

			inline void set_specular_scale_offset(
				const glm::vec4 &specular_scale_offset) noexcept
			{
				m_specular_scale_offset =
					specular_scale_offset;
			}

			inline void set_color(const glm::vec4 &color) noexcept
			{
				m_color = color;
			}

			inline void set_dudv_scale(const glm::vec2 &dudv_scale)
				noexcept
			{
				m_dudv_scale = dudv_scale;
			}

			inline void set_cast_shadows(const bool cast_shadows)
				noexcept
			{
				m_cast_shadows = cast_shadows;
			}

			inline void set_culling(const bool culling) noexcept
			{
				m_culling = culling;
			}

	};

}

#endif	/* UUID_a0597e5a_2b72_4bfe_81f2_d1dfa008fe4e */
