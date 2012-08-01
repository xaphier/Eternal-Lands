/****************************************************************************
 *            actortexturebuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0f7fd32c_e2bd_4a24_851d_183599508843
#define	UUID_0f7fd32c_e2bd_4a24_851d_183599508843

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	enum ActorPartType
	{
		apt_head,
		apt_shield,
		apt_cape,
		apt_helmet,
		apt_weapon,
		apt_torso,
		apt_arms,
		apt_hair,
		apt_boots,
		apt_pants,
		apt_hands,
		apt_neck
	};

	enum ActorPartTextureType
	{
		aptt_pants_tex,
		aptt_pants_mask,
		aptt_boots_tex,
		aptt_boots_mask,
		aptt_torso_tex,
		aptt_arms_tex,
		aptt_torso_mask,
		aptt_arms_mask,
		aptt_hands_tex,
		aptt_head_tex,
		aptt_hands_mask,
		aptt_head_mask,
		aptt_head_base,
		aptt_body_base,
		aptt_arms_base,
		aptt_legs_base,
		aptt_boots_base,
		aptt_hair_tex,
		aptt_weapon_tex,
		aptt_shield_tex,
		aptt_helmet_tex,
		aptt_neck_tex,
		aptt_cape_tex,
		aptt_hands_tex_save
	};

	typedef std::map<ActorPartTextureType, String>
		ActorPartTextureTypeStringMap;

	class ActorTextureBuilder
	{
		private:
			String m_name;
			ActorPartTextureTypeStringMap m_parts;
			ImageSharedPtr m_image;
			TextureSharedPtr m_texture;
			const CodecManagerWeakPtr m_codec_manager;
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			Uint32 m_size;
			Uint16 m_scale;
			BitSet16 m_alphas;
			bool m_compression;

			inline CodecManagerSharedPtr get_codec_manager() const
				noexcept
			{
				CodecManagerSharedPtr result;

				result = m_codec_manager.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline const FileSystemSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

		public:
			ActorTextureBuilder(
				const CodecManagerWeakPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const String &name);
			~ActorTextureBuilder() noexcept;
			void set_parts(
				const ActorPartTextureTypeStringMap &parts);
			void build_actor_images();
			void build_actor_texture();
			String get_effect() const noexcept;

			inline const TextureSharedPtr &get_texture() const
				noexcept
			{
				return m_texture;
			}

			inline bool get_uses_alpha() const noexcept
			{
				return m_alphas.any();
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_0f7fd32c_e2bd_4a24_851d_183599508843 */

