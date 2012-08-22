/****************************************************************************
 *            cpurasterizer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5120b19a_029a_416c_98c6_1789c80800bd
#define	UUID_5120b19a_029a_416c_98c6_1789c80800bd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "alignedarrays.hpp"

/**
 * @file
 * @brief The @c class CpuRasterizer.
 * This file contains the @c class CpuRasterizer.
 */
namespace eternal_lands
{

	/**
	 * @brief CPU rasterizer
	 *
	 * CPU rasterizer for occlusion and visibilty tests.
	 */
	class CpuRasterizer: public boost::noncopyable
	{
		private:
			/**
			 * Depth buffer for occlusion and visibility tests.
			 */
			AlignedVec4Array m_depth_buffer;
			const Uint32 m_width;
			const Uint32 m_height;
			const float m_threshold;
			const bool m_use_simd;

		protected:
			bool check_visibility(const glm::mat4x4 &matrix,
				const glm::ivec4 &min, const glm::ivec4 &max)
				const;

		public:
			/**
			 * Default constructor.
			 */
			CpuRasterizer(const Uint32 width, const Uint32 height,
				const float threshold, const bool use_simd);

			/**
			 * Default destructor.
			 */
			~CpuRasterizer() noexcept;

			static void build_min_max_boxes(
				const SubMeshVector &sub_meshs,
				AlignedSint16Vec8Array &min_max_boxes);

			static void append_min_max_box(
				const BoundingBox &bounding_box,
				AlignedSint16Vec8Array &min_max_boxes);

			BitSet64 check_visibility(
				const glm::mat4x4 &projection_view_matrix,
				const glm::mat4x3 &world_matrix,
				const AlignedSint16Vec8Array &min_max_boxes)
				const;

			BitSet64 check_visibility(
				const glm::mat4x4 &projection_view_matrix,
				const glm::mat4x3 &world_matrix,
				const AlignedSint16Vec8Array &min_max_boxes,
				const SubMeshVector &sub_meshs) const;

			/**
			 */
			inline Uint32 get_width() const noexcept
			{
				return m_width;
			}

			/**
			 */
			inline Uint32 get_height() const noexcept
			{
				return m_height;
			}

			/**
			 */
			inline float get_threshold() const noexcept
			{
				return m_threshold;
			}

			/**
			 * Scaling value for min max boxes compressed as shorts.
			 */
			static inline float get_min_max_box_scale() noexcept
			{
				return 1.0f / get_min_max_box_inverse_scale();
			}

			/**
			 */
			static inline float get_min_max_box_inverse_scale()
				noexcept
			{
				return 16.0f;
			}

			/**
			 * Using SIMD (SSE/SSE2) instruction for rastering.
			 * @result Using SIMD (SSE/SSE2) instruction.
			 */
			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

	};

}

#endif	/* UUID_5120b19a_029a_416c_98c6_1789c80800bd */
