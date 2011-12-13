/****************************************************************************
 *            materialdescription.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_49cf366b_2f4e_481a_ab29_113505e01e90
#define	UUID_49cf366b_2f4e_481a_ab29_113505e01e90

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/shadertextureutil.hpp"

/**
 * @file
 * @brief The @c class MaterialDescription.
 * This file contains the @c class MaterialDescription.
 */
namespace eternal_lands
{

	class MaterialDescription
	{
		private:
			String m_effect;
			StringArray12 m_textures;
			glm::vec4 m_layer_index;
			bool m_shadow;
			bool m_culling;

			bool compare_textures(
				const MaterialDescription &material) const;
			bool compare_non_textures(
				const MaterialDescription &material) const;
			void build_layer_index(
				const TextureCacheSharedPtr &texture_cache,
				const ShaderTextureType texture_type);

		public:
			MaterialDescription();
			MaterialDescription(const String &effect);
			MaterialDescription(const String &diffuse_0,
				const String &effect);
			MaterialDescription(const String &diffuse_0,
				const String &diffuse_1, const String &effect);
			MaterialDescription(const String &diffuse_0,
				const String &normal, const String &specular,
				const String &effect);
			MaterialDescription(const String &diffuse_0,
				const String &diffuse_1, const String &normal,
				const String &specular, const String &effect);
			~MaterialDescription() throw();
			bool operator==(const MaterialDescription &material)
				const;
			bool operator!=(const MaterialDescription &material)
				const;
			bool operator<(const MaterialDescription &material)
				const;

			/**
			 * @brief 
			 * 
			 * Returns true if this material description can be
			 * merged with the given material using texture arrays.
			 * @param diffuses vector of textures the texture array
			 * bound to stt_diffuse_0 contains
			 * @param normals vector of textures the texture array
			 * bound to stt_diffuse_0 contains
			 * @param speculars vector of textures the texture
			 * array bound to stt_specular_0 contains
			 * @param glows vector of textures the texture array
			 * bound to stt_glow_0 contains
			 * @param effect the effect
			 * @param index the index of the textures,
			 * ivec4(stt_diffuse_0, stt_normal_0, stt_specular_0,
			 * stt_glow_0)
			 */
			bool can_merge(const MaterialDescription &material)
				const;
			void build_layer_index(
				const TextureCacheSharedPtr &texture_cache);

			inline void set_effect(const String &effect)
			{
				m_effect = effect;
			}

			inline void set_texture(const String &texture,
				const ShaderTextureType texture_type)
			{
				assert(texture_type < m_textures.size());

				m_textures[texture_type] = texture;
			}

			inline const String &get_texture(
				const ShaderTextureType texture_type) const
			{
				assert(texture_type < m_textures.size());

				return m_textures[texture_type];
			}

			inline const String &get_effect() const
			{
				return m_effect;
			}

			inline void set_layer_index(
				const glm::vec4 &layer_index)
			{
				m_layer_index = layer_index;
			}

			inline void set_shadow(const bool shadow)
			{
				m_shadow = shadow;
			}

			inline void set_culling(const bool culling)
			{
				m_culling = culling;
			}

			inline const glm::vec4 &get_layer_index() const
			{
				return m_layer_index;
			}

			inline bool get_shadow() const
			{
				return m_shadow;
			}

			inline bool get_culling() const
			{
				return m_culling;
			}

	};

}

#endif	/* UUID_49cf366b_2f4e_481a_ab29_113505e01e90 */
