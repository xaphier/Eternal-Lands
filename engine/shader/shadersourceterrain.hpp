/****************************************************************************
 *            shadersourceterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bdba47be_bf5a_4315_b031_ea6027e40175
#define	UUID_bdba47be_bf5a_4315_b031_ea6027e40175

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "terrain/terrainmaterialdata.hpp"
#include "abstractshadersource.hpp"
#include "samplerparameterutil.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceTerrain.
 * This file contains the @c class ShaderSourceTerrain.
 */
namespace eternal_lands
{

	class ShaderSourceTerrain: public AbstractShaderSource
	{
		private:
			TerrainMaterialData m_material_data;

			void write_blend(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_blend_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void write_mix_result(const String &indent,
				const bool write_height,
				const bool write_specular_gloss, OutStream &str)
				const;
			void write_albedo_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void write_height_fetch(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_specular_fetch(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_gloss_fetch(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_height_value(const String &indent,
				const Uint16 index, OutStream &str,
				Uint16 &texture_index) const;
			void write_specular_value(const String &indent,
				const Uint16 index, OutStream &str,
				Uint16 &texture_index) const;
			void write_gloss_value(const String &indent,
				const Uint16 index, OutStream &str,
				Uint16 &texture_index) const;
			virtual ShaderSourceParameterVector get_parameters(
				const ShaderType shader_type,
				const ShaderVersionType version) const;
			virtual String get_source(const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			virtual void do_load_xml(const xmlNodePtr node)
				override;
			virtual void do_save_xml(
				const XmlWriterSharedPtr &xml_writer) const
				override;
			void load_layer_xml(const xmlNodePtr node,
				const Uint32 index);

		public:
			ShaderSourceTerrain();
			virtual ~ShaderSourceTerrain() noexcept override;
			virtual ShaderSourceType get_type() const override;
			virtual bool get_has_data(const ShaderType shader_type,
				const ShaderVersionType version) const override;
			static String get_xml_id();
			static Uint32 get_non_array_albedo_sampler_count()
				noexcept;
			static Uint32 get_non_array_blend_sampler_count()
				noexcept;
			static SamplerParameterType get_albedo_sampler(
				const Uint16 index);
			static SamplerParameterType get_albedo_array_sampler(
				const bool use_blend_size_texture) noexcept;
			static SamplerParameterType get_height_sampler()
				noexcept;
			static SamplerParameterType get_specular_sampler()
				noexcept;
			static SamplerParameterType get_gloss_sampler()
				noexcept;
			static SamplerParameterType get_blend_sampler(
				const Uint16 index);

			inline const TerrainMaterialData &get_material_data()
				const
			{
				return m_material_data;
			}

			inline void set_material_data(
				const TerrainMaterialData &material_data)
			{
				m_material_data = material_data;
			}

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
