/****************************************************************************
 *            material.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b
#define	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "materialdata.hpp"
#include "shader/shadertextureutil.hpp"

/**
 * @file
 * @brief The @c class Material.
 * This file contains the @c class Material.
 */
namespace eternal_lands
{

	class Material: public boost::enable_shared_from_this<Material>,
		public boost::noncopyable
	{
		private:
			MaterialData m_data;
			const EffectCacheWeakPtr m_effect_cache;
			const TextureCacheWeakPtr m_texture_cache;
			const MaterialScriptCacheWeakPtr
				m_material_script_cache;
			const MaterialScriptManagerWeakPtr
				m_material_script_manager;
			EffectSharedPtr m_effect;
			MaterialTextureSharedPtrArray m_textures;
			MaterialScriptSharedPtr m_material_script;

			inline EffectCacheSharedPtr get_effect_cache()
				const
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialScriptCacheSharedPtr
				get_material_script_cache() const
			{
				MaterialScriptCacheSharedPtr result;

				result = m_material_script_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialScriptManagerSharedPtr
				get_material_script_manager() const
			{
				MaterialScriptManagerSharedPtr result;

				result = m_material_script_manager.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void set_texture(const MaterialDescription &material,
				const ShaderTextureType texture_type);

		public:
			Material(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const MaterialScriptCacheWeakPtr
					&material_script_cache,
				const MaterialScriptManagerWeakPtr
					&material_script_manager);
			~Material() throw();
			void init(const MaterialDescription &material);
			void set_texture(const String &name,
				const ShaderTextureType texture_type);
			const String &get_texture_name(
				const ShaderTextureType texture_type) const;
			void set_effect(const String &effect);
			void bind(StateManager &state_manager) const;
			void set_texture(const TextureSharedPtr &texture,
				const ShaderTextureType texture_type);
			const TextureSharedPtr &get_texture(
				const ShaderTextureType texture_type) const;
			const EffectDescription &get_effect_description() const;
			const String &get_material_script_name() const;
			void set_material_script(const String &material_script);
			bool execute_script(const glm::vec4 &time,
				asIScriptContext* context);
			void lock();
			void unlock();

			inline void set_effect(const EffectSharedPtr &effect)
			{
				m_effect = effect;
			}

			inline const EffectSharedPtr &get_effect() const
			{
				return m_effect;
			}

			inline const MaterialScriptSharedPtr
				&get_material_script() const
			{
				return m_material_script;
			}

			inline void set_albedo_scale_offsets(
				const Mat2x4Array4 &albedo_scale_offsets)
			{
				m_data.set_albedo_scale_offsets(
					albedo_scale_offsets);
			}

			inline void set_albedo_scale_offset(
				const glm::mat2x4 &albedo_scale_offset,
				const Uint16 index)
			{
				m_data.set_albedo_scale_offset(
					albedo_scale_offset, index);
			}

			inline void set_texture_matrices(
				const Mat2x3Array2 &texture_matrices)
			{
				m_data.set_texture_matrices(texture_matrices);
			}

			inline void set_texture_matrix(
				const glm::mat2x3 &texture_matrix,
				const Uint16 index)
			{
				set_texture_matrix(texture_matrix, index);
			}

			inline void set_emission_scale_offset(
				const glm::mat2x3 &emission_scale_offset)
			{
				m_data.set_emission_scale_offset(
					emission_scale_offset);
			}

			inline void set_specular_scale_offset(
				const glm::vec4 &specular_scale_offset)
			{
				m_data.set_specular_scale_offset(
					specular_scale_offset);
			}

			inline void set_dudv_scale(const glm::vec2 &dudv_scale)
			{
				m_data.set_dudv_scale(dudv_scale);
			}

			inline void set_cast_shadows(const bool cast_shadows)
			{
				m_data.set_cast_shadows(cast_shadows);
			}

			inline void set_culling(const bool culling)
			{
				m_data.set_culling(culling);
			}

			inline const Mat2x4Array4 &get_albedo_scale_offsets()
				const
			{
				return m_data.get_albedo_scale_offsets();
			}

			inline const glm::mat2x4 &get_albedo_scale_offset(
				const Uint16 index) const
			{
				return m_data.get_albedo_scale_offset(index);
			}

			inline const Mat2x3Array2 &get_texture_matrices()
				const
			{
				return m_data.get_texture_matrices();
			}

			inline const glm::mat2x3 &get_texture_matrix(
				const Uint16 index) const
			{
				return m_data.get_texture_matrix(index);
			}

			inline const glm::mat2x3 &get_emission_scale_offset()
				const
			{
				return m_data.get_emission_scale_offset();
			}

			inline const glm::vec4 &get_specular_scale_offset()
				const
			{
				return m_data.get_specular_scale_offset();
			}

			inline const glm::vec2 &get_dudv_scale() const
			{
				return m_data.get_dudv_scale();
			}

			inline bool get_cast_shadows() const
			{
				return m_data.get_cast_shadows();
			}

			inline bool get_culling() const
			{
				return m_data.get_culling();
			}

	};

}

#endif	/* UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b */
