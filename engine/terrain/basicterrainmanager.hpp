/****************************************************************************
 *            basicterrainmanager.hpp
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
#include "transformation.hpp"

/**
 * @file
 * @brief The @c class BasicTerrainManager.
 * This file contains the @c class BasicTerrainManager.
 */
namespace eternal_lands
{

	class BasicTerrainManager: public boost::noncopyable
	{
		private:
			Transformation m_transformation;
			StringArray4 m_albedo_maps;
			String m_blend_map;
			String m_height_map;
			const String m_name;
			glm::vec4 m_rotation;
			glm::vec3 m_translation;
			float m_height_scale;
			Uint16 m_tile_size;

		protected:
			void load_xml(const FileSystemSharedPtr &file_system);
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

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

			inline void set_blend_map(
				const String &blend_map)
			{
				m_blend_map = blend_map;
			}

			inline void set_height_map(const String &height_map)
			{
				m_height_map = height_map;
			}

			inline void set_height_scale(const float height_scale)
			{
				m_height_scale = height_scale;
			}

			inline void set_tile_size(const Uint16 tile_size)
			{
				m_tile_size = tile_size;
			}

			inline void set_transformation(
				const Transformation &transformation)
			{
				m_transformation = transformation;
			}

		public:
			BasicTerrainManager(const String &name);
			virtual ~BasicTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;

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

			inline const Transformation &get_transformation() const
			{
				return m_transformation;
			}

			inline const float get_height_scale() const
			{
				return m_height_scale;
			}

			inline Uint16 get_tile_size() const
			{
				return m_tile_size;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
