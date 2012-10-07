/****************************************************************************
 *            terraineditor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_977191f8_49e1_489c_9455_45111e370e1c
#define	UUID_977191f8_49e1_489c_9455_45111e370e1c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "imagevalue.hpp"
#include "displacementvalue.hpp"
#include "height.hpp"
#include "shader/shadersourceterrain.hpp"

/**
 * @file
 * @brief The @c class TerrainEditor.
 * This file contains the @c class TerrainEditor.
 */
namespace eternal_lands
{

	enum BrushAttenuationType
	{
		bat_const = 0,
		bat_linear = 1,
		bat_quadratic = 2
	};

	enum BrushShapeType
	{
		bst_circle = 0,
		bst_rect = 1
	};

	enum BrushEffectType
	{
		bet_add = 0,
		bet_set = 1,
		bet_smooth = 2
	};

	enum BrushSourceType
	{
		bst_value = 0,
		bst_slope = 1,
		bst_height = 2
	};

	class LessIvec2
	{
		public:
			inline LessIvec2()
			{
			}

			inline ~LessIvec2() noexcept
			{
			}

			inline bool operator()(const glm::ivec2 &p0,
				const glm::ivec2 &p1)
			{
				if (p0.x == p1.x)
				{
					return p0.y < p1.y;
				}

				return p0.x < p1.x;
			}

	};

	typedef std::set<glm::ivec2, LessIvec2> Ivec2Set;

	/**
	 * @brief @c class for editing terrain.
	 *
	 * @c class for editing terrain.
	 */
	class TerrainEditor
	{
		private:
			ImageSharedPtr m_displacement_image;
			ImageSharedPtr m_normal_image;
			ImageSharedPtr m_dudv_image;
			ImageSharedPtr m_blend_image;
			ShaderSourceTerrain m_shader_source_terrain;
			StringVector m_albedo_maps;
			boost::array<glm::vec3,  65536> m_normals;
			Uint16 m_layer_count;
			bool m_enabled;

			glm::uvec2 get_best_normal(const glm::vec3 &normal)
				const;
			glm::vec3 get_position(const glm::ivec2 &index)
				const;
			glm::vec3 get_smooth_position(const glm::vec2 &index)
				const;
			glm::vec3 get_direction(const glm::vec3 &centre,
				const glm::ivec2 &index) const;
			glm::vec3 get_normal(const glm::ivec2 &index) const;
			void update_normal(const glm::ivec2 &index);
			void update_normals(const Ivec2Set &positions);
			void get_blend_values(const glm::uvec2 &vertex,
				const float radius,
				ImageValueVector &blend_values) const;
			static float get_blend_value(const glm::vec4 &blend,
				const Uint32 index);
			static void set_blend_value(const float value,
				const Uint32 index, glm::vec4 &blend);
			static glm::vec4 get_blend_values(
				const glm::vec4 &blend);
			void get_displacement_values(const Uint32 x,
				const Uint32 y,
				DisplacementValueVector &terrain_values) const;
			void get_blend_values(const Uint32 x, const Uint32 y,
				ImageValueVector &blend_values) const;

		public:
			TerrainEditor();
			~TerrainEditor() noexcept;
			void set_displacement_values(
				const DisplacementValueVector
					&displacement_values);
			void set_blend_values(
				const ImageValueVector &blend_values);
			void init(const glm::uvec2 &size,
				const String &texture);
			void get_displacement_values(const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				DisplacementValueVector
					&displacement_values) const;
			void get_blend_values(const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				ImageValueVector &blend_values) const;
			static const glm::vec3 &get_terrain_offset();
			static const glm::vec3 &get_terrain_offset_min();
			static const glm::vec3 &get_terrain_offset_max();
			bool get_vertex(const glm::vec3 &world_position,
				glm::uvec2 &result) const;
			static glm::vec3 calc_brush(const glm::vec3 &value,
				const glm::vec3 &data,
				const glm::vec3 &average,
				const glm::bvec3 &mask,
				const glm::vec2 &center,
				const glm::vec2 &position,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect);
			static glm::vec3 calc_effect(const glm::vec3 &value,
				const glm::vec3 &data, const glm::vec3 &average,
				const glm::vec3 &strength,
				const BrushEffectType effect);
			static float calc_attenuation(const float distance,
				const float attenuation_size,
				const BrushAttenuationType attenuation);
			static float calc_distance(const glm::vec2 &center,
				const glm::vec2 &position,
				const glm::vec2& size,
				const BrushShapeType shape);
			void change_displacement_values(const glm::vec3 &data,
				const glm::bvec3 &mask, const glm::vec2 &size,
				const glm::uvec2 &vertex,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect,
				DisplacementValueVector &displacement_values)
				const;
			void change_blend_values(const glm::vec2 &size,
				const glm::uvec2 &vertex,
				const float attenuation_size, const float data,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect, const int layer,
				ImageValueVector &blend_values) const;
			static float calc_effect(const float value,
				const float data, const float average,
				const float strength,
				const BrushEffectType effect);
			static float calc_brush(const glm::vec2 &center,
				const glm::vec2 &position,
				const glm::vec2 &size, const float value,
				const float attenuation_size, const float data,
				const float average,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect);
			Uint16 get_layer_count() const;
			void set_water(const glm::uvec2 vertex,
				const float direction, const float speed,
				const Uint16 index);

			inline void set_albedo_map(const String &name,
				const Uint16 index)
			{
				m_albedo_maps[index] = name;
			}

			inline void set_blend_data(const ShaderBlendData &data,
				const Uint16 index)
			{
				m_shader_source_terrain.set_data(data, index);
			}

			static inline Uint32 get_max_layer_count()
			{
				return 17;
			}

			inline const ImageSharedPtr &get_displacement_image()
				const noexcept
			{
				return m_displacement_image;
			}

			inline const ImageSharedPtr &get_normal_image() const
				noexcept
			{
				return m_normal_image;
			}

			inline const ImageSharedPtr &get_dudv_image() const
				noexcept
			{
				return m_dudv_image;
			}

			inline const ImageSharedPtr &get_blend_image() const
				noexcept
			{
				return m_blend_image;
			}

			inline const StringVector &get_albedo_maps() const
				noexcept
			{
				return m_albedo_maps;
			}

			inline const String &get_albedo_map(const Uint16 index)
				const noexcept
			{
				return m_albedo_maps[index];
			}

			inline const ShaderBlendData &get_blend_data(
				const Uint16 index) const
			{
				return m_shader_source_terrain.get_data(index);
			}

			inline String get_effect_main() const
			{
				return m_shader_source_terrain.save_xml_string(
					);
			}

			inline bool get_enabled() const
			{
				return m_enabled;
			}

	};

}

#endif	/* UUID_977191f8_49e1_489c_9455_45111e370e1c */
