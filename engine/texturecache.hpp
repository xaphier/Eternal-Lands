/****************************************************************************
 *            texturecache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			typedef std::map<String, TextureSharedPtr>
				TextureCacheMap;

			TextureCacheMap m_texture_cache;
			const CodecManagerWeakPtr m_codec_manager;
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			TextureSharedPtr m_error_texture;

			TextureSharedPtr load_texture(const String &name,
				const String &index);
			TextureSharedPtr do_load_texture(const String &name,
				const String &index) const;
			TextureSharedPtr do_load_texture(
				const ImageSharedPtr &image,
				const String &name) const;

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
			TextureCache(const CodecManagerWeakPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);
			~TextureCache() noexcept;
			const TextureSharedPtr &get_texture(const String &name);
			const TextureSharedPtr &get_error_texture();
			TextureSharedPtr get_texture(
				const ImageSharedPtr &image) const;
			TextureSharedPtr get_texture_array(
				const StringVector &image_names,
				const String &name) const;
			TextureSharedPtr get_texture_array(
				const ImageSharedPtrVector &images,
				const String &name) const;

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */
