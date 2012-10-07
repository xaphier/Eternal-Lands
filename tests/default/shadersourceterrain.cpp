/****************************************************************************
 *            shadersourceterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersourceterrain.hpp"
#include "shader/samplerparameterutil.hpp"
#include "tools/tests.hpp"
#define BOOST_TEST_MODULE shader_source_terrain
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace eternal_lands;

template <typename T>
bool test_value(const T v0, const T v1)
{
	return v0 == v1;
}

BOOST_AUTO_TEST_CASE(shader_source)
{
	boost::scoped_ptr<ShaderSourceTerrain> shader_source_terrain;

	BOOST_CHECK_NO_THROW(shader_source_terrain.reset(
		new ShaderSourceTerrain()));
}

BOOST_AUTO_TEST_CASE(static_functions)
{
	int i, count;

	BOOST_CHECK_GT(ShaderSourceTerrain::get_non_array_sampler_count(),
		0);

	count = ShaderSourceTerrain::get_non_array_sampler_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(
			ShaderSourceTerrain::get_albedo_sampler(i));
		BOOST_CHECK_NO_THROW(
			ShaderSourceTerrain::get_specular_sampler(i));
	}

	BOOST_CHECK_NO_THROW(ShaderSourceTerrain::get_blend_sampler());
	BOOST_CHECK_NO_THROW(ShaderSourceTerrain::get_dudv_sampler());
	BOOST_CHECK_NO_THROW(
		ShaderSourceTerrain::get_displacement_sampler());
	BOOST_CHECK_NO_THROW(ShaderSourceTerrain::get_normal_sampler());
}

BOOST_AUTO_TEST_CASE(static_functions_extended)
{
	std::set<SamplerParameterType> samplers;
	SamplerParameterType sampler;
	int i, count;

	BOOST_CHECK_GT(ShaderSourceTerrain::get_non_array_sampler_count(),
		0);

	count = ShaderSourceTerrain::get_non_array_sampler_count();

	for (i = 0; i < count; ++i)
	{
		sampler = ShaderSourceTerrain::get_albedo_sampler(i);

		BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
			(samplers)(sampler));

		samplers.insert(sampler);

		sampler = ShaderSourceTerrain::get_specular_sampler(i);

		BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
			(samplers)(sampler));

		samplers.insert(sampler);
	}

	sampler = ShaderSourceTerrain::get_blend_sampler();

	BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
		(samplers)(sampler));

	samplers.insert(sampler);

	sampler = ShaderSourceTerrain::get_dudv_sampler();

	BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
		(samplers)(sampler));

	samplers.insert(sampler);

	sampler = ShaderSourceTerrain::get_displacement_sampler();

	BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
		(samplers)(sampler));

	samplers.insert(sampler);

	sampler = ShaderSourceTerrain::get_normal_sampler();

	BOOST_CHECK_PREDICATE(value_not_in_set<SamplerParameterType>,
		(samplers)(sampler));

	samplers.insert(sampler);
}
