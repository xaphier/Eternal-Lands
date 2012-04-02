/****************************************************************************
 *            materialdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_49cf366b_2f4e_481a_ab29_113505e01e90
#define	UUID_49cf366b_2f4e_481a_ab29_113505e01e90

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "materialdata.hpp"
#include "shader/shadertextureutil.hpp"

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

			inline const String &get_texture(const Uint16 index)
				const
			{
				assert(index < m_textures.size());

				return m_textures[index];
			}

		public:
			MaterialDescription();
			MaterialDescription(const xmlNodePtr node);
			~MaterialDescription() throw();
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;
			bool operator<(const MaterialDescription &material)
				const;
			bool operator==(const MaterialDescription &material)
				const;
			bool operator!=(const MaterialDescription &material)
				const;

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

			inline const String &get_name() const
			{
				return m_name;
			}

			inline const String &get_effect() const
			{
				return m_effect;
			}

			inline const String &get_script() const
			{
				return m_script;
			}

			inline void set_name(const String &name)
			{
				m_name = name;
			}

			inline void set_effect(const String &effect)
			{
				m_effect = effect;
			}

			inline void set_script(const String &script)
			{
				m_script = script;
			}

	};

}

#endif	/* UUID_49cf366b_2f4e_481a_ab29_113505e01e90 */
