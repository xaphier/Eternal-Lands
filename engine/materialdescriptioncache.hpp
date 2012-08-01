/****************************************************************************
 *            materialdescriptioncache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5cf283c9_73bd_4e24_b609_125cd1315f16
#define	UUID_5cf283c9_73bd_4e24_b609_125cd1315f16

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialDescriptionCache.
 * This file contains the @c class MaterialDescriptionCache.
 */
namespace eternal_lands
{

	class MaterialDescriptionCache: public boost::noncopyable
	{
		private:
			typedef std::map<String, MaterialDescription>
				StringMaterialDescriptionMap;

			StringMaterialDescriptionMap
				m_material_description_cache;

			void load_xml(const xmlNodePtr node);

		public:
			MaterialDescriptionCache();
			~MaterialDescriptionCache() noexcept;
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void save_xml(const String &file_name) const;
			StringVector get_material_description_names() const;
			const MaterialDescription &get_material_description(
				const String &name) const;
			bool get_has_material_description(const String &name)
				const;
			bool get_simple_shadow(const String &name) const;

	};

}

#endif	/* UUID_5cf283c9_73bd_4e24_b609_125cd1315f16 */
