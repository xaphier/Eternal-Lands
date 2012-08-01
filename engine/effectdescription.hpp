/****************************************************************************
 *            effectdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f
#define	UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectdescriptionutil.hpp"

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
	 * An effect description is defined throught its world transformation,
	 * lighting informations, texture mappings etc. All these informations
	 * are used to build the shader source that is used to compile and link
	 * the opengl shader.
	 */
	class EffectDescription
	{
		private:
			String m_name;
			String m_world_transformation;
			String m_texture_coodrinates;
			String m_uv_mapping;
			String m_albedo_mapping;
			String m_normal_mapping;
			String m_specular_mapping;
			String m_emission_mapping;
			EffectDescriptionType m_type;
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
			~EffectDescription() noexcept;
			bool operator==(const EffectDescription &effect) const;
			bool operator!=(const EffectDescription &effect) const;
			bool operator<(const EffectDescription &effect) const;
			void load_xml(const xmlNodePtr node);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void save_xml(const XmlWriterSharedPtr &writer) const;

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_world_transformation(
				const String &world_transformation) noexcept
			{
				m_world_transformation = world_transformation;
			}

			inline void set_texture_coodrinates(
				const String &texture_coodrinates) noexcept
			{
				m_texture_coodrinates = texture_coodrinates;
			}

			inline void set_uv_mapping(const String &uv_mapping)
				noexcept
			{
				m_uv_mapping = uv_mapping;
			}

			inline void set_albedo_mapping(
				const String &albedo_mapping) noexcept
			{
				m_albedo_mapping = albedo_mapping;
			}

			inline void set_normal_mapping(
				const String &normal_mapping) noexcept
			{
				m_normal_mapping = normal_mapping;
			}

			inline void set_specular_mapping(
				const String &specular_mapping) noexcept
			{
				m_specular_mapping = specular_mapping;
			}

			inline void set_emission_mapping(
				const String &emission_mapping) noexcept
			{
				m_emission_mapping = emission_mapping;
			}

			inline void set_receives_shadows(
				const bool receives_shadows) noexcept
			{
				m_receives_shadows = receives_shadows;
			}

			inline void set_type(const EffectDescriptionType type)
				noexcept
			{
				m_type = type;
			}

			inline void set_transparent(const bool transparent)
				noexcept
			{
				m_transparent = transparent;
			}

			inline void set_lighting(const bool lighting) noexcept
			{
				m_lighting = lighting;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const String &get_world_transformation() const
				noexcept
			{
				return m_world_transformation;
			}

			inline const String &get_texture_coodrinates() const
				noexcept
			{
				return m_texture_coodrinates;
			}

			inline const String &get_uv_mapping() const noexcept
			{
				return m_uv_mapping;
			}

			inline const String &get_albedo_mapping() const
				noexcept
			{
				return m_albedo_mapping;
			}

			inline const String &get_normal_mapping() const
				noexcept
			{
				return m_normal_mapping;
			}

			inline const String &get_specular_mapping() const
				noexcept
			{
				return m_specular_mapping;
			}

			inline const String &get_emission_mapping() const
				noexcept
			{
				return m_emission_mapping;
			}

			inline EffectDescriptionType get_type() const noexcept
			{
				return m_type;
			}

			inline bool get_receives_shadows() const noexcept
			{
				return m_receives_shadows;
			}

			inline bool get_transparent() const noexcept
			{
				return m_transparent;
			}

			inline bool get_lighting() const noexcept
			{
				return m_lighting;
			}

	};

	OutStream& operator<<(OutStream &str, const EffectDescription &value);

}

#endif	/* UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f */
