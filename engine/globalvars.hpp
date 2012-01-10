/****************************************************************************
 *            globalvars.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4
#define	UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class GlobalVars.
 * This file contains the @c class GlobalVars.
 */
namespace eternal_lands
{

	enum OpenglVerionType
	{
		ovt_2_1,
		ovt_3_0,
		ovt_3_1,
		ovt_3_2,
		ovt_3_3
	};

	enum ShadowQualityType
	{
		sqt_no,
		sqt_low,
		sqt_medium,
		sqt_high,
		sqt_ultra
	};		

	class GlobalVars
	{
		private:
			String m_quality;
			float m_shadow_distance;
			float m_view_distance;
			Uint16 m_shadow_map_size;
			ShadowQualityType m_shadow_quality;
			OpenglVerionType m_opengl_version;
			bool m_fog;
			bool m_optmize_shader_source;
			bool m_use_simd;

		public:
			GlobalVars();
			~GlobalVars() throw();

			inline void set_quality(const String quality)
			{
				m_quality = quality;
			}

			inline void set_shadow_distance(
				const float shadow_distance)
			{
				m_shadow_distance = shadow_distance;
			}

			inline void set_view_distance(
				const float view_distance)
			{
				m_view_distance = view_distance;
			}

			inline void set_shadow_quality(
				const ShadowQualityType shadow_quality)
			{
				m_shadow_quality = shadow_quality;
			}

			inline void set_shadow_map_size(
				const Uint16 shadow_map_size)
			{
				m_shadow_map_size = shadow_map_size;
			}

			inline void set_opengl_version(
				const OpenglVerionType opengl_version)
			{
				m_opengl_version = opengl_version;
			}

			inline void set_fog(const bool fog)
			{
				m_fog = fog;
			}

			inline void set_optmize_shader_source(
				const bool optmize_shader_source)
			{
				m_optmize_shader_source =
					optmize_shader_source;
			}

			inline void set_use_simd(const bool use_simd)
			{
				m_use_simd = use_simd;
			}

			inline const String &get_quality() const
			{
				return m_quality;
			}

			inline float get_shadow_distance() const
			{
				return m_shadow_distance;
			}

			inline float get_view_distance() const
			{
				return m_view_distance;
			}

			inline ShadowQualityType get_shadow_quality() const
			{
				return m_shadow_quality;
			}

			inline Uint16 get_shadow_map_count() const
			{
				switch (get_shadow_quality())
				{
					case sqt_no:
						return 0;
					case sqt_low:
					case sqt_medium:
						return 1;
					case sqt_high:
						return 2;
					case sqt_ultra:
						return 3;
				}

				return 0;
			}

			inline Uint16 get_shadow_map_size() const
			{
				return m_shadow_map_size;
			}

			inline OpenglVerionType get_opengl_version() const
			{
				return m_opengl_version;
			}

			inline bool get_msaa_shadows() const
			{
				switch (get_shadow_quality())
				{
					case sqt_no:
					case sqt_low:
					case sqt_medium:
					case sqt_high:
						return false;
					case sqt_ultra:
						return true;
				}

				return false;
			}

			inline bool get_exponential_shadow_maps() const
			{
				switch (get_shadow_quality())
				{
					case sqt_no:
					case sqt_low:
						return false;
					case sqt_medium:
					case sqt_high:
					case sqt_ultra:
						return true;
				}

				return false;
			}

			inline bool get_filter_shadow_map() const
			{
				switch (get_shadow_quality())
				{
					case sqt_no:
					case sqt_low:
					case sqt_medium:
						return false;
					case sqt_high:
					case sqt_ultra:
						return true;
				}

				return false;
			}

			inline bool get_fog() const
			{
				return m_fog;
			}

			inline bool get_optmize_shader_source() const
			{
				return m_optmize_shader_source;
			}

			inline bool get_opengl_3_0() const
			{
				return get_opengl_version() >= ovt_3_0;
			}

			inline bool get_opengl_3_1() const
			{
				return get_opengl_version() >= ovt_3_1;
			}

			inline bool get_opengl_3_2() const
			{
				return get_opengl_version() >= ovt_3_2;
			}

			inline bool get_opengl_3_3() const
			{
				return get_opengl_version() >= ovt_3_3;
			}

			inline Uint16 get_glsl_version() const
			{
				switch (get_opengl_version())
				{
					case ovt_2_1:
						return 120;
					case ovt_3_0:
						return 130;
					case ovt_3_1:
						return 140;
					case ovt_3_2:
						return 150;
					case ovt_3_3:
						return 330;
				}

				return 110;
			}

			inline bool get_use_simd() const
			{
				return m_use_simd;
			}

	};

}

#endif	/* UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4 */
