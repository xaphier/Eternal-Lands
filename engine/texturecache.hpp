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
			const GlobalVarsConstSharedPtr m_global_vars;
			const FileSystemConstSharedPtr m_file_system;
			TextureSharedPtr m_error_texture;
			TextureSharedPtr m_error_texture_rectangle;

			TextureSharedPtr load_texture(const String &name,
				const String &index, const bool sRGB,
				const bool rectangle, const bool merge_layers);
			TextureSharedPtr do_load_texture(const String &name,
				const String &index, const bool sRGB,
				const bool rectangle, const bool merge_layers)
				const;
			TextureSharedPtr do_load_texture(
				const ImageConstSharedPtr &image,
				const String &name, const bool rectangle) const;

			inline const GlobalVarsConstSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const FileSystemConstSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

		public:
			TextureCache(
				const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system);
			~TextureCache() noexcept;
			const TextureSharedPtr &get_texture(const String &name,
				const bool sRGB, const bool rectangle);
			const TextureSharedPtr &get_error_texture(
				const bool rectangle);
			TextureSharedPtr get_texture(
				const ImageConstSharedPtr &image,
				const bool rectangle) const;
			TextureSharedPtr get_texture_array(
				const StringVector &image_names,
				const String &name, const bool sRGB) const;
			TextureSharedPtr get_texture_array(
				const ImageConstSharedPtrVector &images,
				const String &name) const;
			TextureSharedPtr get_texture_array(
				const ImageConstSharedPtr &image,
				const String &name) const;

	};

}

#endif	/* UUID_25fe0306_e55e_4b1a_bbfe_fd208c58d327 */
