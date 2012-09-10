/****************************************************************************
 *            materialdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			Mat2x3Array2 m_texture_matrices;
			glm::vec4 m_color;
			glm::vec2 m_dudv_scale;
			bool m_cast_shadows;
			bool m_culling;

		public:
			MaterialData();
			~MaterialData() noexcept;

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

			inline void set_texture_matrices(
				const Mat2x3Array2 &texture_matrices) noexcept
			{
				BOOST_FOREACH(const glm::mat2x3 &value,
					texture_matrices)
				{
					assert(std::isfinite(value[0][0]));
					assert(std::isfinite(value[0][1]));
					assert(std::isfinite(value[0][2]));
					assert(std::isfinite(value[1][0]));
					assert(std::isfinite(value[1][1]));
					assert(std::isfinite(value[1][2]));
				}

				m_texture_matrices = texture_matrices;
			}

			inline void set_texture_matrix(
				const glm::mat2x3 &texture_matrix,
				const Uint16 index)
			{
				assert(std::isfinite(texture_matrix[0][0]));
				assert(std::isfinite(texture_matrix[0][1]));
				assert(std::isfinite(texture_matrix[0][2]));
				assert(std::isfinite(texture_matrix[1][0]));
				assert(std::isfinite(texture_matrix[1][1]));
				assert(std::isfinite(texture_matrix[1][2]));

				m_texture_matrices[index] =
					texture_matrix;
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
