/****************************************************************************
 *            materialdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_49cf366b_2f4e_481a_ab29_113505e01e90
#define	UUID_49cf366b_2f4e_481a_ab29_113505e01e90

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "materialdata.hpp"
#include "shader/samplerparameterutil.hpp"

/**
 * @file
 * @brief The @c class MaterialDescription.
 * This file contains the @c class MaterialDescription.
 */
namespace eternal_lands
{

	class MaterialDescription: public MaterialData
	{
		private:
			MaterialStringArray m_textures;
			String m_name;
			String m_effect;
			String m_script;

		public:
			MaterialDescription();
			MaterialDescription(const xmlNodePtr node);
			~MaterialDescription() noexcept;
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			bool operator<(const MaterialDescription &material)
				const;
			bool operator==(const MaterialDescription &material)
				const;
			bool operator!=(const MaterialDescription &material)
				const;

			inline void set_texture(const String &texture,
				const Uint16 index)
			{
				assert(index < m_textures.size());

				m_textures[index] = texture;
			}

			inline const String &get_texture(const Uint16 index)
				const
			{
				assert(index < m_textures.size());

				return m_textures[index];
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const String &get_effect() const noexcept
			{
				return m_effect;
			}

			inline const String &get_script() const noexcept
			{
				return m_script;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_effect(const String &effect) noexcept
			{
				m_effect = effect;
			}

			inline void set_script(const String &script) noexcept
			{
				m_script = script;
			}

	};

}

#endif	/* UUID_49cf366b_2f4e_481a_ab29_113505e01e90 */
