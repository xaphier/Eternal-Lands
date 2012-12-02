/****************************************************************************
 *            material.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b
#define	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "materialdata.hpp"
#include "shader/samplerparameterutil.hpp"

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
			const String m_name;
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
				const noexcept
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
				noexcept
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialScriptCacheSharedPtr
				get_material_script_cache() const noexcept
			{
				MaterialScriptCacheSharedPtr result;

				result = m_material_script_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialScriptManagerSharedPtr
				get_material_script_manager() const noexcept
			{
				MaterialScriptManagerSharedPtr result;

				result = m_material_script_manager.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void set_texture(const MaterialDescription &material,
				const SamplerParameterType sampler);

		public:
			Material(const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache,
				const MaterialScriptCacheSharedPtr
					&material_script_cache,
				const MaterialScriptManagerSharedPtr
					&material_script_manager,
				const MaterialDescription &material);
			~Material() noexcept;
			void set_texture(const String &name,
				const SamplerParameterType sampler,
				const bool sRGB);
			const String &get_texture_name(
				const SamplerParameterType sampler) const;
			void set_effect(const String &effect);
			void bind(StateManager &state_manager) const;
			void set_texture(const TextureSharedPtr &texture,
				const SamplerParameterType sampler);
			const TextureSharedPtr &get_texture(
				const SamplerParameterType sampler) const;
			const EffectDescription &get_effect_description() const
				noexcept;
			const String &get_material_script_name() const noexcept;
			void set_material_script(const String &material_script);
			bool execute_script(const glm::vec4 &time,
				asIScriptContext* context);
			void lock() noexcept;
			void unlock() noexcept;

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline void set_effect(const EffectSharedPtr &effect)
				noexcept
			{
				assert(effect.get() != nullptr);
				m_effect = effect;
			}

			inline const EffectSharedPtr &get_effect() const
				noexcept
			{
				return m_effect;
			}

			inline const MaterialScriptSharedPtr
				&get_material_script() const noexcept
			{
				return m_material_script;
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

			inline void set_color(const glm::vec4 &color)
			{
				m_data.set_color(color);
			}

			inline void set_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset)
			{
				m_data.set_dudv_scale_offset(dudv_scale_offset);
			}

			inline void set_cast_shadows(const bool cast_shadows)
			{
				m_data.set_cast_shadows(cast_shadows);
			}

			inline void set_culling(const bool culling)
			{
				m_data.set_culling(culling);
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

			inline const glm::vec4 &get_color() const
			{
				return m_data.get_color();
			}

			inline const glm::vec4 &get_dudv_scale_offset() const
			{
				return m_data.get_dudv_scale_offset();
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
