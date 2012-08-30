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
#include "displacmentvalue.hpp"
#include "height.hpp"

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
			ImageSharedPtr m_displacment_image;
			ImageSharedPtr m_normal_image;
			ImageSharedPtr m_dudv_image;
			ImageSharedPtrVector m_blend_images;
			boost::array<glm::vec3,  65536> m_normals;
			Uint16 m_blend_map_count;

			glm::uvec2 get_best_normal(const glm::vec3 &normal)
				const;
			glm::vec3 get_position(const glm::ivec2 &index)
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
			void get_displacment_values(const Uint32 x,
				const Uint32 y,
				DisplacmentValueVector &terrain_values) const;
			void get_blend_values(const Uint32 x, const Uint32 y,
				ImageValueVector &blend_values) const;

		public:
			TerrainEditor();
			~TerrainEditor() noexcept;
			void set_displacment_values(const DisplacmentValueVector
				&displacment_values);
			void set_blend_values(
				const ImageValueVector &blend_values);
			void set_blend_map(const String &name,
				const Uint16 index);
			void set_displacment_map(const String &name);
			void set_dudv_map(const String &name);
			void init(const glm::uvec2 &size);
			void get_displacment_values(const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				DisplacmentValueVector
					&displacment_values) const;
			void get_blend_values(const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				ImageValueVector &blend_values) const;
			static const glm::vec3 &get_terrain_offset();
			static const glm::vec3 &get_terrain_offset_min();
			static const glm::vec3 &get_terrain_offset_max();
			glm::uvec2 get_vertex(const glm::vec3 &world_position)
				const;
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
			void change_displacment_values(const glm::vec3 &data,
				const glm::bvec3 &mask, const glm::vec2 &size,
				const glm::uvec2 &vertex,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect,
				DisplacmentValueVector &displacment_values)
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

			inline void set_blend_map_count(
				const Uint16 blend_map_count)
			{
				m_blend_map_count = blend_map_count;

				m_blend_images.resize((blend_map_count + 3) /
					4);
			}

			inline Uint32 get_blend_maps_count() const
			{
				return m_blend_map_count;
			}

			inline const ImageSharedPtr &get_displacment_image()
				const noexcept
			{
				return m_displacment_image;
			}

			inline const ImageSharedPtr &get_normal_image()
				const noexcept
			{
				return m_normal_image;
			}

			inline const ImageSharedPtr &get_dudv_image()
				const noexcept
			{
				return m_dudv_image;
			}

			inline const ImageSharedPtrVector &get_blend_images()
				const noexcept
			{
				return m_blend_images;
			}

	};

}

#endif	/* UUID_977191f8_49e1_489c_9455_45111e370e1c */
