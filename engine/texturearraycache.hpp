/****************************************************************************
 *            texturearraycache.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b21a6c54_291d_452b_a2cf_6040aadf016c
#define	UUID_b21a6c54_291d_452b_a2cf_6040aadf016c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextureArrays.
 * This file contains the @c class TextureArrays.
 */
namespace eternal_lands
{

	class TextureArrayCache: public boost::noncopyable
	{
		private:
			class TextureArrayLayer;
			typedef std::map<String, TextureArrayLayer>
				StringTextureArrayLayerMap;

			TextureArrayVector m_texture_arrays;
			StringTextureArrayLayerMap m_layers;

			void load_xml(const xmlNodePtr node);

		public:
			TextureArrayCache();
			~TextureArrayCache() throw();
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			bool get_texture_array_name(const String &name,
				String &array_name, float &layer) const;
			Uint16 get_texture_array_layer(const String &name)
				const;

			inline const TextureArrayVector &get_texture_arrays()
				const
			{
				return m_texture_arrays;
			}

	};

}

#endif	/* UUID_b21a6c54_291d_452b_a2cf_6040aadf016c */
