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

	class GlobalVars
	{
		private:
			String m_quality;
			float m_shadow_distance;
			float m_view_distance;
			Uint16 m_shadow_map_count;
			Uint16 m_shadow_map_size;
			OpenglVerionType m_opengl_version;
			bool m_msaa_shadows;
			bool m_exponential_shadow_maps;
			bool m_alpha_to_coverage;
			bool m_filter_shadow_map;
			bool m_fog;
			bool m_optmize_shader_source;

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

			inline void set_shadow_map_count(
				const Uint16 shadow_map_count)
			{
				m_shadow_map_count = shadow_map_count;
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

			inline void set_msaa_shadows(const bool msaa_shadows)
			{
				m_msaa_shadows = msaa_shadows;
			}

			inline void set_exponential_shadow_maps(
				const bool exponential_shadow_maps)
			{
				m_exponential_shadow_maps =
					exponential_shadow_maps;
			}

			inline void set_alpha_to_coverage(
				const bool alpha_to_coverage)
			{
				m_alpha_to_coverage =
					alpha_to_coverage;
			}

			inline void set_filter_shadow_map(
				const bool filter_shadow_map)
			{
				m_filter_shadow_map =
					filter_shadow_map;
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

			inline Uint16 get_shadow_map_count() const
			{
				return m_shadow_map_count;
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
				return m_msaa_shadows;
			}

			inline bool get_exponential_shadow_maps() const
			{
				return m_exponential_shadow_maps;
			}

			inline bool get_alpha_to_coverage() const
			{
				return m_alpha_to_coverage;
			}

			inline bool get_filter_shadow_map() const
			{
				return m_filter_shadow_map;
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

	};

}

#endif	/* UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4 */

