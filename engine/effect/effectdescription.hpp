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
#include "effectmainutil.hpp"
#include "effectdescriptionutil.hpp"
#include "shader/shaderoutpututil.hpp"
#include "exceptions.hpp"

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
			EffectMainType m_main_type;
			Uint8Array4 m_output_channels;
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
			static String get_xml_id();

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

			inline void set_output_channel_count(const Uint16 value,
				const Uint16 render_target)
			{
				RANGE_CECK_MAX(render_target,
					m_output_channels.size() - 1,
					UTF8("render target too big"));
				RANGE_CECK_MAX(value, 4, UTF8("value too big"));

				m_output_channels[render_target] =
					std::min(value, static_cast<Uint16>(4));
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

			inline glm::bvec4 get_output_channels(
				const Uint16 render_target) const
			{
				RANGE_CECK_MAX(render_target,
					m_output_channels.size() - 1,
					UTF8("render target too big"));

				return glm::bvec4(
					m_output_channels[render_target] > 0,
					m_output_channels[render_target] > 1,
					m_output_channels[render_target] > 2,
					m_output_channels[render_target] > 3);
			}

			inline Uint16 get_output_channel_count(
				const Uint16 render_target) const
			{
				RANGE_CECK_MAX(render_target,
					m_output_channels.size() - 1,
					UTF8("render target too big"));

				return m_output_channels[render_target];
			}

			inline Uint8Array4 get_output_channels() const noexcept
			{
				return m_output_channels;
			}

			inline String get_output_channels_str() const
			{
				StringStream result;
				Uint32 i, count;
				bool first;

				first = true;

				count = m_output_channels.size();

				for (i = 0; i < count; ++i)
				{
					switch (m_output_channels[i])
					{
						case 1:
							result << UTF8("r");
							break;
						case 2:
							result << UTF8("rg");
							break;
						case 3:
							result << UTF8("rgb");
							break;
						case 4:
							result << UTF8("rgba");
							break;
						default:
							continue;
					};

					if (first)
					{
						result << UTF8(" ");
					}

					first = false;
				}

				return String(result.str());
			}

			inline void set_output_channels_str(const String &str)
			{
				std::vector<std::string> lines;
				std::vector<std::string>::iterator it;
				Uint32 i, count;

				boost::split(lines, str.get(),
					boost::is_any_of(UTF8(" ")),
					boost::token_compress_on);

				it = lines.begin();

				while (it != lines.end())
				{
					boost::algorithm::trim(*it);

					if (it->empty())
					{
						it = lines.erase(it);
					}
					else
					{
						it++;
					}
				}

				count = m_output_channels.size();

				for (i = 0; i < count; ++i)
				{
					m_output_channels[i] = 0;
				}

				count = std::min(lines.size(),
					m_output_channels.size());

				for (i = 0; i < count; ++i)
				{
					if ((lines[i] == UTF8("r")) ||
						(lines[i] == UTF8("1")))
					{
						m_output_channels[i] = 1;
						continue;
					}

					if ((lines[i] == UTF8("rg")) ||
						(lines[i] == UTF8("2")))
					{
						m_output_channels[i] = 2;
						continue;
					}

					if ((lines[i] == UTF8("rgb")) ||
						(lines[i] == UTF8("3")))
					{
						m_output_channels[i] = 3;
						continue;
					}

					if ((lines[i] == UTF8("rgba")) ||
						(lines[i] == UTF8("4")))
					{
						m_output_channels[i] = 4;
						continue;
					}
				}
			}

	};

	OutStream &operator<<(OutStream &str, const EffectDescription &value);

}

#endif	/* UUID_214d6e98_25ef_42c5_a3c7_de04d7274a2f */
