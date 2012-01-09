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

	enum WrapModeType
	{
		wmt_none = 0,
		wmt_word = 1,
		wmt_char = 2
	};

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
				const Uint32 min_line, const Uint32 max_line,
				const float max_width, const float max_height,
				const WrapModeType wrap,
				VertexBuffersSharedPtr &buffers, Uint32 &count)
					const;
			Uint32 check_line(const Text &text, const Uint32 index,
				const float max_width,
				const Utf32Char last_char_code,
				const bool word_wrap, const bool new_line)
				const;

		public:
			/**
			 * Default constructor.
			 */
			TextureFontCache(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system,
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
				const Uint32 min_line, const Uint32 max_line,
				const float max_width, const float max_height,
				const WrapModeType wrap = wmt_none);
			void draw(StateManager &state_manager,
				const AbstractMeshSharedPtr &mesh,
				const Uint32 count) const;
			void draw(StateManager &state_manager,
				const VertexBuffersSharedPtr &buffers,
				const Uint32 count) const;
			Uint32 build_mesh(const Text &text,
				const glm::vec2 &position,
				const Uint32 min_line, const Uint32 max_line,
				const float max_width, const float max_height,
				const WrapModeType wrap,
				AbstractMeshSharedPtr &mesh, Uint32 &count)
				const;
			void build_mesh(
				const VertexBuffersSharedPtr &buffers,
				const Uint32 count,
				AbstractMeshSharedPtr &mesh) const;
			Uint32 write_to_stream(const Text &text,
				const VertexStreamsSharedPtr &streams,
				const glm::vec2 &start_position,
				const Uint32 min_line, const Uint32 max_line,
				const float max_width, const float max_height,
				const WrapModeType wrap, Uint32 &line) const;
			float get_width(const Text &text) const;
			float get_height(const String &font) const;

			inline const GlslProgramSharedPtr &get_program() const
			{
				return m_program;
			}

	};

}

#endif	/* UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4 */
