/****************************************************************************
 *            abstractterrainmanager.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_45c1c534_56a4_4295_a35c_b836fe76721d
#define	UUID_45c1c534_56a4_4295_a35c_b836fe76721d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TerrainManager.
 * This file contains the @c class TerrainManager.
 */
namespace eternal_lands
{

	class AbstractTerrainManager: public boost::noncopyable
	{
		private:
			StringArray4 m_albedo_maps;
			String m_blend_map;
			String m_height_map;
			const String m_name;
			glm::vec3 m_offset;
			glm::vec3 m_scale;

		protected:
			void load_xml(const FileSystemSharedPtr &file_system);

			inline void set_albedo_maps(
				const StringArray4 &albedo_maps)
			{
				m_albedo_maps = albedo_maps;
			}

			inline void set_blend_map(
				const String &blend_map)
			{
				m_blend_map = blend_map;
			}

			inline void set_height_map(const String &height_map)
			{
				m_height_map = height_map;
			}

			inline void set_offset(const glm::vec3 &offset)
			{
				m_offset = offset;
			}

			inline void set_scale(const glm::vec3 &scale)
			{
				m_scale = scale;
			}

		public:
			AbstractTerrainManager(const String &name);
			virtual ~AbstractTerrainManager() throw();
//			virtual void intersect(const Frustum &frustum,
//				RStarTreeObjectVisitor &visitor) const = 0;

			inline const String &get_name() const
			{
				return m_name;
			}

			inline const StringArray4 &get_albedo_maps() const
			{
				return m_albedo_maps;
			}

			inline const String &get_albedo_map(const Uint16 index)
				const
			{
				return m_albedo_maps[index];
			}

			inline const String &get_blend_map() const
			{
				return m_blend_map;
			}

			inline const String &get_height_map() const
			{
				return m_height_map;
			}

			inline const glm::vec3 &get_offset() const
			{
				return m_offset;
			}

			inline const glm::vec3 &get_scale() const
			{
				return m_scale;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
