/****************************************************************************
 *            texturefontcache.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4
#define	UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextureGlyphe.
 * This file contains the @c class TextureGlyphe.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for texture glyphe.
	 *
	 * @c class for texture glyphe.
	 */
	class TextureFontCache
	{
		private:
			typedef boost::ptr_map<String, TextureFont>
				StringTextureFontMap;

			AtlasSharedPtr m_atlas;
			ImageSharedPtr m_image;
			TextureSharedPtr m_texture;
			AbstractMeshSharedPtr m_mesh;
			VertexBuffersSharedPtr m_buffers;
			GlslProgramSharedPtr m_program;
			StringTextureFontMap m_fonts;
			DoubleSharedArray m_data;

			void update_texture();
			void load_xml(const FileSystemSharedPtr &file_system,
				const xmlNodePtr node);
			void load_font(const FileSystemSharedPtr &file_system,
				const xmlNodePtr node);
			Uint32 build_buffer(const Text &text,
				const glm::vec2 &position,
				const Uint32 max_lines, const float max_width,
				VertexBuffersSharedPtr &buffers, Uint32 &count)
					const;

		public:
			/**
			 * Default constructor.
			 */
			TextureFontCache(const FileSystemSharedPtr &file_system,
				const MeshBuilderSharedPtr &mesh_builder,
				const Uint16 width, const Uint16 height,
				const Uint16 max_char_count);

			/**
			 * Default destructor.
			 */
			~TextureFontCache() throw();
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void add_font(const FileSystemSharedPtr &file_system,
				const String &name, const String &file_name,
				const float size);
			Uint32 draw(StateManager &state_manager,
				const Text &text, const glm::vec2 &position,
				const Uint32 max_lines = 1,
				const float max_width =
					std::numeric_limits<Uint32>::max());
			void draw(StateManager &state_manager,
				const AbstractMeshSharedPtr &mesh,
				const Uint32 count) const;
			Uint32 build_mesh(const Text &text,
				const glm::vec2 &position,
				const Uint32 max_lines, const float max_width,
				AbstractMeshSharedPtr &mesh, Uint32 &count)
				const;

			glm::vec2 get_size(const Text &text) const;

			inline const GlslProgramSharedPtr &get_program() const
			{
				return m_program;
			}

	};

}

#endif	/* UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4 */
