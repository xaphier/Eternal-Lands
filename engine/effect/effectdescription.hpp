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
#include "shader/shaderoutpututil.hpp"

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
			String m_main;
			String m_lighting;
			EffectDescriptionType m_description;
			ShaderOutputType m_output;
			bool m_node_based;
			bool m_receives_shadows;
			bool m_transparent;

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

			inline void set_main(const String &main) noexcept
			{
				m_main = main;
			}

			inline void set_lighting(const String &lighting)
				noexcept
			{
				m_lighting = lighting;
			}

			inline void set_node_based(const bool node_based)
				noexcept
			{
				m_node_based = node_based;
			}

			inline void set_receives_shadows(
				const bool receives_shadows) noexcept
			{
				m_receives_shadows = receives_shadows;
			}

			inline void set_description(
				const EffectDescriptionType description)
				noexcept
			{
				m_description = description;
			}

			inline void set_output(const ShaderOutputType output)
				noexcept
			{
				m_output = output;
			}

			inline void set_transparent(const bool transparent)
				noexcept
			{
				m_transparent = transparent;
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

			inline const String &get_main() const noexcept
			{
				return m_main;
			}

			inline const String &get_lighting() const noexcept
			{
				return m_lighting;
			}

			inline bool get_node_based() const noexcept
			{
				return m_node_based;
			}

			inline EffectDescriptionType get_description() const
				noexcept
			{
				return m_description;
			}

			inline ShaderOutputType get_output() const noexcept
			{
				return m_output;
			}

			inline bool get_receives_shadows() const noexcept
			{
				return m_receives_shadows;
			}

			inline bool get_transparent() const noexcept
			{
				return m_transparent;
			}

	};

	OutStream& operator<<(OutStream &str, const EffectDescription &value);

}

#endif	/* UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f */
