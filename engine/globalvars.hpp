/****************************************************************************
 *            globalvars.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		ovt_3_3,
		ovt_4_0,
		ovt_4_1,
		ovt_4_2
	};

	enum ShadowQualityType
	{
		sqt_no,
		sqt_low,
		sqt_medium,
		sqt_high,
		sqt_ultra
	};		

	enum LightSystemType
	{
		/**
		 * Default lighting with ligths per object
		 */
		lst_default,
		/**
		 * Light indexed deferred rendering using rgba8 light index
		 * texture with up to 255 lights and up to 4 lights per pixel.
		 */
		lst_lidr_x4,
		/**
		 * Light indexed deferred rendering using rgb10_a2 light index
		 * texture with up to 63 lights and up to 5 lights per pixel.
		 */
		lst_lidr_x5,
		/**
		 * Light indexed deferred rendering using rgba16 light index
		 * texture with up to 255 lights and up to 8 lights per pixel.
		 */
		lst_lidr_x8
	};

	class GlobalVars
	{
		private:
			String m_quality;
			float m_shadow_distance;
			float m_view_distance;
			Uint16 m_shadow_map_size;
			Uint16 m_clipmap_terrain_size;
			Uint16 m_clipmap_terrain_world_size;
			Uint16 m_clipmap_terrain_slices;
			ShadowQualityType m_shadow_quality;
			OpenglVerionType m_opengl_version;
			LightSystemType m_light_system;
			bool m_fog;
			bool m_optmize_shader_source;
			bool m_use_simd;
			bool m_use_cpu_rasterizer;
			bool m_use_s3tc_for_actors;
			bool m_use_block;
			bool m_use_in_out;
			bool m_use_functions;
			bool m_low_quality_terrain;
			bool m_use_multisample_shadows;
			bool m_effect_debug;
			bool m_use_scene_fbo;
			bool m_use_linear_lighting;
			bool m_use_multithreaded_culling;

		public:
			GlobalVars();
			~GlobalVars() noexcept;

			inline void set_quality(const String quality) noexcept
			{
				m_quality = quality;
			}

			inline void set_shadow_distance(
				const float shadow_distance) noexcept
			{
				m_shadow_distance = shadow_distance;
			}

			inline void set_view_distance(
				const float view_distance) noexcept
			{
				m_view_distance = view_distance;
			}

			inline void set_shadow_quality(
				const ShadowQualityType shadow_quality) noexcept
			{
				m_shadow_quality = shadow_quality;
			}

			inline void set_shadow_map_size(
				const Uint16 shadow_map_size) noexcept
			{
				m_shadow_map_size = shadow_map_size;
			}

			inline void set_clipmap_terrain_size(
				const Uint16 clipmap_terrain_size) noexcept
			{
				m_clipmap_terrain_size = clipmap_terrain_size;
			}

			inline void set_clipmap_terrain_world_size(
				const Uint16 clipmap_terrain_world_size)
				noexcept
			{
				m_clipmap_terrain_world_size =
					clipmap_terrain_world_size;
			}

			inline void set_clipmap_terrain_slices(
				const Uint16 clipmap_terrain_slices) noexcept
			{
				m_clipmap_terrain_slices =
					clipmap_terrain_slices;
			}

			inline void set_opengl_version(
				const OpenglVerionType opengl_version) noexcept
			{
				m_opengl_version = opengl_version;
			}

			inline void set_light_system(
				const LightSystemType light_system) noexcept
			{
				m_light_system = light_system;
			}

			inline void set_fog(const bool fog) noexcept
			{
				m_fog = fog;
			}

			inline void set_optmize_shader_source(
				const bool optmize_shader_source) noexcept
			{
				m_optmize_shader_source =
					optmize_shader_source;
			}

			inline void set_use_simd(const bool use_simd) noexcept
			{
				m_use_simd = use_simd;
			}

			inline void set_use_cpu_rasterizer(
				const bool use_cpu_rasterizer) noexcept
			{
				m_use_cpu_rasterizer = use_cpu_rasterizer;
			}

			inline void set_use_s3tc_for_actors(
				const bool use_s3tc_for_actors) noexcept
			{
				m_use_s3tc_for_actors = use_s3tc_for_actors;
			}

			inline void set_use_block(const bool use_block) noexcept
			{
				m_use_block = use_block;
			}

			inline void set_use_in_out(const bool use_in_out)
				noexcept
			{
				m_use_in_out = use_in_out;
			}

			inline void set_use_functions(const bool use_functions)
				noexcept
			{
				m_use_functions = use_functions;
			}

			inline void set_low_quality_terrain(
				const bool low_quality_terrain) noexcept
			{
				m_low_quality_terrain = low_quality_terrain;
			}

			inline void set_use_multisample_shadows(
				const bool use_multisample_shadows) noexcept
			{
				m_use_multisample_shadows =
					use_multisample_shadows;
			}

			inline void set_effect_debug(const bool effect_debug)
				noexcept
			{
				m_effect_debug = effect_debug;
			}

			inline void set_use_scene_fbo(const bool use_scene_fbo)
				noexcept
			{
				m_use_scene_fbo = use_scene_fbo;
			}

			inline void set_use_linear_lighting(
				const bool use_linear_lighting) noexcept
			{
				m_use_linear_lighting = use_linear_lighting;
			}

			inline void set_use_multithreaded_culling(
				const bool use_multithreaded_culling) noexcept
			{
				m_use_multithreaded_culling =
					use_multithreaded_culling;
			}

			inline const String &get_quality() const noexcept
			{
				return m_quality;
			}

			inline float get_shadow_distance() const noexcept
			{
				return m_shadow_distance;
			}

			inline float get_view_distance() const noexcept
			{
				return m_view_distance;
			}

			inline ShadowQualityType get_shadow_quality() const
				noexcept
			{
				return m_shadow_quality;
			}

			inline Uint16 get_shadow_map_count() const noexcept
			{
				switch (get_shadow_quality())
				{
					case sqt_no:
						return 0;
					case sqt_low:
						return 1;
					case sqt_medium:
						return 1;
					case sqt_high:
						return 2;
					case sqt_ultra:
						return 3;
				}

				return 0;
			}

			inline Uint16 get_shadow_map_size() const noexcept
			{
				return m_shadow_map_size;
			}

			inline Uint16 get_clipmap_terrain_size() const noexcept
			{
				return m_clipmap_terrain_size;
			}

			inline Uint16 get_clipmap_terrain_world_size() const
				noexcept
			{
				return m_clipmap_terrain_world_size;
			}

			inline Uint16 get_clipmap_terrain_slices() const
				noexcept
			{
				return m_clipmap_terrain_slices;
			}

			inline LightSystemType get_light_system() const
				noexcept
			{
				return m_light_system;
			}

			inline OpenglVerionType get_opengl_version() const
				noexcept
			{
				return m_opengl_version;
			}

			inline bool get_exponential_shadow_maps() const
				noexcept
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

			inline bool get_fog() const noexcept
			{
				return m_fog;
			}

			inline bool get_optmize_shader_source() const noexcept
			{
				return m_optmize_shader_source;
			}

			inline bool get_opengl_3_0() const noexcept
			{
				return get_opengl_version() >= ovt_3_0;
			}

			inline bool get_opengl_3_1() const noexcept
			{
				return get_opengl_version() >= ovt_3_1;
			}

			inline bool get_opengl_3_2() const noexcept
			{
				return get_opengl_version() >= ovt_3_2;
			}

			inline bool get_opengl_3_3() const noexcept
			{
				return get_opengl_version() >= ovt_3_3;
			}

			inline bool get_opengl_4_0() const noexcept
			{
				return get_opengl_version() >= ovt_4_0;
			}

			inline bool get_opengl_4_1() const noexcept
			{
				return get_opengl_version() >= ovt_4_1;
			}

			inline bool get_opengl_4_2() const noexcept
			{
				return get_opengl_version() >= ovt_4_2;
			}

			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

			inline bool get_use_cpu_rasterizer() const noexcept
			{
				return m_use_cpu_rasterizer;
			}

			inline bool get_use_s3tc_for_actors() const noexcept
			{
				return m_use_s3tc_for_actors;
			}

			inline bool get_use_block() const noexcept
			{
				return m_use_block;
			}

			inline bool get_use_in_out() const noexcept
			{
				return m_use_in_out;
			}

			inline bool get_use_functions() const noexcept
			{
				return m_use_functions;
			}

			inline bool get_low_quality_terrain() const noexcept
			{
				return m_low_quality_terrain;
			}

			inline bool get_use_multisample_shadows() const noexcept
			{
				return m_use_multisample_shadows;
			}

			inline bool get_effect_debug() const noexcept
			{
				return m_effect_debug;
			}

			inline bool get_use_scene_fbo() const noexcept
			{
				return m_use_scene_fbo;
			}

			inline bool get_use_linear_lighting() const noexcept
			{
				return m_use_linear_lighting;
			}

			inline bool get_use_multithreaded_culling() const
				noexcept
			{
				return m_use_multithreaded_culling;
			}

	};

}

#endif	/* UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4 */
