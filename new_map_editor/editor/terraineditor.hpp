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
#include "imagevalues.hpp"
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
		bat_linear = 0,
		bat_quadratic = 1
	};

	enum BrushShapeType
	{
		bst_circle = 0,
		bst_rect = 1
	};

	enum BrushSourceType
	{
		bst_value = 0,
		bst_slope = 1,
		bst_height = 2
	};

	enum DisplacementEffectType
	{
		det_add = 0,
		det_set = 1,
		det_smooth = 2
	};

	enum BlendEffectType
	{
		bet_set = 0,
		bet_slope = 1,
		bet_inverse_slope = 2
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
			boost::array<glm::vec3,  65536> m_normals;
			ImageSharedPtr m_displacement_map;
			ImageSharedPtr m_normal_tangent_map;
			ImageSharedPtr m_dudv_map;
			ImageSharedPtr m_blend_map;
			boost::scoped_ptr<UvTool> m_uv_tool;
			TerrainMaterialData m_material_data;
			StringVector m_albedo_maps;
			StringVector m_specular_maps;
			StringVector m_gloss_maps;
			StringVector m_height_maps;
			glm::vec4 m_dudv_scale_offset;
			glm::vec3 m_translation;
			glm::uvec3 m_size;
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
			glm::vec3 get_smooth_normal(const glm::vec2 &index)
				const;
			void update_normal_tangent_map(
				const glm::ivec2 &index);
			void update_normal_tangent_map(
				const Ivec2Set &positions);
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
				const Uint16 layer,
				ImageValueVector &blend_values) const;
			void get_blend_values(const Uint32 x, const Uint32 y,
				ImageValuesVector &blend_values) const;
			void init(const glm::uvec2 &size);
			BitSet64 get_used_layers() const;
			float get_blend_slope(const glm::ivec2 &index) const;
			glm::vec2 get_uv(const glm::ivec2 &index) const;
			glm::vec2 get_uv_direction(const glm::vec2 &uv,
				const glm::ivec2 &index) const;
			glm::uvec4 get_packed_normal_tangent(
				const glm::ivec2 &index) const;

		public:
			TerrainEditor();
			~TerrainEditor() noexcept;
			void set_displacement_values(
				const DisplacementValueVector
					&displacement_values);
			void set_blend_values(
				const ImageValueVector &blend_values,
				const Uint16 layer);
			void set_blend_values(
				const ImageValuesVector &blend_values);
			void init(const glm::vec3 &translation,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map);
			void init(const ImageSharedPtr &displacement_map,
				const glm::vec3 &translation,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map);
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
				const BrushShapeType shape, const Uint16 layer,
				ImageValueVector &blend_values) const;
			static const glm::vec3 &get_terrain_offset_min();
			static const glm::vec3 &get_terrain_offset_max();
			bool get_vertex(const glm::vec3 &world_position,
				glm::uvec2 &result) const;
			bool get_neares_terrain_data(
				const glm::vec3 &world_position,
				const glm::bvec3 &mask, glm::vec3 &position,
				glm::vec3 &normal) const;
			static glm::vec3 calc_displacement_brush(
				const glm::vec3 &value, const glm::vec3 &data,
				const glm::vec3 &average,
				const glm::bvec3 &mask,
				const glm::vec2 &center,
				const glm::vec2 &position,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const DisplacementEffectType effect);
			static glm::vec3 calc_displacement_effect(
				const glm::vec3 &value,	const glm::vec3 &data,
				const glm::vec3 &average,
				const glm::vec3 &strength,
				const DisplacementEffectType effect);
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
				const DisplacementEffectType effect,
				DisplacementValueVector &displacement_values)
				const;
			void change_blend_values(const glm::vec2 &size,
				const glm::uvec2 &vertex,
				const float attenuation_size,
				const float strength,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BlendEffectType effect,
				const Uint16 layer,
				ImageValueVector &blend_values) const;
			static float calc_blend_effect(const float value,
				const float data, const float slope,
				const float strength,
				const BlendEffectType effect);
			static float calc_blend_brush(const glm::vec2 &center,
				const glm::vec2 &position,
				const glm::vec2 &size, const float value,
				const float attenuation_size,
				const float data, const float slope,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BlendEffectType effect);
			void set_water(const glm::uvec2 vertex,
				const float direction, const float speed,
				const Uint16 index);
			void set_material(const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map, const Uint16 index);
			void get_material(String &albedo_map,
				String &specular_map, String &gloss_map,
				String &height_map,
				glm::vec3 &default_specular,
				float &default_gloss, float &default_height,
				float &blend_size,
				bool &use_blend_size_texture,
				bool &use_specular_map, bool &use_gloss_map,
				bool &use_height_map, const Uint16 index) const;
			void import_height_map(
				const ImageSharedPtr &height_map);
			void get_all_displacement_values(
				DisplacementValueVector
					&displacement_values) const;
			void get_all_blend_values(
				ImageValuesVector &blend_values) const;
			void get_all_blend_values(const Uint16 layer,
				ImageValueVector &blend_values) const;
			void set(const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const ImageSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::vec3 &translation,
				const glm::uvec2 &size);
			void relax_uv(
				const AbstractProgressSharedPtr &progress,
				const Uint16 count, const bool use_simd);
			void import_blend_map(const ImageSharedPtr &blend_map);
			void rebuild_normal_tangent_map();
			void import_dudv_map(const ImageSharedPtr &dudv_map,
				const glm::vec4 &dudv_scale_offset);
			void get_layer_usage(Uint32Vector &use_layer_pixels,
				Uint32 &pixels) const;
			void clear_invisible_layers();
			void pack_layers();
			void clear();
			glm::vec3 get_displacement_value(
				const glm::uvec2 &vertex) const;
			glm::vec3 get_normal_value(const glm::uvec2 &vertex)
				const;
			void fill_blend_layer(const float strength,
				const BlendEffectType effect,
				const Uint16 layer);
			Uint16 get_used_layer_count() const;
			ImageSharedPtr get_partial_blend_map(const Uint16 layer)
				const;

			inline void set_enabled(const bool enabled)
			{
				m_enabled = enabled;
			}

			inline void set_translation(
				const glm::vec3 &translation)
			{
				m_translation = translation;
			}

			inline const glm::vec3 &get_translation() const
			{
				return m_translation;
			}

			inline void set_albedo_map(const String &name,
				const Uint16 index)
			{
				m_albedo_maps[index] = name;
			}

			inline void set_height_map(const String &name,
				const Uint16 index)
			{
				m_height_maps[index] = name;
			}

			inline void set_specular_map(const String &name,
				const Uint16 index)
			{
				m_specular_maps[index] = name;
			}

			inline void set_gloss_map(const String &name,
				const Uint16 index)
			{
				m_gloss_maps[index] = name;
			}

			inline void set_use_blend_size_texture(const bool value,
				const Uint16 index)
			{
				m_material_data.set_use_blend_size_texture(
					value, index);
			}

			inline void set_use_height_map(const bool value,
				const Uint16 index)
			{
				m_material_data.set_use_height_map(value,
					index);
			}

			inline void set_use_specular_map(const bool value,
				const Uint16 index)
			{
				m_material_data.set_use_specular_map(value,
					index);
			}

			inline void set_use_gloss_map(const bool value,
				const Uint16 index)
			{
				m_material_data.set_use_gloss_map(value,
					index);
			}

			inline void set_default_specular(
				const glm::vec3 &default_specular,
				const Uint16 index)
			{
				m_material_data.set_default_specular(
					default_specular, index);
			}

			inline void set_default_gloss(
				const float default_gloss,
				const Uint16 index)
			{
				m_material_data.set_default_gloss(
					default_gloss, index);
			}

			inline void set_default_height(
				const float default_height, const Uint16 index)
			{
				m_material_data.set_default_height(
					default_height, index);
			}

			inline void set_blend_size(const float blend_size,
				const Uint16 index)
			{
				m_material_data.set_blend_size(blend_size,
					index);
			}

			inline void set_material_data(
				const TerrainMaterialData &material_data)
			{
				m_material_data = material_data;
			}

			inline void set_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset)
			{
				m_dudv_scale_offset = dudv_scale_offset;
			}

			static inline Uint16 get_layer_count()
			{
				return 17;
			}

			inline const ImageSharedPtr &get_displacement_map()
				const noexcept
			{
				return m_displacement_map;
			}

			inline const ImageSharedPtr &get_normal_tangent_map()
				const noexcept
			{
				return m_normal_tangent_map;
			}

			inline const ImageSharedPtr &get_dudv_map() const
				noexcept
			{
				return m_dudv_map;
			}

			inline const ImageSharedPtr &get_blend_map() const
				noexcept
			{
				return m_blend_map;
			}

			inline const StringVector &get_albedo_maps() const
				noexcept
			{
				return m_albedo_maps;
			}

			inline const StringVector &get_height_maps() const
				noexcept
			{
				return m_height_maps;
			}

			inline const StringVector &get_specular_maps() const
				noexcept
			{
				return m_specular_maps;
			}

			inline const StringVector &get_gloss_maps() const
				noexcept
			{
				return m_gloss_maps;
			}

			inline const String &get_albedo_map(const Uint16 index)
				const noexcept
			{
				return m_albedo_maps[index];
			}

			inline const String &get_height_map(const Uint16 index)
				const noexcept
			{
				return m_height_maps[index];
			}

			inline const String &get_specular_map(
				const Uint16 index) const noexcept
			{
				return m_specular_maps[index];
			}

			inline const String &get_gloss_map(const Uint16 index)
				const noexcept
			{
				return m_gloss_maps[index];
			}

			inline bool get_use_blend_size_texture(
				const Uint16 index) const
			{
				return m_material_data.
					get_use_blend_size_texture(index);
			}

			inline bool get_use_height_map(const Uint16 index)
				const
			{
				return m_material_data.get_use_height_map(
					index);
			}

			inline bool get_use_specular_map(const Uint16 index)
				const
			{
				return m_material_data.get_use_specular_map(
					index);
			}

			inline bool get_use_gloss_map(const Uint16 index) const
			{
				return m_material_data.get_use_gloss_map(index);
			}

			inline const Vec3Vector &get_default_speculars() const
			{
				return m_material_data.get_default_speculars();
			}

			inline const FloatVector &get_default_glosses() const
			{
				return m_material_data.get_default_glosses();
			}

			inline const FloatVector &get_default_heights() const
			{
				return m_material_data.get_default_heights();
			}

			inline const FloatVector &get_blend_sizes() const
			{
				return m_material_data.get_blend_sizes();
			}

			inline const glm::vec3 &get_default_specular(
				const Uint16 index) const
			{
				return m_material_data.get_default_specular(
					index);
			}

			inline float get_default_gloss(const Uint16 index)
				const
			{
				return m_material_data.get_default_gloss(
					index);
			}

			inline float get_default_height(const Uint16 index)
				const
			{
				return m_material_data.get_default_height(
					index);
			}

			inline float get_blend_size(const Uint16 index) const
			{
				return m_material_data.get_blend_size(index);
			}

			inline const glm::vec4 &get_dudv_scale_offset() const
			{
				return m_dudv_scale_offset;
			}

			inline const glm::uvec3 &get_size() const
			{
				return m_size;
			}

			inline const TerrainMaterialData &get_material_data()
				const noexcept
			{
				return m_material_data;
			}

			inline bool get_enabled() const
			{
				return m_enabled;
			}

	};

}

#endif	/* UUID_977191f8_49e1_489c_9455_45111e370e1c */
