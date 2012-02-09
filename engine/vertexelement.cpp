/****************************************************************************
 *            vertexelement.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexelement.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class VertexElementTypeData
		{
			private:
				const String m_name;
				const Uint32 m_size;
				const GLenum m_gl_type;
				const Uint16 m_element_count;
				const PackFormatType m_pack_format;
				const bool m_dynamic_range;
				const bool m_signed_elements;
				const bool m_normalized;

			public:
				inline VertexElementTypeData(
					const String &name,
					const Uint32 size, const GLenum gl_type,
					const Uint16 element_count,
					const PackFormatType pack_format,
					const bool dynamic_range,
					const bool signed_elements,
					const bool normalized):
					m_name(name), m_size(size),
					m_gl_type(gl_type),
					m_element_count(element_count),
					m_pack_format(pack_format),
					m_dynamic_range(dynamic_range),
					m_signed_elements(signed_elements),
					m_normalized(normalized)
				{
					assert(get_element_count() > 0);
					assert(!(get_normalized() &&
						get_dynamic_range()));
				}

				inline ~VertexElementTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline Uint32 get_size() const
				{
					return m_size;
				}

				inline GLenum get_gl_type() const
				{
					return m_gl_type;
				}

				inline Uint16 get_element_count() const
				{
					return m_element_count;
				}

				inline PackFormatType get_pack_format() const
				{
					return m_pack_format;
				}

				inline bool get_dynamic_range() const
				{
					return m_dynamic_range;
				}

				inline bool get_signed_elements() const
				{
					return m_signed_elements;
				}

				inline bool get_normalized() const
				{
					return m_normalized;
				}

				inline GLboolean get_gl_normalized() const
				{
					if (m_normalized)
					{
						return GL_TRUE;
					}
					else
					{
						return GL_FALSE;
					}
				}

		};

		const VertexElementTypeData vertex_element_type_datas[] =
		{
			VertexElementTypeData(String(UTF8("float1")),
				1 * sizeof(GLfloat), GL_FLOAT, 1, pft_float_1,
				true, true, false),
			VertexElementTypeData(String(UTF8("float2")),
				2 * sizeof(GLfloat), GL_FLOAT, 2, pft_float_2,
				true, true, false),
			VertexElementTypeData(String(UTF8("float3")),
				3 * sizeof(GLfloat), GL_FLOAT, 3, pft_float_3,
				true, true, false),
			VertexElementTypeData(String(UTF8("float4")),
				4 * sizeof(GLfloat), GL_FLOAT, 4, pft_float_4,
				true, true, false),
			VertexElementTypeData(String(UTF8("half2")),
				2 * sizeof(GLhalf), GL_HALF_FLOAT, 2,
				pft_half_2, true, true, false),
			VertexElementTypeData(String(UTF8("half4")),
				4 * sizeof(GLhalf), GL_HALF_FLOAT, 4,
				pft_half_4, true, true, false),
			VertexElementTypeData(String(UTF8("short2")),
				2 * sizeof(GLshort), GL_SHORT, 2,
				pft_signed_short_2, false, true, false),
			VertexElementTypeData(String(UTF8("short2_normalized")),
				2 * sizeof(GLshort), GL_SHORT, 2,
				pft_signed_normalized_short_2, false, true,
				true),
			VertexElementTypeData(String(UTF8("short4")),
				4 * sizeof(GLshort), GL_SHORT, 4,
				pft_signed_short_4, false, true, false),
			VertexElementTypeData(String(UTF8("short4_normalized")),
				4 * sizeof(GLshort), GL_SHORT, 4,
				pft_signed_normalized_short_4, false, true,
				true),
			VertexElementTypeData(String(UTF8("ushort2")),
				2 * sizeof(GLushort), GL_UNSIGNED_SHORT, 2,
				pft_unsigned_short_2, false, false, false),
			VertexElementTypeData(String(UTF8("ushort2_normalized")),
				2 * sizeof(GLushort), GL_UNSIGNED_SHORT, 2,
				pft_unsigned_normalized_short_2, false, false,
				true),
			VertexElementTypeData(String(UTF8("ushort4")),
				4 * sizeof(GLushort), GL_UNSIGNED_SHORT, 4,
				pft_unsigned_short_4, false, false, false),
			VertexElementTypeData(String(UTF8("ushort4_normalized")),
				4 * sizeof(GLushort), GL_UNSIGNED_SHORT, 4,
				pft_unsigned_normalized_short_4, false, false,
				true),
			VertexElementTypeData(String(UTF8("byte4")),
				4 * sizeof(GLbyte), GL_BYTE, 4,
				pft_signed_byte_4, false, true, false),
			VertexElementTypeData(String(UTF8("byte4_normalized")),
				4 * sizeof(GLbyte), GL_BYTE, 4,
				pft_signed_normalized_byte_4, false, true,
				true),
			VertexElementTypeData(String(UTF8("ubyte4")),
				4 * sizeof(GLubyte), GL_UNSIGNED_BYTE, 4,
				pft_unsigned_byte_4, false, false, false),
			VertexElementTypeData(String(UTF8("ubyte4_normalized")),
				4 * sizeof(GLubyte), GL_UNSIGNED_BYTE, 4,
				pft_unsigned_normalized_byte_4, false, false,
				true),
			VertexElementTypeData(String(UTF8("signed_xyz10_w2")),
				sizeof(GLint), GL_INT_2_10_10_10_REV, 4,
				pft_signed_int_rev_2_10_10_10, false, true,
				false),
			VertexElementTypeData(
				String(UTF8("signed_xyz10_w2_normalized")),
				sizeof(GLint), GL_INT_2_10_10_10_REV, 4,
				pft_signed_normalized_int_rev_2_10_10_10,
				false, true, true),
			VertexElementTypeData(String(UTF8("unsigned_xyz10_w2")),
				sizeof(GLuint), GL_UNSIGNED_INT_2_10_10_10_REV,
				4, pft_unsigned_int_rev_2_10_10_10, false,
				false, false),
			VertexElementTypeData(
				String(UTF8("unsigned_xyz10_w2_normalized")),
				sizeof(GLuint), GL_UNSIGNED_INT_2_10_10_10_REV,
				4, pft_unsigned_normalized_int_rev_2_10_10_10,
				false, false, true)
		};

		const Uint32 vertex_element_type_datas_count =
			sizeof(vertex_element_type_datas) /
			sizeof(VertexElementTypeData);

		const String vertex_semantic_type_names[] =
		{
			String(UTF8("position")),
			String(UTF8("normal")),
			String(UTF8("tangent")),
			String(UTF8("color")),
			String(UTF8("bone_weight")),
			String(UTF8("bone_index")),
			String(UTF8("extra_bone_weight")),
			String(UTF8("extra_bone_index")),
			String(UTF8("texture_coordinate_0")),
			String(UTF8("texture_coordinate_1")),
			String(UTF8("mesh_index")),
			String(UTF8("morph_position")),
			String(UTF8("morph_normal")),
			String(UTF8("morph_tangent")),
			String(UTF8("morph_texture_coordinate_0")),
			String(UTF8("morph_texture_coordinate_1"))
		};

		const Uint32 vertex_semantic_type_names_count =
			sizeof(vertex_semantic_type_names) / sizeof(String);

	}

	VertexElement::VertexElement(const VertexSemanticType semantic,
		const VertexElementType type, const Uint16 offset):
		m_semantic(semantic), m_type(type),
		m_pack_format(get_pack_format(type)),
		m_gl_type(get_gl_type(type)), m_offset(offset),
		m_count(get_count(type)), m_size(get_size(type)),
		m_gl_normalized(get_gl_normalized(type))
	{
	}

	VertexElement::~VertexElement() throw()
	{
	}

	const String &VertexElement::get_str(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[vertex_element].get_name();
	}

	const String &VertexElement::get_str(
		const VertexSemanticType vertex_semantic_type)
	{
		if (vertex_semantic_type_names_count <= vertex_semantic_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_semantic_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_semantic_type))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexSemanticType")));
		}

		return vertex_semantic_type_names[vertex_semantic_type];
	}

	VertexElementType VertexElement::get_vertex_element(const String &str)
	{
		Uint32 i;
		VertexElementType vertex_element;

		for (i = 0; i < vertex_element_type_datas_count; ++i)
		{
			vertex_element = static_cast<VertexElementType>(i);

			if (str == get_str(vertex_element))
			{
				return vertex_element;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"VertexElementType")));
	}

	VertexSemanticType VertexElement::get_vertex_semantic(
		const String &str)
	{
		Uint32 i;
		VertexSemanticType vertex_semantic_type;

		for (i = 0; i < vertex_semantic_type_names_count; ++i)
		{
			vertex_semantic_type =
				static_cast<VertexSemanticType>(i);

			if (str == get_str(vertex_semantic_type))
			{
				return vertex_semantic_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"VertexSemanticType")));
	}

	bool VertexElement::get_vertex_semantic(const String &str,
		VertexSemanticType &vertex_semantic)
	{
		Uint32 i;
		VertexSemanticType vertex_semantic_type;

		for (i = 0; i < vertex_semantic_type_names_count; ++i)
		{
			vertex_semantic_type =
				static_cast<VertexSemanticType>(i);

			if (str == get_str(vertex_semantic_type))
			{
				vertex_semantic = vertex_semantic_type;
				return true;
			}
		}

		return false;
	}

	Uint32 VertexElement::get_vertex_semantic_count()
	{
		return vertex_semantic_type_names_count;
	}

	Uint32 VertexElement::get_size(const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[vertex_element].get_size();
	}

	Uint32 VertexElement::get_count(const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[
			vertex_element].get_element_count();
	}

	GLenum VertexElement::get_gl_type(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[vertex_element].get_gl_type();
	}

	bool VertexElement::get_supported(
		const VertexElementType vertex_element)
	{
		switch (vertex_element)
		{
			case vet_float1:
			case vet_float2:
			case vet_float3:
			case vet_float4:
			case vet_short2:
			case vet_short2_normalized:
			case vet_short4:
			case vet_short4_normalized:
			case vet_ushort2:
			case vet_ushort2_normalized:
			case vet_ushort4:
			case vet_ushort4_normalized:
			case vet_byte4:
			case vet_byte4_normalized:
			case vet_ubyte4:
			case vet_ubyte4_normalized:
				return true;
			case vet_half2:
			case vet_half4:
				return GLEW_VERSION_3_0;
			case vet_signed_xyz10_w2:
			case vet_signed_xyz10_w2_normalized:
			case vet_unsigned_xyz10_w2:
			case vet_unsigned_xyz10_w2_normalized:
				return GLEW_VERSION_3_3;
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(vertex_element, UTF8("VertexElementType"));
	}

	bool VertexElement::get_signed_elements(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[
			vertex_element].get_signed_elements();
	}

	bool VertexElement::get_dynamic_range(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[
			vertex_element].get_dynamic_range();
	}

	bool VertexElement::get_normalized(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[
			vertex_element].get_normalized();
	}

	GLboolean VertexElement::get_gl_normalized(
		const VertexElementType vertex_element)
	{
		if (get_normalized(vertex_element))
		{
			return GL_TRUE;
		}
		else
		{
			return GL_FALSE;
		}
	}

	PackFormatType VertexElement::get_pack_format(
		const VertexElementType vertex_element)
	{
		if (vertex_element_type_datas_count <= vertex_element)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element))
				<< boost::errinfo_type_info_name(UTF8(
					"VertexElementType")));
		}

		return vertex_element_type_datas[
			vertex_element].get_pack_format();
	}

	Uint32 VertexElement::get_vertex_element_count()
	{
		return vertex_element_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const VertexElementType value)
	{
		str << VertexElement::get_str(value);

		return str;
	}

	OutStream& operator<<(OutStream &str, const VertexSemanticType value)
	{
		str << VertexElement::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, VertexElementType &value)
	{
		StringType string;

		str >> string;

		value = VertexElement::get_vertex_element(String(string));

		return str;
	}

	InStream& operator>>(InStream &str, VertexSemanticType &value)
	{
		StringType string;

		str >> string;

		value = VertexElement::get_vertex_semantic(String(string));

		return str;
	}

}
