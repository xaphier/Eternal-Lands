/****************************************************************************
 *            effecttextureunit.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a3f7f4ca_8d28_44ed_8ef6_d79da68a3393
#define	UUID_a3f7f4ca_8d28_44ed_8ef6_d79da68a3393

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "texturetargetutil.hpp"

/**
 * @file
 * @brief The @c class EffectTextureUnit.
 * This file contains the @c class EffectTextureUnit.
 */
namespace eternal_lands
{

	class EffectTextureUnit
	{
		private:
			String m_name;
			StringVector m_default_texture;
			TextureTargetType m_target;

			void load_default_texture_xml(const xmlNodePtr node);

		public:
			EffectTextureUnit();
			~EffectTextureUnit() noexcept;
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void load_xml(const xmlNodePtr node);

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_default_texture(
				const String &default_texture) noexcept
			{
				m_default_texture.clear();

				if (!default_texture.get().empty())
				{
					m_default_texture.push_back(
						default_texture);
				}
			}

			inline void set_default_texture(
				const StringVector &default_texture) noexcept
			{
				m_default_texture = default_texture;
			}

			inline void set_target(const TextureTargetType target)
				noexcept
			{
				m_target = target;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const StringVector &get_default_texture() const
				noexcept
			{
				return m_default_texture;
			}

			inline TextureTargetType get_target() const noexcept
			{
				return m_target;
			}

	};

}

#endif	/* UUID_a3f7f4ca_8d28_44ed_8ef6_d79da68a3393 */
