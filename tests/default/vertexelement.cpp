/****************************************************************************
 *            vertexelement.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "vertexelement.hpp"
#define BOOST_TEST_MODULE vertex_element
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::VertexSemanticType semantic;
	el::VertexElementType element;
	Uint32 i, j, semantic_count, element_count;

	semantic_count = el::VertexElement::get_vertex_semantic_count();
	element_count = el::VertexElement::get_vertex_element_count();

	for (i = 0; i < semantic_count; ++i)
	{
		for (j = 0; j < element_count; ++j)
		{
			semantic = static_cast<el::VertexSemanticType>(i);
			element = static_cast<el::VertexElementType>(j);

			el::VertexElement data(semantic, element, 0);

			BOOST_CHECK_EQUAL(data.get_semantic(),  semantic);
			BOOST_CHECK_EQUAL(data.get_type(),  element);
		}
	}
}

BOOST_AUTO_TEST_CASE(vertex_semantic_type_convert)
{
	el::VertexSemanticType semantic;
	Uint32 i, count;

	count = el::VertexElement::get_vertex_semantic_count();

	for (i = 0; i < count; ++i)
	{
		semantic = static_cast<el::VertexSemanticType>(i);

		BOOST_CHECK_EQUAL(el::VertexElement::get_vertex_semantic(
			el::VertexElement::get_str(semantic)), semantic);
	}
}

BOOST_AUTO_TEST_CASE(vertex_semantic_type_get_str_value)
{
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_position).get(),
		"position");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_normal).get(),
		"normal");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_tangent).get(),
		"tangent");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_color).get(),
		"color");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_bone_weight).get(),
		"bone_weight");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vst_bone_index).get(),
		"bone_index");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_extra_bone_weight).get(), "extra_bone_weight");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_extra_bone_index).get(), "extra_bone_index");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_texture_coordinate).get(), "texture_coordinate");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_morph_position).get(), "morph_position");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_morph_normal).get(), "morph_normal");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_morph_tangent).get(), "morph_tangent");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vst_morph_texture_coordinate).get(),
			"morph_texture_coordinate");
}

BOOST_AUTO_TEST_CASE(vertex_element_type_convert)
{
	el::VertexElementType element;
	Uint32 i, count;

	count = el::VertexElement::get_vertex_element_count();

	for (i = 0; i < count; ++i)
	{
		element = static_cast<el::VertexElementType>(i);

		BOOST_CHECK_EQUAL(el::VertexElement::get_vertex_element(
			el::VertexElement::get_str(element)), element);
	}
}

BOOST_AUTO_TEST_CASE(vertex_element_type_get_str_value)
{
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vet_float1).get(),
		"float1");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vet_short4_normalized).get(), "short4_normalized");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vet_ushort2).get(),
		"ushort2");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(el::vet_byte4).get(),
		"byte4");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vet_signed_xyz10_w2).get(), "signed_xyz10_w2");
	BOOST_CHECK_EQUAL(el::VertexElement::get_str(
		el::vet_unsigned_xyz10_w2_normalized).get(),
			"unsigned_xyz10_w2_normalized");
}

BOOST_AUTO_TEST_CASE(vertex_element_type_size)
{
	el::VertexElementType element;
	Uint32 i, count;

	count = el::VertexElement::get_vertex_element_count();

	for (i = 0; i < count; ++i)
	{
		element = static_cast<el::VertexElementType>(i);

		BOOST_CHECK_EQUAL(el::VertexElement::get_size(element) % 4, 0);
		BOOST_CHECK_GE(el::VertexElement::get_size(element), 4);
	}
}

BOOST_AUTO_TEST_CASE(vertex_element_type_size_value)
{
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_float1), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_float2), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_float3), 12);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_float4), 16);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_ushort2), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_ushort4), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_short2), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_short4), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_half2), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_half4), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_ushort2_normalized), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_ushort4_normalized), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_short2_normalized), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_short4_normalized), 8);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_ubyte4), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(el::vet_byte4), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_ubyte4_normalized), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_byte4_normalized), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_signed_xyz10_w2), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_unsigned_xyz10_w2), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_signed_xyz10_w2_normalized), 4);
	BOOST_CHECK_EQUAL(el::VertexElement::get_size(
		el::vet_unsigned_xyz10_w2_normalized), 4);
}
