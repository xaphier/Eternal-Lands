/****************************************************************************
 *            abstractterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
 * @brief The @c class AbstractTerrainManager.
 * This file contains the @c class AbstractTerrainManager.
 */
namespace eternal_lands
{

	class AbstractTerrainManager: public boost::noncopyable
	{
		private:
			Transformation m_transformation;
			StringArray4 m_albedo_maps;
			String m_blend_map;
			String m_height_map;
			String m_dvdu_map;

		protected:
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
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

			inline void set_blend_map(const String &blend_map)
			{
				m_blend_map = blend_map;
			}

			inline void set_height_map(const String &height_map)
			{
				m_height_map = height_map;
			}

			inline void set_dvdu_map(const String &dvdu_map)
			{
				m_dvdu_map = dvdu_map;
			}

			inline void set_transformation(
				const Transformation &transformation)
			{
				m_transformation = transformation;
			}

		public:
			AbstractTerrainManager();
			virtual ~AbstractTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const = 0;

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

			inline const String &get_dvdu_map() const
			{
				return m_dvdu_map;
			}

			inline const Transformation &get_transformation() const
			{
				return m_transformation;
			}

			static inline float get_height_scale()
			{
				return 0.1f / 257.0f;
			}

			static inline Uint16 get_tile_size()
			{
				return 16;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
