/****************************************************************************
 *            shadersourceparameterbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "autoparameterutil.hpp"
#include "commonparameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source, const String &name,
		const ParameterType type,
		const ParameterQualifierType qualifier,
		const Uint16 array_size)
	{
		return ShaderSourceParameter(source, name, type, qualifier,
			pst_one, array_size);
	}

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source, const String &name,
		const ParameterType type,
		const ParameterQualifierType qualifier,
		const ParameterSizeType size, const Uint16 array_size)
	{
		return ShaderSourceParameter(source, name, type, qualifier,
			size, array_size);
	}

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source,
		const CommonParameterType common_parameter,
		const ParameterQualifierType qualifier)
	{
		return ShaderSourceParameter(source,
			CommonParameterUtil::get_str(common_parameter),
			CommonParameterUtil::get_type(common_parameter),
			qualifier,
			CommonParameterUtil::get_size(common_parameter),
			CommonParameterUtil::get_scale(common_parameter));
	}

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source, const AutoParameterType auto_parameter)
	{
		return ShaderSourceParameter(source,
			AutoParameterUtil::get_str(auto_parameter),
			AutoParameterUtil::get_type(auto_parameter), pqt_in,
			AutoParameterUtil::get_size(auto_parameter),
			AutoParameterUtil::get_scale(auto_parameter));
	}

	void ShaderSourceParameterBuilder::add_parameter(
		const ShaderSourceParameter &parameter,
		const ShaderSourceParameterVector &locals,
		ShaderSourceParameterVector &parameters)
	{
		BOOST_FOREACH(const ShaderSourceParameter &tmp, parameters)
		{
			if (tmp.get_name() == parameter.get_name())
			{
				if (tmp.get_use(parameter))
				{
					return;
				}
			}
		}

		BOOST_FOREACH(const ShaderSourceParameter &tmp, locals)
		{
			if (tmp.get_name() == parameter.get_name())
			{
				if (tmp.get_use(parameter))
				{
					return;
				}
			}
		}

		if (parameter.get_qualifier() == pqt_inout)
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' has "
				"qualifier %2% and there is no possible "
				"input."), parameter % pqt_inout,
				InvalidParameterException());
		}

		parameters.push_back(parameter);
	}

	void ShaderSourceParameterBuilder::add_local(
		const ShaderSourceParameter &local,
		ShaderSourceParameterVector &locals,
		const ShaderSourceParameterVector &parameters)
	{
		BOOST_FOREACH(const ShaderSourceParameter &tmp, parameters)
		{
			if (tmp.get_name() == local.get_name())
			{
				if (tmp.get_use(local))
				{
					return;
				}
			}
		}

		BOOST_FOREACH(const ShaderSourceParameter &tmp, locals)
		{
			if (tmp.get_name() == local.get_name())
			{
				if (tmp.get_use(local))
				{
					return;
				}
			}
		}

		if (local.get_qualifier() == pqt_inout)
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' has "
				"qualifier %2% and there is no possible "
				"input."), local % pqt_inout,
				InvalidParameterException());
		}

		locals.push_back(local);
	}

}
