/****************************************************************************
 *            texturecache.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327
#define	UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class TextureManager.
 * This file contains the @c class TextureManager.
 */
namespace eternal_lands
{

	class TextureCache: public boost::noncopyable
	{
		private:
			class TextureArrayItem;
			typedef std::map<String, TextureSharedPtr>
				TextureCacheMap;
			typedef std::map<String, TextureArrayItem>
				TextureArrayMap;

			TextureCacheMap m_texture_cache;
			TextureArrayMap m_texture_arrays;
			const CodecManagerWeakPtr m_codec_manager;
			const FileSystemWeakPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			TextureSharedPtr m_error_texture;

			TextureSharedPtr load_texture(const String &name,
				const String &index);
			TextureSharedPtr do_load_texture(const String &name,
				const String &index) const;
			void load_texture_array_xml(const xmlNodePtr node);
			void load_xml(const xmlNodePtr node);
			TextureSharedPtr load_texture_array(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint16 mipmaps,
				const TextureFormatType format,
				const StringVector &file_names) const;
			void add_texture_array(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint16 mipmaps,
				const TextureFormatType format,
				const StringVector &file_names);

			inline CodecManagerSharedPtr get_codec_manager() const
			{
				CodecManagerSharedPtr result;

				result = m_codec_manager.lock();

				assert(result.get() != 0);

				return result;
			}

			inline FileSystemSharedPtr get_file_system() const
			{
				FileSystemSharedPtr result;

				result = m_file_system.lock();

				assert(result.get() != 0);

				return result;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

		public:
			TextureCache(const CodecManagerWeakPtr &codec_manager,
				const FileSystemWeakPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);
			~TextureCache() throw();
			void load_xml(const String &file_name);
			const TextureSharedPtr &get_texture(const String &name);
			const TextureSharedPtr &get_texture_array(
				const String &name, float &layer);
			bool get_texture_array(const String &name,
				TextureSharedPtr &texture, float &layer) const;
			bool get_texture_array_name(const String &name,
				String &array_name, float &layer) const;
			Uint16 get_texture_array_layer(const String &name)
				const;
			const TextureSharedPtr &get_error_texture();

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */
