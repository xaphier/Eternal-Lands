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
 * @brief The @c class MeshCache.
 * This file contains the @c class MeshCache.
 */
namespace eternal_lands
{

	enum VertexFormatType
	{
		vft_mesh = 0,
		vft_animated_mesh,
		vft_morph_mesh,
		vft_instanced_mesh,
		vft_mesh_tangent,
		vft_animated_mesh_tangent,
		vft_morph_mesh_tangent,
		vft_instanced_mesh_tangent,
		vft_mesh_extra_uv,
		vft_animated_mesh_extra_uv,
		vft_morph_mesh_extra_uv,
		vft_instanced_mesh_extra_uv,
		vft_mesh_tangent_extra_uv,
		vft_animated_mesh_tangent_extra_uv,
		vft_morph_mesh_tangent_extra_uv,
		vft_instanced_mesh_tangent_extra_uv,
		vft_rect,
		vft_sprite
	};

	class MeshBuilder: public boost::noncopyable
	{
		private:
			typedef std::map<VertexFormatType,
				VertexFormatSharedPtr> VertexFormatsMap;

			VertexFormatsMap m_vertex_formats;
			const GlobalVarsSharedPtr m_global_vars;

			void set_format(const VertexFormatType format,
				const VertexElements &elements);

		public:
			MeshBuilder(const GlobalVarsSharedPtr &global_vars);
			~MeshBuilder() throw();
			AbstractMeshSharedPtr get_mesh() const;
			AbstractMeshSharedPtr get_mesh(
				const VertexFormatType vertex_format,
				const MeshDataToolSharedPtr &mesh_data_tool)
				const;
			VertexFormatSharedPtr get_vertex_format(
				const VertexFormatType vertex_format) const;
			static const String &get_str(
				const VertexFormatType type);
			static VertexFormatType get_type(const String &str);

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

	};

	OutStream& operator<<(OutStream &str, const VertexFormatType value);
	InStream& operator>>(InStream &str, VertexFormatType &value);

}

#endif	/* UUID_b23e3aa2_d5bf_4877_afef_b2fbd05da5ac */
