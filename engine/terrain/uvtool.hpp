/****************************************************************************
 *            uvtool.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41
#define	UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "alignedarrays.hpp"

/**
 * @file
 * @brief The @c class HeightMapTool.
 * This file contains the @c class HeightMapTool.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for height map.
	 *
	 * @c class for height map.
	 */
	class UvTool
	{
		private:
			Vec2Vector m_uvs;
			AlignedVec4Array m_half_distances;
			const ImageSharedPtr m_displacement_map;
			const Uint32 m_width, m_height;

			static void relax_edge(
				const AlignedVec4Array &half_distances,
				const Vec2Vector &uvs,
				const glm::uvec2 &position,
				const glm::uvec2 &size, const float damping,
				const float clamping, Vec2Vector &new_uv);
			static void relax(
				const AlignedVec4Array &half_distances,
				const Vec2Vector &uvs, const float damping,
				const float clamping, const Uint32 width,
				const Uint32 index, Vec2Vector &new_uv);
			static void relax_default(
				const AlignedVec4Array &half_distances,
				const Vec2Vector &uvs, const float damping,
				const float clamping, const Uint32 width,
				const Uint32 height, Vec2Vector &new_uvs);
			static void relax_sse2(
				const AlignedVec4Array &half_distances,
				const Vec2Vector &uvs, const float damping,
				const float clamping, const Uint32 width,
				const Uint32 height, Vec2Vector &new_uvs);
			void build_uv();
			void build_half_size();
			void build_half_size(const Sint32 x, const Sint32 y);

			static inline float get_half_distance(
				const AlignedVec4Array &half_distances,
				const Uint32 index, const Uint16 direction)
			{
				return half_distances[index * 2 +
					direction / 4][direction % 4];
			}

		public:
			/**
			 * Default constructor.
			 */
			UvTool(const ImageSharedPtr &displacement_map);

			/**
			 * Default destructor.
			 */
			~UvTool() noexcept;

			void rebuild_half_distances();
			void relax_uv(
				const AbstractProgressSharedPtr &progress,
				const Uint16 count, const bool use_simd);

			/**
			 * Converts to image
			 */
			void convert(ImageSharedPtr &dudv_map,
				glm::vec4 &dudv_scale_offset) const;

			/**
			 * Converts to unsigned rg 16 bit image
			 */
			ImageSharedPtr convert(glm::vec4 &dudv_scale_offset)
				const;

			/**
			 * Imports from unsigned rg 16 bit image
			 */
			void import(const ImageSharedPtr &dudv_map,
				const glm::vec4 &dudv_scale_offset);

			inline const glm::vec2 &get_uv(const Uint16 x,
				const Uint16 y) const
			{
				RANGE_CECK_MAX(x, m_width - 1,
					UTF8("x value too big"));
				RANGE_CECK_MAX(y, m_height - 1,
					UTF8("y value too big"));
				assert((x + y * m_width) < m_uvs.size());

				return m_uvs[x + y * m_width];
			}

	};

}

#endif	/* UUID_c4a6d8e9_ab8a_4e83_8157_46218039aa41 */
