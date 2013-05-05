/****************************************************************************
 *            meshbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		/**
		 * two component uv (u0, v0, 0, 1)
		 */
		vft_mesh = 0,
		vft_animated_mesh_2_bones,
		vft_animated_mesh_4_bones,
		vft_animated_mesh_6_bones,
		vft_animated_mesh_8_bones,
		vft_morph_mesh,
		vft_instanced_mesh,
		/**
		 * four component uv (u0, v0, u1, v1)
		 */
		vft_mesh_extra_uv,
		vft_animated_mesh_2_bones_extra_uv,
		vft_animated_mesh_4_bones_extra_uv,
		vft_animated_mesh_6_bones_extra_uv,
		vft_animated_mesh_8_bones_extra_uv,
		vft_morph_mesh_extra_uv,
		vft_instanced_mesh_extra_uv,
		vft_simple_terrain,
		vft_cdlod_terrain,
		vft_sprite,
		vft_font
	};

	class MeshBuilder: public boost::noncopyable
	{
		private:
			typedef std::map<VertexFormatType,
				VertexFormatConstSharedPtr> VertexFormatsMap;

			const GlobalVarsConstSharedPtr m_global_vars;
			const HardwareBufferMapperConstWeakPtr
				m_hardware_buffer_mapper;
			VertexFormatsMap m_vertex_formats;

			void set_format(const VertexFormatType format,
				const VertexElements &elements);

			void set_format(const VertexFormatType format,
				const VertexElementsVector &elements);

			inline const GlobalVarsConstSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const HardwareBufferMapperConstWeakPtr
				&get_hardware_buffer_mapper() const noexcept
			{
				return m_hardware_buffer_mapper;
			}

		public:
			MeshBuilder(const GlobalVarsConstSharedPtr &global_vars,
				const HardwareBufferMapperConstWeakPtr
					&hardware_buffer_mapper);
			~MeshBuilder() noexcept;
			AbstractMeshSharedPtr get_mesh(const String &name,
				const bool static_indices = true,
				const bool static_vertices = true,
				const bool static_instances = true) const;
			AbstractMeshSharedPtr get_mesh(
				const VertexFormatType vertex_format,
				const MeshDataToolSharedPtr &mesh_data_tool,
				const String &name, const Uint32 instances = 0,
				const bool static_indices = true,
				const bool static_vertices = true,
				const bool static_instances = true) const;
			VertexBuffersSharedPtr get_vertex_buffers(
				const VertexFormatType vertex_format,
				const Uint32 vertex_count) const;
			VertexFormatConstSharedPtr get_vertex_format(
				const VertexFormatType vertex_format) const;
			static const String &get_str(
				const VertexFormatType type);
			static VertexFormatType get_type(const String &str);

	};

	OutStream &operator<<(OutStream &str, const VertexFormatType value);
	InStream &operator>>(InStream &str, VertexFormatType &value);

}

#endif	/* UUID_b23e3aa2_d5bf_4877_afef_b2fbd05da5ac */
