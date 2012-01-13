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
			std::size_t m_hash;
			bool m_shadow;
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

			void set_texture(const MaterialDescription &material,
				const ShaderTextureType texture_type);
			void build_hash();

		public:
			Material(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);
			Material(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const MaterialDescription &material);
			~Material() throw();
			bool operator==(const Material &material) const;
			bool operator!=(const Material &material) const;
			bool operator<(const Material &material) const;
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
				build_hash();
			}

			inline const EffectSharedPtr &get_effect() const
			{
				return m_effect;
			}

			inline void set_shadow(const bool shadow)
			{
				m_shadow = shadow;
				build_hash();
			}

			inline void set_culling(const bool culling)
			{
				m_culling = culling;
				build_hash();
			}

			inline bool get_shadow() const
			{
				return m_shadow;
			}

			inline bool get_culling() const
			{
				return m_culling;
			}

			inline std::size_t get_hash() const
			{
				return m_hash;
			}

	};

}

#endif	/* UUID_6431c502_a2d4_457e_ba5f_8fb8f476628b */
