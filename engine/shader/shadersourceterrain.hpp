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
				const bool use_extra_map, OutStream &str) const;
			void write_albedo_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void write_extra_fetch(const String &indent,
				const Uint16 index, OutStream &str) const;
			virtual ShaderSourceParameterVector get_parameters(
				const ShaderVersionType version) const;
			virtual String get_source(
				const ShaderVersionType version) const
				override;
			virtual void do_load_xml(const xmlNodePtr node)
				override;
			virtual void do_save_xml(
				const XmlWriterSharedPtr &xml_writer) const
				override;

		public:
			ShaderSourceTerrain();
			virtual ~ShaderSourceTerrain() noexcept override;
			virtual ShaderSourceType get_type() const override;
			virtual bool get_has_data(
				const ShaderVersionType version) const override;
			static String get_xml_id();
			static Uint32 get_non_array_albedo_sampler_count();
			static Uint32 get_non_array_blend_sampler_count();
			static SamplerParameterType get_albedo_sampler(
				const Uint16 index);
			static SamplerParameterType get_extra_sampler();
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

			inline bool get_use_any_blend_size_sampler() const
			{
				return m_material_data.
					get_use_any_blend_size_sampler();
			}

			inline bool get_use_any_none_blend_size_sampler() const
			{
				return m_material_data.
					get_use_any_none_blend_size_sampler();
			}

			inline bool get_use_any_extra_map() const
			{
				return m_material_data.get_use_any_extra_map();
			}

			inline Uint32 get_blend_datas_size() const
			{
				return m_material_data.get_blend_datas_size();
			}

			inline void resize(const Uint16 size)
			{
				m_material_data.resize(size);
			}

			inline void clear()
			{
				m_material_data.clear();
			}

			inline const BlendData &get_blend_data(
				const Uint16 index) const
			{
				return m_material_data.get_blend_data(index);
			}

			inline void set_blend_data(
				const BlendData &blend_data,
				const Uint16 index)
			{
				m_material_data.set_blend_data(blend_data,
					index);
			}

			inline void append_blend_data(
				const BlendData &blend_data)
			{
				m_material_data.append_blend_data(blend_data);
			}

			inline const BlendDataVector &get_blend_datas()
				const
			{
				return m_material_data.get_blend_datas();
			}

			inline void set_blend_datas(
				const BlendDataVector &blend_datas)
			{
				m_material_data.set_blend_datas(blend_datas);
			}

			inline bool get_use_extra_map(const Uint16 index) const
			{
				return m_material_data.get_use_extra_map(index);
			}

			inline const BitSet64 &get_use_extra_maps() const
			{
				return m_material_data.get_use_extra_maps();
			}

			inline void set_use_extra_map(const bool use_extra_map,
				const Uint16 index)
			{
				m_material_data.set_use_extra_map(
					use_extra_map, index);
			}

			inline void set_use_extra_maps(
				const BitSet64 &use_extra_maps)
			{
				m_material_data.set_use_extra_maps(
					use_extra_maps);
			}

			inline bool get_use_blend_size_sampler(
				const Uint16 index) const
			{
				return m_material_data.
					get_use_blend_size_sampler(index);
			}

			inline const BitSet64 &get_use_blend_size_samplers()
				const
			{
				return m_material_data.
					get_use_blend_size_samplers();
			}

			inline void set_use_blend_size_sampler(
				const bool use_blend_size_sampler,
				const Uint16 index)
			{
				m_material_data.set_use_blend_size_sampler(
					use_blend_size_sampler, index);
			}

			inline void set_use_blend_size_samplers(
				const BitSet64 &use_blend_size_samplers)
			{
				m_material_data.set_use_blend_size_samplers(
					use_blend_size_samplers);
			}

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
