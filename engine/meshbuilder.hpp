/****************************************************************************
 *            meshbuilder.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b23e3aa2_d5bf_4877_afef_b2fbd05da5ac
#define	UUID_b23e3aa2_d5bf_4877_afef_b2fbd05da5ac

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MeshBuilder.
 * This file contains the @c class MeshBuilder.
 */
namespace eternal_lands
{

	enum VertexFormatType
	{
		vft_mesh = 0,
		vft_animated_mesh,
		vft_morph_mesh,
		vft_instanced_mesh,
		vft_mesh_extra_uv,
		vft_animated_mesh_extra_uv,
		vft_morph_mesh_extra_uv,
		vft_instanced_mesh_extra_uv,
		vft_sprite,
		vft_font
	};

	class MeshBuilder: public boost::noncopyable
	{
		private:
			typedef std::map<VertexFormatType,
				VertexFormatSharedPtr> VertexFormatsMap;

			const GlobalVarsSharedPtr m_global_vars;
			VertexFormatsMap m_vertex_formats;

			void set_format(const VertexFormatType format,
				const VertexElements &elements);

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

		public:
			MeshBuilder(const GlobalVarsSharedPtr &global_vars);
			~MeshBuilder() throw();
			AbstractMeshSharedPtr get_mesh(const String &name,
				const bool static_indices = true,
				const bool static_vertices = true) const;
			AbstractMeshSharedPtr get_mesh(
				const VertexFormatType vertex_format,
				const MeshDataToolSharedPtr &mesh_data_tool,
				const String &name,
				const bool static_indices = true,
				const bool static_vertices = true) const;
			VertexBuffersSharedPtr get_vertex_buffers(
				const VertexFormatType vertex_format,
				const Uint32 vertex_count) const;
			VertexFormatSharedPtr get_vertex_format(
				const VertexFormatType vertex_format) const;
			static const String &get_str(
				const VertexFormatType type);
			static VertexFormatType get_type(const String &str);

	};

	OutStream& operator<<(OutStream &str, const VertexFormatType value);
	InStream& operator>>(InStream &str, VertexFormatType &value);

}

#endif	/* UUID_b23e3aa2_d5bf_4877_afef_b2fbd05da5ac */
