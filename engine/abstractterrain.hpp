/****************************************************************************
 *            abstractterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_45c1c534_56a4_4295_a35c_b836fe76721d
#define	UUID_45c1c534_56a4_4295_a35c_b836fe76721d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundingbox.hpp"
#include "qualityutil.hpp"
#include "shader/samplerparameterutil.hpp"

/**
 * @file
 * @brief The @c class AbstractTerrain.
 * This file contains the @c class AbstractTerrain.
 */
namespace eternal_lands
{

	class AbstractTerrain: public boost::noncopyable
	{
		private:
			const GlobalVarsSharedPtr m_global_vars;
			BoundingBox m_bounding_box;
			MaterialSharedPtrArray3 m_materials;
			MaterialSharedPtrArray4 m_clipmap_materials;
			glm::vec4 m_size_data;
			glm::vec3 m_translation;
			const bool m_low_quality_terrain;

			virtual void do_set_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map) = 0;
			virtual void do_update_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map) = 0;
			void set_albedo_maps(const StringVector &albedo_maps,
				const BitSet64 &blend_size_textures,
				const TextureCacheSharedPtr &texture_cache);
			void set_specular_maps(
				const StringVector &specular_maps,
				const BitSet64 &use_specular_maps,
				const TextureCacheSharedPtr &texture_cache);
			void set_gloss_maps(const StringVector &gloss_maps,
				const BitSet64 &use_gloss_maps,
				const TextureCacheSharedPtr &texture_cache);
			void set_height_maps(const StringVector &height_maps,
				const BitSet64 &use_height_maps,
				const TextureCacheSharedPtr &texture_cache);

			static inline Uint16 get_clipmap_material_index(
				const bool write_height,
				const bool write_specular_gloss) noexcept
			{
				BitSet16 result;

				result[0] = write_height;
				result[1] = write_specular_gloss;

				return result.to_ulong();
			}				

		protected:
			AbstractTerrain(const GlobalVarsSharedPtr &global_vars,
				const EffectCacheSharedPtr &effect_cache,
				const MaterialBuilderSharedPtr
					&material_builder,
				const MaterialCacheSharedPtr &material_cache,
				const StringArray3 &material,
				const String &effect);
			void set_clipmap_material_texture(
				const TextureSharedPtr &texture,
				const SamplerParameterType sampler);

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline void set_bounding_box(
				const BoundingBox &bounding_box) noexcept
			{
				m_bounding_box = bounding_box;
			}

			inline void set_size(const glm::vec2 &size) noexcept
			{
				m_size_data = glm::vec4(size, 1.0f / size);
			}

		public:
			virtual ~AbstractTerrain() noexcept;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const = 0;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const = 0;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const = 0;
			virtual void init_rendering_data(
				TerrainRenderingData &rendering_data) const = 0;
			virtual void clear() = 0;
			void set_clipmap_texture(
				const TextureSharedPtr &texture);
			void set_clipmap_specular_gloss_texture(
				const TextureSharedPtr &texture);
			void set_clipmap_normal_texture(
				const TextureSharedPtr &texture);
			void set_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void set_blend_map(const ImageSharedPtr &blend_map,
				const TextureCacheSharedPtr &texture_cache);
			void update_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void update_blend_map(
				const ImageSharedPtr &blend_map,
				const BitSet64 &layers);
			void set_texture_maps(const StringVector &albedo_maps,
				const StringVector &height_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const BitSet64 &blend_size_textures,
				const BitSet64 &use_height_maps,
				const BitSet64 &use_specular_maps,
				const BitSet64 &use_gloss_maps,
				const TextureCacheSharedPtr &texture_cache);
			void set_effect(const EffectSharedPtr &effect,
				const bool write_height,
				const bool write_specular_gloss);
			void set_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset);
			const glm::vec4 &get_dudv_scale_offset() const;
			const MaterialSharedPtr	&get_material() const noexcept;
			static const glm::vec3 &get_vector_min() noexcept;
			static const glm::vec3 &get_vector_max() noexcept;
			static const glm::vec3 &get_vector_scale() noexcept;

			inline bool get_empty() const noexcept
			{
				return get_bounding_box().get_empty();
			}

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

			inline const glm::vec4 &get_size_data() const noexcept
			{
				return m_size_data;
			}

			inline const glm::vec3 &get_translation() const noexcept
			{
				return m_translation;
			}

			inline glm::vec2 get_size() const noexcept
			{
				return glm::vec2(m_size_data);
			}

			inline const MaterialSharedPtr &get_material(
				const QualityType quality) const noexcept
			{
				return m_materials[quality];
			}

			inline const MaterialSharedPtr &get_clipmap_material(
				const bool write_height,
				const bool write_specular_gloss) const noexcept
			{
				return m_clipmap_materials[
					get_clipmap_material_index(
						write_height,
						write_specular_gloss)];
			}

			inline bool get_low_quality_terrain() const noexcept
			{
				return m_low_quality_terrain;
			}

			static inline Uint16 get_tile_size() noexcept
			{
				return 32;
			}

			static inline float get_patch_scale() noexcept
			{
				return 0.25f;
			}

			static inline glm::vec3 get_offset_scaled_0_1(
				const glm::vec4 &value)
			{
				glm::vec3 result;

				result = glm::vec3(value);
				result.x = result.x * 2.0f - 1.0f;
				result.y = result.y * 2.0f - 1.0f;

				return result * get_vector_scale();
			}

			static inline glm::vec3 get_offset_scaled_rgb10_a2(
				const glm::uvec4 &value)
			{
				glm::vec3 result;

				result = glm::vec3(value) / 1023.0f;
				result.x = result.x * 2.0f - 1.0f;
				result.y = result.y * 2.0f - 1.0f;

				return result * get_vector_scale();
			}

			static inline glm::vec3 get_offset_scaled_rgb10_a2(
				const Uint32 value)
			{
				glm::vec3 result;

				result = glm::vec3((value >> 22) & 0x3FF,
					(value >> 12) & 0x3FF,
					(value >> 02) & 0x3FF) / 1023.0f;
				result.x = result.x * 2.0f - 1.0f;
				result.y = result.y * 2.0f - 1.0f;

				return result * get_vector_scale();
			}

			static inline glm::uvec4 get_value_scaled_rgb10_a2(
				const glm::vec3 &offset)
			{
				glm::vec3 result;

				result = offset / get_vector_scale();
				result.x = result.x * 0.5f + 0.5f;
				result.y = result.y * 0.5f + 0.5f;

				return glm::uvec4(result * 1023.0f + 0.5f, 0);
			}

			static inline glm::vec3 get_offset_rgb10_a2(
				const glm::uvec4 &value)
			{
				glm::vec3 result;

				result = glm::vec3(value) / 1023.0f;
				result.x = result.x * 2.0f - 1.0f;
				result.y = result.y * 2.0f - 1.0f;

				return result;
			}

			static inline glm::uvec4 get_value_rgb10_a2(
				const glm::vec3 &offset)
			{
				glm::vec3 result;

				result = offset;
				result.x = result.x * 0.5f + 0.5f;
				result.y = result.y * 0.5f + 0.5f;

				return glm::uvec4(result * 1023.0f + 0.5f, 0);
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
