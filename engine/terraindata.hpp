/****************************************************************************
 *            terraindata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_64143e6a_403e_4ec0_8a06_c83556939595
#define	UUID_64143e6a_403e_4ec0_8a06_c83556939595

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "transformation.hpp"

/**
 * @file
 * @brief The @c class TerrainData.
 * This file contains the @c class TerrainData.
 */
namespace eternal_lands
{

	class TerrainData
	{
		private:
			StringArray4 m_albedo_maps;
			String m_blend_map;
			String m_height_map;
			String m_dudv_map;
			glm::vec3 m_translation;
			glm::vec2 m_dudv_scale;

		protected:
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

		public:
			TerrainData();
			TerrainData(const StringArray4 &albedo_maps,
				const String &blend_map,
				const String &height_map,
				const String &dudv_map,
				const glm::vec3 &translation,
				const glm::vec2 &dudv_scale);
			~TerrainData() throw();

			inline void set_albedo_maps(
				const StringArray4 &albedo_maps)
			{
				m_albedo_maps = albedo_maps;
			}

			inline void set_albedo_map(const String &albedo_map,
				const Uint16 index)
			{
				m_albedo_maps[index] = albedo_map;
			}

			inline void set_blend_map(const String &blend_map)
			{
				m_blend_map = blend_map;
			}

			inline void set_height_map(const String &height_map)
			{
				m_height_map = height_map;
			}

			inline void set_dudv_map(const String &dudv_map)
			{
				m_dudv_map = dudv_map;
			}

			inline void set_translation(
				const glm::vec3 &translation)
			{
				m_translation = translation;
			}

			inline void set_dudv_scale(const glm::vec2 &dudv_scale)
			{
				m_dudv_scale = dudv_scale;
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

			inline const String &get_dudv_map() const
			{
				return m_dudv_map;
			}

			inline const glm::vec3 &get_translation() const
			{
				return m_translation;
			}

			inline const glm::vec2 &get_dudv_scale() const
			{
				return m_dudv_scale;
			}

	};

}

#endif	/* UUID_64143e6a_403e_4ec0_8a06_c83556939595 */
