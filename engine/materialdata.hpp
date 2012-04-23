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
			Mat2x4Array4 m_albedo_scale_offsets;
			Mat2x3Array2 m_texture_matrices;
			glm::mat2x3 m_emission_scale_offset;
			glm::vec4 m_specular_scale_offset;
			glm::vec2 m_dudv_scale;
			bool m_cast_shadows;
			bool m_culling;

		public:
			MaterialData();
			~MaterialData() throw();

			inline const Mat2x4Array4 &get_albedo_scale_offsets()
				const
			{
				return m_albedo_scale_offsets;
			}

			inline const glm::mat2x4 &get_albedo_scale_offset(
				const Uint16 index) const
			{
				return m_albedo_scale_offsets[index];
			}

			inline const Mat2x3Array2 &get_texture_matrices()
				const
			{
				return m_texture_matrices;
			}

			inline const glm::mat2x3 &get_texture_matrix(
				const Uint16 index) const
			{
				return m_texture_matrices[index];
			}

			inline const glm::mat2x3 &get_emission_scale_offset()
				const
			{
				return m_emission_scale_offset;
			}

			inline const glm::vec4 &get_specular_scale_offset()
				const
			{
				return m_specular_scale_offset;
			}

			inline const glm::vec2 &get_dudv_scale() const
			{
				return m_dudv_scale;
			}

			inline bool get_cast_shadows() const
			{
				return m_cast_shadows;
			}

			inline bool get_culling() const
			{
				return m_culling;
			}

			inline void set_albedo_scale_offsets(
				const Mat2x4Array4 &albedo_scale_offsets)
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
				const Mat2x3Array2 &texture_matrices)
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
			{
				m_emission_scale_offset =
					emission_scale_offset;
			}

			inline void set_specular_scale_offset(
				const glm::vec4 &specular_scale_offset)
			{
				m_specular_scale_offset =
					specular_scale_offset;
			}

			inline void set_dudv_scale(const glm::vec2 &dudv_scale)
			{
				m_dudv_scale = dudv_scale;
			}

			inline void set_cast_shadows(const bool cast_shadows)
			{
				m_cast_shadows = cast_shadows;
			}

			inline void set_culling(const bool culling)
			{
				m_culling = culling;
			}

	};

}

#endif	/* UUID_a0597e5a_2b72_4bfe_81f2_d1dfa008fe4e */
