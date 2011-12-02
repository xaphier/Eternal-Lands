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
				const bool m_dynamic_range;
				const bool m_signed_elements;
				const bool m_normalized;

			public:
				inline VertexElementTypeData(
					const String &name,
					const Uint32 size, const GLenum gl_type,
					const Uint16 element_count,
					const bool dynamic_range,
					const bool signed_elements,
					const bool normalized):
					m_name(name), m_size(size),
					m_gl_type(gl_type),
					m_element_count(element_count),
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
			VertexElementTypeData(String(L"float1"),
				1 * sizeof(GLfloat), GL_FLOAT, 1, true, true,
				false),
			VertexElementTypeData(String(L"float2"),
				2 * sizeof(GLfloat), GL_FLOAT, 2, true, true,
				false),
			VertexElementTypeData(String(L"float3"),
				3 * sizeof(GLfloat), GL_FLOAT, 3, true, true,
				false),
			VertexElementTypeData(String(L"float4"),
				4 * sizeof(GLfloat), GL_FLOAT, 4, true, true,
				false),
			VertexElementTypeData(String(L"half2"),
				2 * sizeof(GLhalf), GL_HALF_FLOAT, 2, true,
				true, false),
			VertexElementTypeData(String(L"half4"),
				4 * sizeof(GLhalf), GL_HALF_FLOAT, 4, true,
				true, false),
			VertexElementTypeData(String(L"short2"),
				2 * sizeof(GLshort), GL_SHORT, 2, false, true,
				false),
			VertexElementTypeData(String(L"short2_normlaized"),
				2 * sizeof(GLshort), GL_SHORT, 2, false, true,
				true),
			VertexElementTypeData(String(L"short4"),
				4 * sizeof(GLshort), GL_SHORT, 4, false, true,
				false),
			VertexElementTypeData(String(L"short4_normlaized"),
				4 * sizeof(GLshort),
				GL_SHORT, 4, false, true, true),
			VertexElementTypeData(String(L"ushort2"),
				2 * sizeof(GLushort), GL_UNSIGNED_SHORT, 2,
				false, false, false),
			VertexElementTypeData(String(L"ushort2_normlaized"),
				2 * sizeof(GLushort),
				GL_UNSIGNED_SHORT, 2, false, false, true),
			VertexElementTypeData(String(L"ushort4"),
				4 * sizeof(GLushort), GL_UNSIGNED_SHORT, 4,
				false, false, false),
			VertexElementTypeData(String(L"ushort4_normlaized"),
				4 * sizeof(GLushort),
				GL_UNSIGNED_SHORT, 4, false, false, true),
			VertexElementTypeData(String(L"byte4"),
				4 * sizeof(GLbyte), GL_BYTE, 4, false, true,
				false),
			VertexElementTypeData(String(L"byte4_normlaized"),
				4 * sizeof(GLbyte), GL_BYTE, 4, false, true,
				true),
			VertexElementTypeData(String(L"ubyte4"),
				4 * sizeof(GLubyte), GL_UNSIGNED_BYTE, 4,
				false, false, false),
			VertexElementTypeData(String(L"ubyte4_normlaized"),
				4 * sizeof(GLubyte),
				GL_UNSIGNED_BYTE, 4, false, false, true),
			VertexElementTypeData(String(L"signed_xyz10_w2"),
				sizeof(GLint),
				GL_INT_2_10_10_10_REV, 4, false, true, false),
			VertexElementTypeData(
				String(L"signed_xyz10_w2_normlaized"),
				sizeof(GLint),
				GL_INT_2_10_10_10_REV, 4, false, true, true),
			VertexElementTypeData(String(L"unsigned_xyz10_w2"),
				sizeof(GLuint),
				GL_UNSIGNED_INT_2_10_10_10_REV, 4, false,
				false, false),
			VertexElementTypeData(
				String(L"unsigned_xyz10_w2_normlaized"),
				sizeof(GLuint),
				GL_UNSIGNED_INT_2_10_10_10_REV, 4, false,
				false, true),
		};

		const Uint32 vertex_element_type_datas_count =
			sizeof(vertex_element_type_datas) /
			sizeof(VertexElementTypeData);

		const String vertex_semantic_type_names[] =
		{
			String(L"position"),
			String(L"normal"),
			String(L"tangent"),
			String(L"color"),
			String(L"bone_weight"),
			String(L"bone_index"),
			String(L"extra_bone_weight"),
			String(L"extra_bone_index"),
			String(L"texture_coordinate_0"),
			String(L"texture_coordinate_1"),
			String(L"layer_index"),
			String(L"morph_position"),
			String(L"morph_normal"),
			String(L"morph_tangent"),
			String(L"morph_texture_coordinate_0"),
			String(L"morph_texture_coordinate_1")
		};

		const Uint32 vertex_semantic_type_names_count =
			sizeof(vertex_semantic_type_names) / sizeof(String);

	}

	VertexElement::VertexElement(const VertexSemanticType semantic,
		const VertexElementType type, const Uint16 offset):
		m_semantic(semantic), m_type(type),
		m_gl_type(get_gl_type(type)), m_offset(offset),
		m_count(get_count(type)), m_size(get_size(type)),
		m_gl_normalized(get_gl_normalized(type))
	{
	}

	VertexElement::~VertexElement() throw()
	{
	}

	const String &VertexElement::get_str(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_name();
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
				<< boost::errinfo_type_info_name(
					"VertexSemanticType"));
		}

		return vertex_semantic_type_names[vertex_semantic_type];
	}

	VertexElementType VertexElement::get_vertex_element_type(
		const String &str)
	{
		Uint32 i;
		VertexElementType vertex_element_type;

		for (i = 0; i < vertex_element_type_datas_count; i++)
		{
			vertex_element_type = static_cast<VertexElementType>(i);

			if (str == get_str(vertex_element_type))
			{
				return vertex_element_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name("VertexElementType"));
	}

	VertexSemanticType VertexElement::get_vertex_semantic_type(
		const String &str)
	{
		Uint32 i;
		VertexSemanticType vertex_semantic_type;

		for (i = 0; i < vertex_semantic_type_names_count; i++)
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
			<< boost::errinfo_type_info_name("VertexSemanticType"));
	}

	bool VertexElement::get_is_vertex_semantic_type(const String &str)
	{
		Uint32 i;
		VertexSemanticType vertex_semantic_type;

		for (i = 0; i < vertex_semantic_type_names_count; i++)
		{
			vertex_semantic_type =
				static_cast<VertexSemanticType>(i);

			if (str == get_str(vertex_semantic_type))
			{
				return true;
			}
		}

		return false;
	}

	Uint32 VertexElement::get_size(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[vertex_element_type].get_size();
	}

	Uint32 VertexElement::get_count(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_element_count();
	}

	GLenum VertexElement::get_gl_type(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_gl_type();
	}

	bool VertexElement::get_supported(
		const VertexElementType vertex_element_type)
	{
		switch (vertex_element_type)
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
				return GLEW_ARB_half_float_vertex;
			case vet_signed_xyz10_w2:
			case vet_signed_xyz10_w2_normalized:
			case vet_unsigned_xyz10_w2:
			case vet_unsigned_xyz10_w2_normalized:
				return GLEW_ARB_vertex_type_2_10_10_10_rev;
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(vertex_element_type, "VertexElementType");
	}

	bool VertexElement::get_signed_elements(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_signed_elements();
	}

	bool VertexElement::get_dynamic_range(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_dynamic_range();
	}

	bool VertexElement::get_normalized(
		const VertexElementType vertex_element_type)
	{
		if (vertex_element_type_datas_count <= vertex_element_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					vertex_element_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					vertex_element_type))
				<< boost::errinfo_type_info_name(
					"VertexElementType"));
		}

		return vertex_element_type_datas[
			vertex_element_type].get_normalized();
	}

	GLboolean VertexElement::get_gl_normalized(
		const VertexElementType vertex_element_type)
	{
		if (get_normalized(vertex_element_type))
		{
			return GL_TRUE;
		}
		else
		{
			return GL_FALSE;
		}
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

		value = VertexElement::get_vertex_element_type(String(string));

		return str;
	}

	InStream& operator>>(InStream &str, VertexSemanticType &value)
	{
		StringType string;

		str >> string;

		value = VertexElement::get_vertex_semantic_type(String(string));

		return str;
	}

}
