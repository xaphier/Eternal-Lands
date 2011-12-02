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
			typedef std::map<String, TextureSharedPtr>
				TextureCacheMap;

			TextureCacheMap m_texture_cache;
			CodecManagerWeakPtr m_codec_manager;
			FileSystemWeakPtr m_file_system;
			TextureSharedPtr m_error_texture;

			TextureSharedPtr load_texture(const String &name);
			TextureSharedPtr do_load_texture(const String &name)
				const;

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

		public:
			TextureCache(const CodecManagerWeakPtr &codec_manager,
				const FileSystemWeakPtr &file_system);
			~TextureCache() throw();

			const TextureSharedPtr &get_texture(const String &name);
			const TextureSharedPtr &get_error_texture();

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */
