/****************************************************************************
 *            materialeffect.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b
#define	UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectdescription.hpp"
#include "shader/shadertextureutil.hpp"

/**
 * @file
 * @brief The @c class MaterialEffect.
 * This file contains the @c class MaterialEffect.
 */
namespace eternal_lands
{

	class MaterialEffect
	{
		private:
			EffectCacheWeakPtr m_effect_cache;
			TextureCacheWeakPtr m_texture_cache;
			EffectSharedPtr m_effect;
			MaterialTextureSharedPtrArray m_textures;
			glm::vec4 m_texture_scale_offset;
			bool m_cast_shadows;
			bool m_culling;

			inline EffectCacheSharedPtr get_effect_cache()
				const
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			void set_texture(
				const MaterialEffectDescription &material,
				const ShaderTextureType texture_type);

		public:
			MaterialEffect(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);
			MaterialEffect(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const MaterialEffectDescription &material);
			~MaterialEffect() throw();
			bool operator==(const MaterialEffect &material) const;
			bool operator!=(const MaterialEffect &material) const;
			bool operator<(const MaterialEffect &material) const;
			void set_texture(const String &name,
				const ShaderTextureType texture_type);
			const String &get_texture_name(
				const ShaderTextureType texture_type) const;
			void set_effect(const EffectDescription &effect);
			void bind(StateManager &state_manager) const;
			void set_texture(const TextureSharedPtr &texture,
				const ShaderTextureType texture_type);
			const TextureSharedPtr &get_texture(
				const ShaderTextureType texture_type) const;
			const EffectDescription &get_effect_description() const;

			inline void set_effect(const EffectSharedPtr &effect)
			{
				m_effect = effect;
			}

			inline const EffectSharedPtr &get_effect() const
			{
				return m_effect;
			}

			inline void set_texture_scale_offset(
				const glm::vec4 &texture_scale_offset)
			{
				m_texture_scale_offset =
					texture_scale_offset;
			}

			inline void set_cast_shadows(const bool cast_shadows)
			{
				m_cast_shadows = cast_shadows;
			}

			inline void set_culling(const bool culling)
			{
				m_culling = culling;
			}

			inline const glm::vec4 &get_texture_scale_offset()
				const
			{
				return m_texture_scale_offset;
			}

			inline bool get_cast_shadows() const
			{
				return m_cast_shadows;
			}

			inline bool get_culling() const
			{
				return m_culling;
			}

	};

}

#endif	/* UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b */
