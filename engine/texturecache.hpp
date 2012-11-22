/****************************************************************************
 *            texturecache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			TextureSharedPtr m_error_texture;

			TextureSharedPtr load_texture(const String &name,
				const String &index, const bool sRGB,
				const bool merge_layers);
			TextureSharedPtr do_load_texture(const String &name,
				const String &index, const bool sRGB,
				const bool merge_layers) const;
			TextureSharedPtr do_load_texture(
				const ImageSharedPtr &image,
				const String &name) const;

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
			TextureCache(const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);
			~TextureCache() noexcept;
			const TextureSharedPtr &get_texture(const String &name,
				const bool sRGB);
			const TextureSharedPtr &get_error_texture();
			TextureSharedPtr get_texture(
				const ImageSharedPtr &image) const;
			TextureSharedPtr get_texture_array(
				const StringVector &image_names,
				const String &name, const bool sRGB) const;
			TextureSharedPtr get_texture_array(
				const ImageSharedPtrVector &images,
				const String &name) const;
			TextureSharedPtr get_texture_array(
				const ImageSharedPtr &image, const String &name)
				const;

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */
