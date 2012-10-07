/****************************************************************************
 *            effectfunctionutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectfunctionutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class EffectFunctionTypeData
		{
			private:
				const String m_name;
				const String m_description;
				const bool m_geometric;
				const bool m_trigonometric;

			public:
				inline EffectFunctionTypeData(
					const String &name,
					const String &description,
					const bool geometric = false,
					const bool trigonometric = false):
						m_name(name),
						m_description(description),
						m_geometric(geometric),
						m_trigonometric(trigonometric)
				{
				}

				inline ~EffectFunctionTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline const String &get_description() const
					noexcept
				{
					return m_description;
				}

				inline bool get_geometric() const noexcept
				{
					return m_geometric;
				}

				inline bool get_trigonometric() const noexcept
				{
					return m_trigonometric;
				}

		};

		const EffectFunctionTypeData effect_function_datas[] =
		{
			EffectFunctionTypeData(String(UTF8("add")),
				String(UTF8("x + y"))),
			EffectFunctionTypeData(String(UTF8("sub")),
				String(UTF8("x - y"))),
			EffectFunctionTypeData(String(UTF8("mul")),
				String(UTF8("x * y"))),
			EffectFunctionTypeData(String(UTF8("div")),
				String(UTF8("x / y"))),
			EffectFunctionTypeData(String(UTF8("mad")),
				String(UTF8("x * y + z"))),
			EffectFunctionTypeData(String(UTF8("radians")),
				String(UTF8("radians converts a quantity, "
					"specified in degrees into radians.\n"
					"That is, the return value is \u03C0 *"
					" degrees / 180")), false, true),
			EffectFunctionTypeData(String(UTF8("degrees")),
				String(UTF8("degrees converts a quantity, "
					"specified in radians into degrees.\n"
					"That is, the return value is 180 * "
					"radians / \u03C0")), false, true),
			EffectFunctionTypeData(String(UTF8("sin")),
				String(UTF8("sin returns the trigonometric "
					"sine of angle.")), false, true),
			EffectFunctionTypeData(String(UTF8("cos")),
				String(UTF8("cos returns the trigonometric "
					"cosine of angle.")), false, true),
			EffectFunctionTypeData(String(UTF8("tan")),
				String(UTF8("tan returns the trigonometric "
					"tangent of angle.")), false, true),
			EffectFunctionTypeData(String(UTF8("asin")),
				String(UTF8("atan returns the angle whose "
					"trigonometric sine is x.\n"
					"The range of values returned by asin"
					" is (- \u03C0 / 2, \u03C0 / 2).\n"
					"The result is undefined if |x|>1.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("acos")),
				String(UTF8("atan returns the angle whose "
					"trigonometric cosine is x.\n"
					"The range of values returned by acos "
					"is (0, \u03C0).\n"
					"The result is undefined if |x|>1.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("atan")),
				String(UTF8("atan returns the angle whose "
					"trigonometric arctangent is "
					"y_over_x.\n"
					"atan returns the angle whose tangent"
					" is y_over_x.\n"
					"Values returned are in the range "
					"(- \u03C0 / 2, \u03C0 / 2).")),
				false, true),
			EffectFunctionTypeData(String(UTF8("atan2")),
				String(UTF8("atan returns the angle whose "
					"trigonometric arctangent is y / x.\n"
					"The signs of y and x are used to "
					"determine the quadrant that the angle"
					" lies in.\n"
					"The values returned by atan in this "
					"case are in the range (−\u03C0, "
					"\u03C0).\n"
					"Results are undefined if x is zero.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("sinh")),
				String(UTF8("sinh returns the hyperbolic sine "
					"of x.\n"
					"The hyperbolic sine of x is computed "
					"as (e\u02E3 - e\u207B\u02E3) / 2.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("cosh")),
				String(UTF8("cosh returns the hyperbolic "
					"cosine of x.\n"
					"The hyperbolic cosine of x is "
					"computed as (e\u02E3 + e\u207B\u02E3)"
					" / 2.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("tanh")),
				String(UTF8("tanh returns the hyperbolic "
					"tangent of x.\n"
					"The hyperbolic tangent of x is "
					"computed as sinh(x) / cosh(x).")),
				false, true),
			EffectFunctionTypeData(String(UTF8("asinh")),
				String(UTF8("asinh returns the arc hyperbolic"
					" sine of x; the inverse of sinh.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("acosh")),
				String(UTF8("acosh returns the arc hyperbolic"
					" cosine of x; the non-negative "
					"inverse of cosh.\n"
					"Results are undefined if x < 1.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("atanh")),
				String(UTF8("atanh returns the arc hyperbolic"
					" tangent of x; the inverse of tanh.\n"
					"Results are undefined if |x| > 1.")),
				false, true),
			EffectFunctionTypeData(String(UTF8("pow")),
				String(UTF8("pow returns the value of x raised"
				" to the y power. i.e., x\u02B8.\n"
				"Results are undefined if x < 0 or if x = 0 "
				"and y = 0."))),
			EffectFunctionTypeData(String(UTF8("exp")),
				String(UTF8("pow returns the natural "
					"exponentiation of x. i.e., "
					"e\u02E3."))),
			EffectFunctionTypeData(String(UTF8("log")),
				String(UTF8("log returns the natural logarithm"
					" of x. i.e., the value y which "
					"satisfies x = e\u02B8."))),
			EffectFunctionTypeData(String(UTF8("exp2")),
				String(UTF8("exp2 returns 2 raised to the "
					"power of x. i.e., 2\u02E3."))),
			EffectFunctionTypeData(String(UTF8("log2")),
				String(UTF8("log2 returns the base 2 logarithm"
					" of x. i.e., the value y which "
					"satisfies x = 2\u02B8.\n"
					"Results are undefined if x \u2264 "
					"0."))),
			EffectFunctionTypeData(String(UTF8("sqrt")),
				String(UTF8("sqrt returns the square root of "
					"x. i.e., the value \u221Ax.\n"
					"Results are undefined if x < 0."))),
			EffectFunctionTypeData(String(UTF8("inversesqrt")),
				String(UTF8("inversesqrt returns the inverse "
					"of the square root of x. i.e., the "
					"value 1 / \u221Ax.\n"
					"Results are undefined if x \u2264 "
					"0."))),
			EffectFunctionTypeData(String(UTF8("abs")),
				String(UTF8("abs returns the absolute value "
					"of x."))),
			EffectFunctionTypeData(String(UTF8("sign")),
				String(UTF8("abs returns 1.0 if x < 0.0, 0.0 "
					"if x = 0.0 and 1.0 if x > 0.0."))),
			EffectFunctionTypeData(String(UTF8("round")),
				String(UTF8("round returns a value equal to "
					"the nearest integer to x.\n"
					"The fraction 0.5 will round in a "
					"direction chosen by the "
					"implementation, presumably the "
					"direction that is fastest.\n"
					"This includes the possibility that "
					"round(x) returns the  same value as "
					"roundEven(x) for all values of x."))),
			EffectFunctionTypeData(String(UTF8("roundEven")),
				String(UTF8("roundEven returns a value equal "
					"to the nearest integer to x.\n"
					"The fractional part of 0.5 will "
					"round toward the nearest even "
					"integer.\n"
					"For example, both 3.5 and 4.5 will "
					"round to 4.0."))),
			EffectFunctionTypeData(String(UTF8("trunc")),
				String(UTF8("trunc returns a a value equal to"
					" the nearest integer to x whose "
					"absolute value is not larger than "
					"the absolute value of x."))),
			EffectFunctionTypeData(String(UTF8("floor")),
				String(UTF8("floor returns a value equal to "
					"the nearest integer that is less than"
					" or equal to x."))),
			EffectFunctionTypeData(String(UTF8("ceil")),
				String(UTF8("ceil returns a value equal to the"
					" nearest integer that is greater than"
					" or equal to x."))),
			EffectFunctionTypeData(String(UTF8("fract")),
				String(UTF8("fract returns the fractional part"
					" of x.\n"
					"This is calculated as x - "
					"floor(x)."))),
			EffectFunctionTypeData(String(UTF8("mod")),
				String(UTF8("mod returns the value of x modulo"
					" y.\n"
					"This is computed as x - y * "
					"floor(x / y)."))),
			EffectFunctionTypeData(String(UTF8("min")),
				String(UTF8("min returns the minimum of the "
					"two parameters.\n"
					"It returns y if y is less than x, "
					"otherwise it returns x."))),
			EffectFunctionTypeData(String(UTF8("max")),
				String(UTF8("max returns the maximum of the "
					"two parameters.\n"
					"It returns y if y is greater than x,"
					" otherwise it returns x."))),
			EffectFunctionTypeData(String(UTF8("clamp")),
				String(UTF8("clamp returns the value of x "
					"constrained to the range minVal to "
					"maxVal.\n"
					"The returned value is computed as "
					"min(max(x, minVal), maxVal)."))),
			EffectFunctionTypeData(String(UTF8("mix")),
				String(UTF8("mix performs a linear "
					"interpolation between x and y using a"
					" to weight between them.\n"
					"The return value is computed as "
					"follows: x * (1 - a) + y * a.")),
				true),
			EffectFunctionTypeData(String(UTF8("step")),
				String(UTF8("step generates a step function "
					"by comparing x to edge.\n"
					"For element i of the return value, "
					"0.0 is returned if x[i] < edge[i], "
					"and 1.0 is returned otherwise.")),
					true),
			EffectFunctionTypeData(String(UTF8("smoothstep")),
				String(UTF8("smoothstep performs smooth "
					"Hermite interpolation between 0 and 1"
					" when edge0 < x < edge1.\n"
					"This is useful in cases where a "
					"threshold function with a smooth "
					"transition is desired.\n"
					"smoothstep is equivalent to:\n"
					"  t = clamp((x - edge0) / (edge1 - "
					"edge0), 0.0, 1.0);\n"
					"  return t * t * (3.0 - 2.0 * t);\n"
					"Results are undefined if edge0 "
					"\u2265 edge1.")), true),
			EffectFunctionTypeData(String(UTF8("reflect")),
				String(UTF8("For a given incident vector I and"
					" surface normal N reflect returns the"
					" reflection direction calculated as I"
					" - 2.0 * dot(N, I) * N.\n"
					"N should be normalized in order to "
					"achieve the desired result.")), true),
			EffectFunctionTypeData(String(UTF8("length")),
				String(UTF8("length returns the length of the "
					"vector. i.e., \u221A x[0]\u00B2 + "
					"x[1]\u00B2 + ...")), true),
			EffectFunctionTypeData(String(UTF8("distance")),
				String(UTF8("distance returns the distance "
					"between the two points p0 and p1. "
					"i.e., length(p0, p1);")), true),
			EffectFunctionTypeData(String(UTF8("dot")),
				String(UTF8("dot returns the dot product of "
					"two vectors, x and y. i.e., x[0] * "
					"y[0] + x[1] * y[1] + ...")), true),
			EffectFunctionTypeData(String(UTF8("normalize")),
				String(UTF8("normalize returns a vector with "
					"the same direction as its parameter, "
					"v, but with length 1.")), true),
			EffectFunctionTypeData(String(UTF8("saturate")),
				String(UTF8("clamp(x, 0.0, 1.0)"))),
			EffectFunctionTypeData(String(UTF8("refract")),
				String(UTF8("For a given incident vector I, "
					"surface normal N and ratio of indices"
					" of refraction, eta, refract returns "
					"the refraction vector, R.\n"
					"R is calculated as:\n"
					"  k = 1.0 - eta * eta * (1.0 - dot(N,"
					" I) * dot(N, I));\n"
					"  if (k < 0.0)\n"
					"    R = 0.0;\n"
					"  else\n"
					"    R = eta * I - (eta * dot(N, I) + "
					"sqrt(k)) * N;\n"
					"The input parameters I and N should "
					"be normalized in order to achieve the"
					" desired result.")), true),
			EffectFunctionTypeData(String(UTF8("derivatives")),
				String(UTF8("dFdx and dFdy return the partial "
					"derivative of expression p in x and "
					"y, respectively.\n"
					"Deviatives are calculated using local"
					" differencing.\n"
					"Expressions that imply higher order"
					" derivatives such as dFdx(dFdx(n)) "
					"have undefined results, as do mixed"
					"-order derivatives such as "
					"dFdx(dFdy(n)).\n"
					"It is assumed that the expression p "
					"is continuous and therefore, "
					"expressions evaluated via non-uniform"
					" control flow may be undefined."))),
			EffectFunctionTypeData(String(UTF8("fwidth")),
				String(UTF8("fwidth returns the sum of the "
					"absolute derivatives in x and y using"
					" local differencing for the input "
					"argument p.\n"
					"It is equivalent to abs(dFdx(p)) + "
					"abs(dFdy(p))."))),
			EffectFunctionTypeData(String(UTF8("ycocg_to_rgb")),
				String(UTF8("ycocg_to_rgb converts an YCoCg ("
					"luma, chroma orange, and chroma green"
					") value to an RGB value."))),
			EffectFunctionTypeData(String(UTF8("rgb_to_ycocg")),
				String(UTF8("rgb_ycocg converts a RGB value "
					"to an YCoCg (luma, chroma orange, and"
					" chroma green) value."))),
			EffectFunctionTypeData(String(UTF8("luminance")),
				String(UTF8("calculates the elative luminance "
					"with:\n"
					"\tY = 0.2126 R + 0.7152 G + 0.0722 "
					"B.")))
		};

		const Uint32 effect_function_datas_count =
			sizeof(effect_function_datas) /
				sizeof(EffectFunctionTypeData);

	}

	const String &EffectFunctionUtil::get_str(
		const EffectFunctionType effect_function)
	{
		if (effect_function_datas_count <= effect_function)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_function_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_function))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectFunctionType")));
		}

		return effect_function_datas[effect_function].get_name();
	}

	const String &EffectFunctionUtil::get_description(
		const EffectFunctionType effect_function)
	{
		if (effect_function_datas_count <= effect_function)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_function_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_function))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectFunctionType")));
		}

		return effect_function_datas[effect_function].get_description();
	}

	bool EffectFunctionUtil::get_geometric(
		const EffectFunctionType effect_function)
	{
		if (effect_function_datas_count <= effect_function)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_function_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_function))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectFunctionType")));
		}

		return effect_function_datas[effect_function].get_geometric();
	}

	bool EffectFunctionUtil::get_trigonometric(
		const EffectFunctionType effect_function)
	{
		if (effect_function_datas_count <= effect_function)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_function_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_function))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectFunctionType")));
		}

		return effect_function_datas[
			effect_function].get_trigonometric();
	}

	EffectFunctionType EffectFunctionUtil::get_effect_function(
		const String &str)
	{
		Uint32 i;
		EffectFunctionType effect_function;

		for (i = 0; i < effect_function_datas_count; ++i)
		{
			effect_function = static_cast<EffectFunctionType>(i);

			if (str == get_str(effect_function))
			{
				return effect_function;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectFunctionType")));
	}

	bool EffectFunctionUtil::get_effect_function(const String &str,
		EffectFunctionType &effect_function) noexcept
	{
		Uint32 i;
		EffectFunctionType tmp;

		for (i = 0; i < effect_function_datas_count; ++i)
		{
			tmp = static_cast<EffectFunctionType>(i);

			if (str == get_str(tmp))
			{
				effect_function = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectFunctionUtil::get_effect_function_count() noexcept
	{
		return effect_function_datas_count;
	}

	OutStream &operator<<(OutStream &str, const EffectFunctionType value)
	{
		str << EffectFunctionUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, EffectFunctionType &value)
	{
		std::string string;

		str >> string;

		value = EffectFunctionUtil::get_effect_function(String(string));

		return str;
	}

}
