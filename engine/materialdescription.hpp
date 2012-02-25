/****************************************************************************
 *            materialdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_49cf366b_2f4e_481a_ab29_113505e01e90
#define	UUID_49cf366b_2f4e_481a_ab29_113505e01e90

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/shadertextureutil.hpp"

/**
 * @file
 * @brief The @c class MaterialDescription.
 * This file contains the @c class MaterialDescription.
 */
namespace eternal_lands
{

	class MaterialDescription
	{
		private:
			MaterialStringArray m_textures;
			Mat2x4Array4 m_albedo_scale_offsets;
			Mat2x3Array4 m_texture_matrices;
			glm::mat2x3 m_emission_scale_offset;
			glm::vec4 m_specular_scale_offset;
			String m_material_name;
			String m_texture_coodrinates;
			String m_albedo_mapping;
			String m_normal_mapping;
			String m_specular_mapping;
			String m_emission_mapping;
			bool m_receives_shadows;
			bool m_lighting;

		protected:
			inline const String &get_texture(const Uint16 index)
				const
			{
				assert(index < m_textures.size());

				return m_textures[index];
			}

			inline void set_texture(const String &texture,
				const Uint16 index)
			{
				assert(index < m_textures.size());

				m_textures[index] = texture;
			}

			inline void asign(const MaterialDescription &material)
			{
				m_textures = material.m_textures;

				set_albedo_scale_offsets(
					material.get_albedo_scale_offsets());
				set_texture_matrices(
					material.get_texture_matrices());
				set_emission_scale_offset(
					material.get_emission_scale_offset());
				set_specular_scale_offset(
					material.get_specular_scale_offset());
				set_material_name(
					material.get_material_name());
				set_texture_coodrinates(
					material.get_texture_coodrinates());
				set_albedo_mapping(
					material.get_albedo_mapping());
				set_normal_mapping(
					material.get_normal_mapping());
				set_specular_mapping(
					material.get_specular_mapping());
				set_emission_mapping(
					material.get_emission_mapping());
				set_receives_shadows(
					material.get_receives_shadows());
				set_lighting(material.get_lighting());
			}				

		public:
			MaterialDescription();
			MaterialDescription(const xmlNodePtr node);
			~MaterialDescription() throw();
			void load_xml(const xmlNodePtr node);
			void save_xml(const XmlWriterSharedPtr &writer) const;

			inline void set_texture(const String &texture,
				const ShaderTextureType texture_type)
			{
				assert(texture_type < m_textures.size());

				m_textures[texture_type] = texture;
			}

			inline const String &get_texture(
				const ShaderTextureType texture_type) const
			{
				assert(texture_type < m_textures.size());

				return m_textures[texture_type];
			}

			inline const Mat2x4Array4 &get_albedo_scale_offsets()
				const
			{
				return m_albedo_scale_offsets;
			}

			inline const glm::mat2x4 &get_albedo_scale_offset(
				const Uint16 index) const
			{
				return m_albedo_scale_offsets[index];
			}

			inline const Mat2x3Array4 &get_texture_matrices()
				const
			{
				return m_texture_matrices;
			}

			inline const glm::mat2x3 &get_texture_matrix(
				const Uint16 index) const
			{
				return m_texture_matrices[index];
			}

			inline const glm::mat2x3 &get_emission_scale_offset()
				const
			{
				return m_emission_scale_offset;
			}

			inline const glm::vec4 &get_specular_scale_offset()
				const
			{
				return m_specular_scale_offset;
			}

			inline const String &get_material_name() const
			{
				return m_material_name;
			}

			inline const String &get_texture_coodrinates() const
			{
				return m_texture_coodrinates;
			}

			inline const String &get_albedo_mapping() const
			{
				return m_albedo_mapping;
			}

			inline const String &get_normal_mapping() const
			{
				return m_normal_mapping;
			}

			inline const String &get_specular_mapping() const
			{
				return m_specular_mapping;
			}

			inline const String &get_emission_mapping() const
			{
				return m_emission_mapping;
			}

			inline bool get_receives_shadows() const
			{
				return m_receives_shadows;
			}

			inline bool get_lighting() const
			{
				return m_lighting;
			}

			inline void set_albedo_scale_offsets(
				const Mat2x4Array4 &albedo_scale_offsets)
			{
				m_albedo_scale_offsets = albedo_scale_offsets;
			}

			inline void set_albedo_scale_offset(
				const glm::mat2x4 &albedo_scale_offset,
				const Uint16 index)
			{
				m_albedo_scale_offsets[index] =
					albedo_scale_offset;
			}

			inline void set_texture_matrices(
				const Mat2x3Array4 &texture_matrices)
			{
				m_texture_matrices = texture_matrices;
			}

			inline void set_texture_matrix(
				const glm::mat2x3 &texture_matrix,
				const Uint16 index)
			{
				m_texture_matrices[index] =
					texture_matrix;
			}

			inline void set_emission_scale_offset(
				const glm::mat2x3 &emission_scale_offset)
			{
				m_emission_scale_offset =
					emission_scale_offset;
			}

			inline void set_specular_scale_offset(
				const glm::vec4 &specular_scale_offset)
			{
				m_specular_scale_offset =
					specular_scale_offset;
			}

			inline void set_material_name(
				const String &material_name)
			{
				m_material_name = material_name;
			}

			inline void set_texture_coodrinates(
				const String &texture_coodrinates)
			{
				m_texture_coodrinates = texture_coodrinates;
			}

			inline void set_albedo_mapping(
				const String &albedo_mapping)
			{
				m_albedo_mapping = albedo_mapping;
			}

			inline void set_normal_mapping(
				const String &normal_mapping)
			{
				m_normal_mapping = normal_mapping;
			}

			inline void set_specular_mapping(
				const String &specular_mapping)
			{
				m_specular_mapping = specular_mapping;
			}

			inline void set_emission_mapping(
				const String &emission_mapping)
			{
				m_emission_mapping = emission_mapping;
			}

			inline void set_receives_shadows(
				const bool receives_shadows)
			{
				m_receives_shadows = receives_shadows;
			}

			inline void set_lighting(const bool lighting)
			{
				m_lighting = lighting;
			}

	};

}

#endif	/* UUID_49cf366b_2f4e_481a_ab29_113505e01e90 */
