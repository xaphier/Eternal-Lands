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
#include "shaderblenddata.hpp"
#include "shaderblendutil.hpp"
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
			typedef std::vector<ShaderBlendData>
				ShaderBlendDataVector;

			ShaderBlendDataVector m_data;
			BitSet64 m_use_randomized_uvs;
			bool m_use_specular;
			bool m_use_texture_arrays;

			void write_blend(const String &indent,
				const Uint16 index, OutStream &str) const;
			void write_blend_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void write_mix_result(const String &indent,
				OutStream &str) const;
			void write_albedo_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void write_specular_fetch(const String &indent,
				const Uint16 index, const bool use_glsl_130,
				OutStream &str) const;
			void save_xml(const XmlWriterSharedPtr &writer) const;
			bool get_use_normal_map() const;
			bool get_use_displacement_map() const;
			bool get_use_randomized_uv() const;
			virtual ShaderSourceParameterVector get_parameters(
				const ShaderVersionType version) const;
			virtual String get_source(
				const ShaderVersionType version) const;
			virtual void load_xml_node(const xmlNodePtr node);

		public:
			ShaderSourceTerrain();
			~ShaderSourceTerrain() noexcept;
			void save_xml(const String &file_name) const;
			virtual ShaderSourceType get_type() const;
			virtual bool get_has_data(
				const ShaderVersionType version) const;
			static String get_xml_id();
			static Uint32 get_non_array_sampler_count();
			static SamplerParameterType get_albedo_sampler(
				const Uint16 index);
			static SamplerParameterType get_specular_sampler(
				const Uint16 index);
			static SamplerParameterType get_blend_sampler();
			static SamplerParameterType get_dudv_sampler();
			static SamplerParameterType get_displacement_sampler();
			static SamplerParameterType get_normal_sampler();

			inline void resize(const Uint16 size)
			{
				RANGE_CECK_MAX(size,
					m_use_randomized_uvs.size() - 1,
					UTF8("size too big."));

				m_data.resize(size);
			}

			inline void clear()
			{
				m_data.clear();
			}

			inline const ShaderBlendData &get_data(
				const Uint16 index) const
			{
				RANGE_CECK_MAX(index, m_data.size(),
					UTF8("index too big."));

				return m_data[index];
			}

			inline void set_data(const ShaderBlendData &data,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index, m_data.size(),
					UTF8("index too big."));

				m_data[index] = data;
			}

			inline bool get_use_specular() const noexcept
			{
				return m_use_specular;
			}

			inline void set_use_specular(const bool use_specular)
				noexcept
			{
				m_use_specular = use_specular;
			}

			inline bool get_use_texture_arrays() const noexcept
			{
				return m_use_texture_arrays;
			}

			inline void set_use_texture_arrays(
				const bool use_texture_arrays) noexcept
			{
				m_use_texture_arrays = use_texture_arrays;
			}

			inline bool get_use_randomized_uv(const Uint16 index)
				const
			{
				RANGE_CECK_MAX(index,
					m_use_randomized_uvs.size(),
					UTF8("index too big."));

				return m_use_randomized_uvs[index];
			}

			inline void set_use_randomized_uvs(
				const bool use_randomized_uvs,
				const Uint16 index)
			{
				RANGE_CECK_MAX(index,
					m_use_randomized_uvs.size(),
					UTF8("index too big."));

				m_use_randomized_uvs[index] =
					use_randomized_uvs;
			}

	};

}

#endif	/* UUID_bdba47be_bf5a_4315_b031_ea6027e40175 */
