/****************************************************************************
 *            shadersourceparameterbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "autoparameterutil.hpp"
#include "commonparameterutil.hpp"
#include "exceptions.hpp"
#include "uniformbufferusage.hpp"

namespace eternal_lands
{

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source, const String &name,
		const ParameterType type,
		const ParameterQualifierType qualifier,
		const ParameterSizeType size, const Uint16 scale)
	{
		return ShaderSourceParameter(source, name, type, qualifier,
			size, scale);
	}

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source,
		const CommonParameterType common_parameter,
		const ParameterQualifierType qualifier)
	{
		return ShaderSourceParameter(source, common_parameter,
			qualifier);
	}

	ShaderSourceParameter ShaderSourceParameterBuilder::build(
		const String &source, const AutoParameterType auto_parameter)
	{
		return ShaderSourceParameter(source, auto_parameter);
	}

	bool ShaderSourceParameterBuilder::add_parameter(const String &source,
		const CommonParameterType common_parameter,
		const ParameterQualifierType qualifier,
		ShaderSourceParameterVector &parameters)
	{
		return add_parameter(build(source, common_parameter, qualifier),
			parameters);
	}

	bool ShaderSourceParameterBuilder::add_parameter(const String &source,
		const AutoParameterType auto_parameter,
		ShaderSourceParameterVector &parameters)
	{
		return add_parameter(build(source, auto_parameter), parameters);
	}

	bool ShaderSourceParameterBuilder::add_parameter(const String &source,
		const String &name, const ParameterType type,
		const ParameterQualifierType qualifier,
		const ParameterSizeType size, const Uint16 scale,
		ShaderSourceParameterVector &parameters)
	{
		return add_parameter(build(source, name, type, qualifier, size,
			scale), parameters);
	}

	bool ShaderSourceParameterBuilder::add_parameter(
		const ShaderSourceParameter &parameter,
		ShaderSourceParameterVector &parameters)
	{
		BOOST_FOREACH(const ShaderSourceParameter &tmp, parameters)
		{
			if (tmp.get_name() == parameter.get_name())
			{
				if (tmp.get_use(parameter))
				{
					return false;
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

		return true;
	}

	bool ShaderSourceParameterBuilder::add_parameter(
		const ShaderSourceParameter &parameter,
		const ShaderSourceParameterVector &locals,
		ShaderSourceParameterVector &parameters,
		UniformBufferUsage &uniform_buffers)
	{
		BOOST_FOREACH(const ShaderSourceParameter &tmp, parameters)
		{
			if (tmp.get_name() == parameter.get_name())
			{
				if (tmp.get_use(parameter))
				{
					return false;
				}
			}
		}

		BOOST_FOREACH(const ShaderSourceParameter &tmp, locals)
		{
			if (tmp.get_name() == parameter.get_name())
			{
				if (tmp.get_use(parameter))
				{
					return false;
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

		if (!uniform_buffers.add_parameter(parameter))
		{
			parameters.push_back(parameter);

			return true;
		}

		return false;
	}

	bool ShaderSourceParameterBuilder::add_local(
		const ShaderSourceParameter &local,
		ShaderSourceParameterVector &locals,
		const ShaderSourceParameterVector &parameters,
		const UniformBufferUsage &uniform_buffers)
	{
		BOOST_FOREACH(const ShaderSourceParameter &tmp, parameters)
		{
			if (tmp.get_name() == local.get_name())
			{
				if (tmp.get_use(local))
				{
					return false;
				}
			}
		}

		BOOST_FOREACH(const ShaderSourceParameter &tmp, locals)
		{
			if (tmp.get_name() == local.get_name())
			{
				if (tmp.get_use(local))
				{
					return false;
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

		if (!uniform_buffers.used_parameter(local))
		{
			locals.push_back(local);

			return true;
		}

		return false;
	}

}
