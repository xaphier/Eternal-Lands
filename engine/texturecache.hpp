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
 * @brief The @c class TextureCache.
 * This file contains the @c class TextureCache.
 */
namespace eternal_lands
{

	class TextureCache: public boost::noncopyable
	{
		private:
			class TextureArrayLayer;
			typedef std::map<String, TextureSharedPtr>
				TextureCacheMap;
			typedef std::map<String, TextureArrayLayer>
				StringTextureArrayLayerMap;

			TextureCacheMap m_texture_cache;
			StringTextureArrayLayerMap m_texture_arrays;
			const CodecManagerWeakPtr m_codec_manager;
			const TextureArrayCacheWeakPtr m_texture_array_cache;
			const FileSystemWeakPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			TextureSharedPtr m_error_texture;

			TextureSharedPtr load_texture(const String &name,
				const String &index);
			TextureSharedPtr do_load_texture(const String &name,
				const String &index) const;
			TextureSharedPtr load(
				const TextureArray &array) const;
			void add(const TextureArray &array);

			inline CodecManagerSharedPtr get_codec_manager() const
			{
				CodecManagerSharedPtr result;

				result = m_codec_manager.lock();

				assert(result.get() != 0);

				return result;
			}

			inline TextureArrayCacheSharedPtr
				get_texture_array_cache() const
			{
				TextureArrayCacheSharedPtr result;

				result = m_texture_array_cache.lock();

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
				const TextureArrayCacheWeakPtr
					&texture_array_cache,
				const FileSystemWeakPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);
			~TextureCache() throw();
			void add_texture_arrays();
			const TextureSharedPtr &get_texture(const String &name);
			const TextureSharedPtr &get_texture(
				const String &name, float &layer);
			const TextureSharedPtr &get_error_texture();

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */