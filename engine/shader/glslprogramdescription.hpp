/****************************************************************************
 *            glslprogramdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_941ea873_7cf7_4c50_aaf5_979b201b0946
#define	UUID_941ea873_7cf7_4c50_aaf5_979b201b0946

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class GlslProgramDescription.
 * This file contains the @c class GlslProgramDescription.
 */
namespace eternal_lands
{

	/**
	 * @brief This class describes a glsl program.
	 *
	 * An glsl program description is defined throught its vertex,
	 * fragment, geometry, tesselation control and tesselation evaluation
	 * shader.
	 */
	class GlslProgramDescription
	{
		private:
			String m_vertex_shader;
			String m_tess_control_shader;
			String m_tess_evaluation_shader;
			String m_geometry_shader;
			String m_fragment_shader;

			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

		public:
			/**
			 * Default constructor.
			 */
			GlslProgramDescription();

			/**
			 * Default constructor.
			 */
			GlslProgramDescription(const String &vertex_shader,
				const String &tess_control_shader,
				const String &tess_evaluation_shader,
				const String &geometry_shader,
				const String &fragment_shader);

			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void save_xml(const String &file_name) const;

			/**
			 * Default destructor.
			 */
			~GlslProgramDescription() noexcept;
			bool operator==(
				const GlslProgramDescription &description)
				const;
			bool operator!=(
				const GlslProgramDescription &description)
				const;
			bool operator<(
				const GlslProgramDescription &description)
				const;

			inline void set_vertex_shader(
				const String &vertex_shader) noexcept
			{
				m_vertex_shader = vertex_shader;
			}

			inline void set_tess_control_shader(
				const String &tess_control_shader) noexcept
			{
				m_tess_control_shader = tess_control_shader;
			}

			inline void set_tess_evaluation_shader(
				const String &tess_evaluation_shader) noexcept
			{
				m_tess_evaluation_shader =
					tess_evaluation_shader;
			}

			inline void set_geometry_shader(
				const String &geometry_shader) noexcept
			{
				m_geometry_shader = geometry_shader;
			}

			inline void set_fragment_shader(
				const String &fragment_shader) noexcept
			{
				m_fragment_shader = fragment_shader;
			}

			inline const String &get_vertex_shader() const noexcept
			{
				return m_vertex_shader;
			}

			inline const String &get_tess_control_shader() const
				noexcept
			{
				return m_tess_control_shader;
			}

			inline const String &get_tess_evaluation_shader() const
				noexcept
			{
				return m_tess_evaluation_shader;
			}

			inline const String &get_geometry_shader() const
				noexcept
			{
				return m_geometry_shader;
			}

			inline const String &get_fragment_shader() const
				noexcept
			{
				return m_fragment_shader;
			}

	};

	OutStream &operator<<(OutStream &str,
		const GlslProgramDescription &value);

}

#endif	/* UUID_941ea873_7cf7_4c50_aaf5_979b201b0946 */
