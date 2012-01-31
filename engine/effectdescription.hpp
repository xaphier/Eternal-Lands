/****************************************************************************
 *            effectdescription.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f
#define	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectDescription.
 * This file contains the @c class EffectDescription.
 */
namespace eternal_lands
{

	/**
	 * @brief This class describes an effect.
	 *
	 * An effect description is defined throught its world transform,
	 * lighting informations, texture mappings etc. All these informations
	 * are used to build the shader source that is used to compile and link
	 * the opengl shader.
	 */
	class EffectDescription
	{
		private:
			String m_world_transform;
			String m_texture_coodrinates;
			String m_albedo_mapping;
			String m_normal_mapping;
			String m_specular_mapping;
			String m_emission_mapping;
			bool m_receives_shadows;
			bool m_transparent;
			bool m_lighting;

		public:
			/**
			 * Default constructor.
			 */
			EffectDescription();

			/**
			 * Default destructor.
			 */
			~EffectDescription() throw();
			bool operator==(const EffectDescription &effect) const;
			bool operator!=(const EffectDescription &effect) const;
			bool operator<(const EffectDescription &effect) const;
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

			inline const String &get_world_transform() const
			{
				return m_world_transform;
			}

			inline const String &get_texture_coodrinates() const
			{
				return m_texture_coodrinates;
			}

			inline const String &get_albedo_mapping() const
			{
				return m_albedo_mapping;
			}

			inline const String &get_normal_mapping() const
			{
				return m_normal_mapping;
			}

			inline const String &get_specular_mapping() const
			{
				return m_specular_mapping;
			}

			inline const String &get_emission_mapping() const
			{
				return m_emission_mapping;
			}

			inline bool get_receives_shadows() const
			{
				return m_receives_shadows;
			}

			inline bool get_transparent() const
			{
				return m_transparent;
			}

			inline bool get_lighting() const
			{
				return m_lighting;
			}

			inline void set_world_transform(
				const String &world_transform)
			{
				m_world_transform = world_transform;
			}

			inline void set_texture_coodrinates(
				const String &texture_coodrinates)
			{
				m_texture_coodrinates = texture_coodrinates;
			}

			inline void set_albedo_mapping(
				const String &albedo_mapping)
			{
				m_albedo_mapping = albedo_mapping;
			}

			inline void set_normal_mapping(
				const String &normal_mapping)
			{
				m_normal_mapping = normal_mapping;
			}

			inline void set_specular_mapping(
				const String &specular_mapping)
			{
				m_specular_mapping = specular_mapping;
			}

			inline void set_emission_mapping(
				const String &emission_mapping)
			{
				m_emission_mapping = emission_mapping;
			}

			inline void set_receives_shadows(
				const bool receives_shadows)
			{
				m_receives_shadows = receives_shadows;
			}

			inline void set_transparent(const bool transparent)
			{
				m_transparent = transparent;
			}

			inline void set_lighting(const bool lighting)
			{
				m_lighting = lighting;
			}

	};

	OutStream& operator<<(OutStream &str, const EffectDescription &value);

}

#endif	/* UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f */
