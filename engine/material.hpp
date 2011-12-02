/****************************************************************************
 *            material.hpp
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
#include "shader/shadertextureutil.hpp"

/**
 * @file
 * @brief The @c class Material.
 * This file contains the @c class Material.
 */
namespace eternal_lands
{

	class Material
	{
		private:
			EffectCacheWeakPtr m_effect_cache;
			TextureCacheWeakPtr m_texture_cache;
			EffectSharedPtr m_effect;
			TextureSharedPtrArray12 m_textures;
			bool m_shadow;

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

			void set_texture(const MaterialDescription &material,
				const ShaderTextureType texture_type);

		public:
			Material(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);
			Material(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const MaterialDescription &material);
			~Material() throw();
			void set_texture(const String &name,
				const ShaderTextureType texture_type);
			const String &get_texture_name(
				const ShaderTextureType texture_type) const;
			const String &get_effect_name() const;
			void set_effect(const String &effect);
			void bind(StateManager &state_manager) const;
			void set_texture(const TextureSharedPtr &texture,
				const ShaderTextureType texture_type);
			const TextureSharedPtr &get_texture(
				const ShaderTextureType texture_type) const;

			inline void set_effect(const EffectSharedPtr &effect)
			{
				m_effect = effect;
			}

			inline const EffectSharedPtr &get_effect() const
			{
				return m_effect;
			}

			inline void set_shadow(const bool shadow)
			{
				m_shadow = shadow;
			}

			inline bool get_shadow() const
			{
				return m_shadow;
			}

	};

}

#endif	/* UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b */
