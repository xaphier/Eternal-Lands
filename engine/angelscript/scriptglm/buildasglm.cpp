/**
 * This file generates the source file for the angel script engine.
 */

#include <iostream>
#include <sstream>
#include <set>

enum UnaryOperatorType
{
	uot_assign = 0,
	uot_add_assign,
	uot_sub_assign,
	uot_mul_assign,
	uot_div_assign,
	uot_mod_assign,
	uot_and_assign,
	uot_or_assign,
	uot_xor_assign,
	uot_shl_assign,
	uot_shr_assign
};

std::string get_string(const UnaryOperatorType type)
{
	std::string result;

	switch (type)
	{
		case uot_assign:
			result = "=";
			break;
		case uot_add_assign:
			result = "+=";
			break;
		case uot_sub_assign:
			result = "-=";
			break;
		case uot_mul_assign:
			result = "*=";
			break;
		case uot_div_assign:
			result = "/=";
			break;
		case uot_mod_assign:
			result = "%=";
			break;
		case uot_and_assign:
			result = "&=";
			break;
		case uot_or_assign:
			result = "|=";
			break;
		case uot_xor_assign:
			result = "^=";
			break;
		case uot_shl_assign:
			result = "<<=";
			break;
		case uot_shr_assign:
			result = ">>=";
			break;
	}

	return result;
}

std::string get_angelscript_string(const UnaryOperatorType type)
{
	std::string result;

	switch (type)
	{
		case uot_assign:
			result = "opAssign";
			break;
		case uot_add_assign:
			result = "opAddAssign";
			break;
		case uot_sub_assign:
			result = "opSubAssign";
			break;
		case uot_mul_assign:
			result = "opMulAssign";
			break;
		case uot_div_assign:
			result = "opDivAssign";
			break;
		case uot_mod_assign:
			result = "opModAssign";
			break;
		case uot_and_assign:
			result = "opAndAssign";
			break;
		case uot_or_assign:
			result = "opOrAssign";
			break;
		case uot_xor_assign:
			result = "opXorAssign";
			break;
		case uot_shl_assign:
			result = "opShlAssign";
			break;
		case uot_shr_assign:
			result = "opShrAssign";
			break;
	}

	return result;
}

enum BinaryOperatorType
{
	bot_add = 0,
	bot_sub,
	bot_mul,
	bot_div,
	bot_mod,
	bot_and,
	bot_or,
	bot_xor,
	bot_shl,
	bot_shr
};

std::string get_string(const BinaryOperatorType type)
{
	std::string result;

	switch (type)
	{
		case bot_add:
			result = "+";
			break;
		case bot_sub:
			result = "-";
			break;
		case bot_mul:
			result = "*";
			break;
		case bot_div:
			result = "/";
			break;
		case bot_mod:
			result = "%";
			break;
		case bot_and:
			result = "&";
			break;
		case bot_or:
			result = "|";
			break;
		case bot_xor:
			result = "^";
			break;
		case bot_shl:
			result = "<<";
			break;
		case bot_shr:
			result = ">>";
			break;
	}

	return result;
}

std::string get_angelscript_string(const BinaryOperatorType type, const bool swapped)
{
	std::string result;

	switch (type)
	{
		case bot_add:
			result = "opAdd";
			break;
		case bot_sub:
			result = "opSub";
			break;
		case bot_mul:
			result = "opMul";
			break;
		case bot_div:
			result = "opDiv";
			break;
		case bot_mod:
			result = "opMod";
			break;
		case bot_and:
			result = "opAnd";
			break;
		case bot_or:
			result = "opOr";
			break;
		case bot_xor:
			result = "opXor";
			break;
		case bot_shl:
			result = "opShl";
			break;
		case bot_shr:
			result = "opShr";
			break;
	}

	if (swapped)
	{
		return result + "_r";
	}

	return result;
}

enum ValueType
{
	vt_int,
	vt_uint,
	vt_bool,
	vt_float
};

std::string get_angelscript_string(const ValueType type)
{
	switch (type)
	{
		case vt_int:
			return "int";
		case vt_uint:
			return "uint";
		case vt_bool:
			return "bool";
		case vt_float:
			return "float";
	}

	return "";
}

std::string get_string(const ValueType type)
{
	switch (type)
	{
		case vt_int:
			return "int";
		case vt_uint:
			return "unsigned int";
		case vt_bool:
			return "bool";
		case vt_float:
			return "float";
	}

	return "";
}

enum ParameterType
{
	pt_int = 0,
	pt_uint,
	pt_bool,
	pt_float,
	pt_vec2,
	pt_ivec2,
	pt_uvec2,
	pt_bvec2,
	pt_vec3,
	pt_ivec3,
	pt_uvec3,
	pt_bvec3,
	pt_vec4,
	pt_ivec4,
	pt_uvec4,
	pt_bvec4,
	pt_mat2x2,
	pt_mat2x3,
	pt_mat2x4,
	pt_mat3x2,
	pt_mat3x3,
	pt_mat3x4,
	pt_mat4x2,
	pt_mat4x3,
	pt_mat4x4,
	pt_quat
};

ParameterType get_parameter(const ValueType type, const unsigned int count)
{
	switch (count)
	{
		case 1:
			switch (type)
			{
				case vt_int:
					return pt_int;
				case vt_uint:
					return pt_uint;
				case vt_bool:
					return pt_bool;
				case vt_float:
					return pt_float;
			}
			break;
		case 2:
			switch (type)
			{
				case vt_int:
					return pt_ivec2;
				case vt_uint:
					return pt_uvec2;
				case vt_bool:
					return pt_bvec2;
				case vt_float:
					return pt_vec2;
			}
			break;
		case 3:
			switch (type)
			{
				case vt_int:
					return pt_ivec3;
				case vt_uint:
					return pt_uvec3;
				case vt_bool:
					return pt_bvec3;
				case vt_float:
					return pt_vec3;
			}
			break;
		case 4:
			switch (type)
			{
				case vt_int:
					return pt_ivec4;
				case vt_uint:
					return pt_uvec4;
				case vt_bool:
					return pt_bvec4;
				case vt_float:
					return pt_vec4;
			}
			break;
	}

	return pt_float;
}

ValueType get_value(const ParameterType type)
{
	switch (type)
	{
		case pt_int:
		case pt_ivec2:
		case pt_ivec3:
		case pt_ivec4:
			return vt_int;
		case pt_uint:
		case pt_uvec2:
		case pt_uvec3:
		case pt_uvec4:
			return vt_uint;
		case pt_bool:
		case pt_bvec2:
		case pt_bvec3:
		case pt_bvec4:
			return vt_bool;
		case pt_float:
		case pt_vec2:
		case pt_vec3:
		case pt_vec4:
		case pt_mat2x2:
		case pt_mat2x3:
		case pt_mat2x4:
		case pt_mat3x2:
		case pt_mat3x3:
		case pt_mat3x4:
		case pt_mat4x2:
		case pt_mat4x3:
		case pt_mat4x4:
		case pt_quat:
			return vt_float;
	}

	return vt_float;
}

ParameterType get_type(const ParameterType type)
{
	switch (type)
	{
		case pt_int:
		case pt_ivec2:
		case pt_ivec3:
		case pt_ivec4:
			return pt_int;
		case pt_uint:
		case pt_uvec2:
		case pt_uvec3:
		case pt_uvec4:
			return pt_uint;
		case pt_bool:
		case pt_bvec2:
		case pt_bvec3:
		case pt_bvec4:
			return pt_bool;
		case pt_float:
		case pt_vec2:
		case pt_vec3:
		case pt_vec4:
		case pt_mat2x2:
		case pt_mat2x3:
		case pt_mat2x4:
		case pt_mat3x2:
		case pt_mat3x3:
		case pt_mat3x4:
		case pt_mat4x2:
		case pt_mat4x3:
		case pt_mat4x4:
		case pt_quat:
			return pt_float;
	}

	return pt_float;
}

unsigned int get_count(const ParameterType type)
{
	switch (type)
	{
		case pt_int:
		case pt_uint:
		case pt_bool:
		case pt_float:
			return 1;
		case pt_ivec2:
		case pt_uvec2:
		case pt_bvec2:
		case pt_vec2:
			return 2;
		case pt_ivec3:
		case pt_uvec3:
		case pt_bvec3:
		case pt_vec3:
			return 3;
		case pt_bvec4:
		case pt_ivec4:
		case pt_uvec4:
		case pt_vec4:
			return 4;
		case pt_mat2x2:
		case pt_mat2x3:
		case pt_mat2x4:
			return 2;
		case pt_mat3x2:
		case pt_mat3x3:
		case pt_mat3x4:
			return 3;
		case pt_mat4x2:
		case pt_mat4x3:
		case pt_mat4x4:
			return 4;
		case pt_quat:
			return 4;
	}

	return 0;
}

bool get_matrix_counts(const ParameterType type, unsigned int &row, unsigned int &col)
{
	switch (type)
	{
		case pt_int:
		case pt_uint:
		case pt_bool:
		case pt_float:
		case pt_ivec2:
		case pt_uvec2:
		case pt_bvec2:
		case pt_vec2:
		case pt_ivec3:
		case pt_uvec3:
		case pt_bvec3:
		case pt_vec3:
		case pt_bvec4:
		case pt_ivec4:
		case pt_uvec4:
		case pt_vec4:
			return false;
		case pt_mat2x2:
			row = 2;
			col = 2;
			return true;
		case pt_mat2x3:
			row = 2;
			col = 3;
			return true;
		case pt_mat3x2:
			row = 3;
			col = 2;
			return true;
		case pt_mat2x4:
			row = 2;
			col = 4;
			return true;
		case pt_mat4x2:
			row = 4;
			col = 2;
			return true;
		case pt_mat3x3:
			row = 3;
			col = 3;
			return true;
		case pt_mat3x4:
			row = 3;
			col = 4;
			return true;
		case pt_mat4x3:
			row = 4;
			col = 3;
			return true;
		case pt_mat4x4:
			row = 4;
			col = 4;
			return true;
		case pt_quat:
			return false;
	}

	return false;
}

ParameterType get_matrix(const unsigned int row, const unsigned int col)
{
	switch (row)
	{
		case 2:
			switch (col)
			{
				case 2:
					return pt_mat2x2;
				case 3:
					return pt_mat2x3;
				case 4:
					return pt_mat2x4;
			}
		case 3:
			switch (col)
			{
				case 2:
					return pt_mat3x2;
				case 3:
					return pt_mat3x3;
				case 4:
					return pt_mat3x4;
			}
		case 4:
			switch (col)
			{
				case 2:
					return pt_mat4x2;
				case 3:
					return pt_mat4x3;
				case 4:
					return pt_mat4x4;
			}
	}

	return pt_float;
}

std::string get_angelscript_string(const ParameterType type)
{
	std::stringstream str;
	std::string tmp;
	ValueType value;
	unsigned int count, row, col;

	if (type == pt_quat)
	{
		return "quat";
	}

	if (get_matrix_counts(type, row, col))
	{
		str << "mat" << row << "x" << col;
		return str.str();
	}

	count = get_count(type);
	value = get_value(type);

	switch (count)
	{
		case 1:
			return get_angelscript_string(value);
		case 2:
			tmp = "2";
			break;
		case 3:
			tmp = "3";
			break;
		case 4:
			tmp = "4";
			break;
	}

	switch (value)
	{
		case vt_int:
			return "ivec" + tmp;
		case vt_uint:
			return "uvec" + tmp;
		case vt_bool:
			return "bvec" + tmp;
		case vt_float:
			return "vec" + tmp;
	}		

	return "";
}

std::string get_string(const ParameterType type)
{
	std::stringstream str;
	std::string tmp;
	ValueType value;
	unsigned int count, row, col;

	if (type == pt_quat)
	{
		return "glm::quat";
	}

	if (get_matrix_counts(type, row, col))
	{
		str << "glm::mat" << row << "x" << col;
		return str.str();
	}

	count = get_count(type);
	value = get_value(type);

	switch (count)
	{
		case 1:
			return get_string(value);
		case 2:
			tmp = "2";
			break;
		case 3:
			tmp = "3";
			break;
		case 4:
			tmp = "4";
			break;
	}

	switch (value)
	{
		case vt_int:
			return "glm::ivec" + tmp;
		case vt_uint:
			return "glm::uvec" + tmp;
		case vt_bool:
			return "glm::bvec" + tmp;
		case vt_float:
			return "glm::vec" + tmp;
	}		

	return "";
}

const std::string indents[4] = { "", "\t", "\t\t", "\t\t\t" };

inline const std::string &get_indent(const int indent)
{
	return indents[indent];
}

const char swizzel_names[12] = { 'x', 'r', 's', 'y', 'g', 't', 'z', 'b', 'p', 'w', 'a', 'q' };

void register_vector_element(const ParameterType type,
	const int indent)
{
	unsigned int i, size;

	size = get_count(type);

	for (i = 0; i < (size * 3); ++i)
	{
		std::cout << get_indent(indent) << "\tr = engine->RegisterObjectProperty(\"";
		std::cout << get_angelscript_string(type) << "\", \"";
		std::cout << get_angelscript_string(get_value(type)) << " ";
		std::cout << swizzel_names[i] << "\", ";
		std::cout << "asOFFSET(" << get_string(type) << ", ";
		std::cout << swizzel_names[i] << "));" << std::endl;
		std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
	}
}

void register_quat_element(const int indent)
{
	ParameterType type;
	unsigned int i;

	type = pt_quat;

	for (i = 0; i < 4; ++i)
	{
		std::cout << get_indent(indent) << "\tr = engine->RegisterObjectProperty(\"";
		std::cout << get_angelscript_string(type) << "\", \"";
		std::cout << get_angelscript_string(get_value(type)) << " ";
		std::cout << swizzel_names[i * 3] << "\", ";
		std::cout << "asOFFSET(" << get_string(type) << ", ";
		std::cout << swizzel_names[i * 3] << "));" << std::endl;
		std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
	}
}

void register_index_operator(const ParameterType type,
	const ParameterType result, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectMethod(\"";
	std::cout << get_angelscript_string(type);
	std::cout << "\", \"" << get_angelscript_string(result);
	std::cout << " &opIndex(uint)\", " << std::endl;
	std::cout << get_indent(indent) << "\t\tasFUNCTION((get_value_at<" << get_string(type);
	std::cout << ", " << get_string(result) << ", " << get_count(type);
	std::cout << ">)), asCALL_CDECL_OBJLAST);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;

	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectMethod(\"";
	std::cout << get_angelscript_string(type);
	std::cout << "\", \"const " << get_angelscript_string(result);
	std::cout << " &opIndex(uint) const\", " << std::endl;
	std::cout << get_indent(indent) << "\t\tasFUNCTION((get_value_at<" << get_string(type);
	std::cout << ", " << get_string(result) << ", " << get_count(type);
	std::cout << get_indent(indent) << ">)), asCALL_CDECL_OBJLAST);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_operator(const ParameterType type, const ParameterType value,
	const UnaryOperatorType uo, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectMethod(\"";
	std::cout << get_angelscript_string(type);
	std::cout << "\"," << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << get_angelscript_string(type) << " &" << get_angelscript_string(uo);
	std::cout << "(const " << get_angelscript_string(value) << " &in)\"," << std::endl;
	std::cout << get_indent(indent) << "\t\tasMETHODPR(" << get_string(type) << ", operator";
	std::cout << get_string(uo) << ", (const " << get_string(value) << " &), ";
	std::cout << get_string(type) << "&)," << std::endl << get_indent(indent) << "\t\t";
	std::cout << get_indent(indent) << "asCALL_THISCALL);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_operator(const ParameterType type, const ParameterType result,
	const ParameterType parameter, const BinaryOperatorType bo,
	const bool swapped, const std::string &namespace_name,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectMethod(\"" << get_angelscript_string(type);
	std::cout << "\"," << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << get_angelscript_string(result) << " " << get_angelscript_string(bo, swapped);
	std::cout << "(const " << get_angelscript_string(parameter) << " &in)\"," << std::endl;
	std::cout << get_indent(indent) << "\t\tasFUNCTIONPR(";

	if (!namespace_name.empty())
	{
		std::cout << namespace_name << "::";
	}

	std::cout << "operator";
	std::cout << get_string(bo) << "," << std::endl << get_indent(indent) << "\t\t";

	if (swapped)
	{
		std::cout << "(const " << get_string(parameter);
		std::cout << " &, const " << get_string(type) << " &), ";
	}
	else
	{
		std::cout << "(const " << get_string(type);
		std::cout << " &, const " << get_string(parameter) << " &), ";
	}

	std::cout << get_string(result) << ")," << std::endl << get_indent(indent) << "\t\t";

	if (swapped)
	{
		std::cout << "asCALL_CDECL_OBJLAST);" << std::endl;
	}
	else
	{
		std::cout << "asCALL_CDECL_OBJFIRST);" << std::endl;
	}

	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_operator(const ParameterType type, const ParameterType value,
	const BinaryOperatorType bo, const bool swapped,
	const std::string &namespace_name, const int indent)
{
	register_operator(type, type, value, bo, swapped, namespace_name,
		indent);
}

void register_to_string_function(const ParameterType type,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterGlobalFunction(" << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << "string to_string(const " << get_angelscript_string(type);
	std::cout << " &in)\"," << std::endl << get_indent(indent) << "\t\tasFUNCTIONPR(glm::";
	std::cout << "to_string," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "(const " << get_string(type) << " &), ";
	std::cout << "std::string)," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "asCALL_CDECL);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_function_1(const ParameterType result, const ParameterType param_0,
	const std::string &name, const std::string &as_name,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterGlobalFunction(";
	std::cout << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << get_angelscript_string(result) << " " << as_name;
	std::cout << "(const " << get_angelscript_string(param_0) << " &in)\",";
	std::cout << std::endl << get_indent(indent);
	std::cout << "\t\tasFUNCTIONPR(";
	std::cout << name << "," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "(const " << get_string(param_0) << " &), ";
	std::cout << get_string(result) << ")," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "asCALL_CDECL);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_function_2(const ParameterType result, const ParameterType param_0,
	const ParameterType param_1, const std::string &name,
	const std::string &as_name, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterGlobalFunction(";
	std::cout << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << get_angelscript_string(result) << " " << as_name;
	std::cout << "(const " << get_angelscript_string(param_0) << " &in,";
	std::cout << " const " << get_angelscript_string(param_1) << " &in)\",";
	std::cout << std::endl << get_indent(indent) << "\t\tasFUNCTIONPR(";
	std::cout << name << "," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "(const " << get_string(param_0) << " &,";
	std::cout << " const " << get_string(param_1) << " &), ";
	std::cout << get_string(result) << ")," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "asCALL_CDECL);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_function_3(const ParameterType result, const ParameterType param_0,
	const ParameterType param_1, const ParameterType param_2,
	const std::string &name, const std::string &as_name,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterGlobalFunction(";
	std::cout << std::endl << get_indent(indent) << "\t\t\"";
	std::cout << get_angelscript_string(result) << " " << as_name;
	std::cout << "(const " << get_angelscript_string(param_0) << " &in,";
	std::cout << " const " << get_angelscript_string(param_1) << " &in,";
	std::cout << " const " << get_angelscript_string(param_2) << " &in)\",";
	std::cout << std::endl << get_indent(indent);
	std::cout << "\t\tasFUNCTIONPR(";
	std::cout << name << "," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "(const " << get_string(param_0) << " &,";
	std::cout << " const " << get_string(param_1) << " &,";
	std::cout << " const " << get_string(param_2) << " &), ";
	std::cout << get_string(result) << ")," << std::endl << get_indent(indent) << "\t\t";
	std::cout << "asCALL_CDECL);" << std::endl;
	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_function_1(const ParameterType result, const ParameterType param_0,
	const std::string &name, const int indent)
{
	register_function_1(result, param_0, name, name, indent);
}

void register_function_2(const ParameterType result, const ParameterType param_0,
	const ParameterType param_1, const std::string &name,
	const int indent)
{
	register_function_2(result, param_0, param_1, name, name, indent);
}

void register_function_3(const ParameterType result, const ParameterType param_0,
	const ParameterType param_1, const ParameterType param_2,
	const std::string &name, const int indent)
{
	register_function_3(result, param_0, param_1, param_2, name, name, indent);
}

void register_type(const ParameterType type, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectType(\"" << get_angelscript_string(type);
	std::cout << "\", sizeof(" << get_string(type) << ")," << std::endl << get_indent(indent);

	switch (get_value(type))
	{
		case pt_float:
			std::cout << "\t\tasOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asOBJ_APP_CLASS_ALLFLOATS);" << std::endl;
			break;
		case pt_int:
		case pt_uint:
		case pt_bool:
			std::cout << "\t\tasOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK | asOBJ_APP_CLASS_ALLINTS);" << std::endl;
			break;
	}

	std::cout << get_indent(indent) << "\tassert(r >= 0);" << std::endl;
}

void register_vector_float_functions(const ParameterType type,
	const int indent)
{
	register_function_1(type, type, "glm::radians", "radians", indent);
	register_function_1(type, type, "glm::degrees", "degrees", indent);
	register_function_1(type, type, "glm::sin", "sin", indent);
	register_function_1(type, type, "glm::cos", "cos", indent);
	register_function_1(type, type, "glm::tan", "tan", indent);
	register_function_1(type, type, "glm::asin", "asin", indent);
	register_function_1(type, type, "glm::acos", "acos", indent);
	register_function_2(type, type, type, "glm::atan", "atan", indent);
	register_function_1(type, type, "glm::atan", "atan", indent);

	register_function_1(type, type, "glm::sinh", "sinh", indent);
	register_function_1(type, type, "glm::cosh", "cosh", indent);
	register_function_1(type, type, "glm::tanh", "tanh", indent);
	register_function_1(type, type, "glm::asinh", "asinh", indent);
	register_function_1(type, type, "glm::acosh", "acosh", indent);
	register_function_1(type, type, "glm::atanh", "atanh", indent);

	register_function_2(type, type, type, "glm::pow", "pow", indent);
	register_function_1(type, type, "glm::exp", "exp", indent);
	register_function_1(type, type, "glm::log", "log", indent);
	register_function_1(type, type, "glm::exp2", "exp2", indent);
	register_function_1(type, type, "glm::log2", "log2", indent);
	register_function_1(type, type, "glm::sqrt", "sqrt", indent);
	register_function_1(type, type, "glm::inversesqrt", "inversesqrt",
		indent);
	register_function_1(type, type, "glm::abs", "abs", indent);
	register_function_1(type, type, "glm::sign", "sign", indent);
	register_function_1(type, type, "glm::round", "round", indent);
	register_function_1(type, type, "glm::trunc", "trunc", indent);
	register_function_1(type, type, "glm::floor", "floor", indent);

	register_function_1(type, type, "glm::ceil", "ceil", indent);
	register_function_1(type, type, "glm::fract", "fract", indent);
	register_function_2(type, type, type, "glm::mod", "mod", indent);
	register_function_2(type, type, type, "glm::min", "min", indent);
	register_function_2(type, type, type, "glm::max", "max", indent);

	register_function_3(type, type, type, type, "glm::clamp", "clamp",
		indent);
	register_function_3(type, type, type, type, "glm::mix", "mix", indent);
	register_function_2(type, type, type, "glm::step", "step", indent);
	register_function_3(type, type, type, type, "glm::smoothstep",
		"smoothstep", indent);
	register_function_2(type, type, type, "glm::reflect", "reflect",
		indent);
	register_function_3(type, type, type, pt_float, "glm::refract",
		"refract", indent);

	if (type != pt_float)
	{
		register_function_2(type, type, pt_float, "glm::mod", "mod",
			indent);
		register_function_2(type, type, pt_float, "glm::min", "min",
			indent);
		register_function_2(type, type, pt_float, "glm::max", "max",
			indent);
		register_function_3(type, type, pt_float, pt_float,
			"glm::clamp", "clamp", indent);
		register_function_3(type, type, type, pt_float, "glm::mix",
			"mix", indent);
		register_function_2(type, pt_float, type, "glm::step", "step",
			indent);
		register_function_3(type, pt_float, pt_float, type,
			"glm::smoothstep", "smoothstep", indent);
	}

	register_function_1(pt_float, type, "glm::length", "length", indent);
	register_function_2(pt_float, type, type, "glm::distance", "distance",
		indent);
	register_function_2(pt_float, type, type, "dot", indent);

	register_function_1(type, type, "glm::normalize", "normalize", indent);
}

void register_vector_float_functions(const int indent)
{
	register_vector_float_functions(pt_float, indent);
	register_vector_float_functions(pt_vec2, indent);
	register_vector_float_functions(pt_vec3, indent);
	register_vector_float_functions(pt_vec4, indent);
	register_function_2(pt_vec3, pt_vec3, pt_vec3, "glm::cross", "cross",
		indent);
}

void register_vector_compare_functions(const int indent)
{
	ParameterType itype, utype, btype, type;
	unsigned int i;

	for (i = 2; i < 5; ++i)
	{
		itype = get_parameter(vt_int, i);
		utype = get_parameter(vt_uint, i);
		btype = get_parameter(vt_bool, i);
		type = get_parameter(vt_float, i);

		register_function_2(pt_bool, type, type,
			"glm::detail::operator==", "opEquals", indent);

		register_function_2(btype, type, type, "glm::lessThan",
			"lessThan", indent);
		register_function_2(btype, itype, itype, "glm::lessThan",
			"lessThan", indent);
		register_function_2(btype, utype, utype, "glm::lessThan",
			"lessThan", indent);

		register_function_2(btype, type, type, "glm::lessThanEqual",
			"lessThanEqual", indent);
		register_function_2(btype, itype, itype, "glm::lessThanEqual",
			"lessThanEqual", indent);
		register_function_2(btype, utype, utype, "glm::lessThanEqual",
			"lessThanEqual", indent);

		register_function_2(btype, type, type, "glm::greaterThan",
			"greaterThan", indent);
		register_function_2(btype, itype, itype, "glm::greaterThan",
			"greaterThan", indent);
		register_function_2(btype, utype, utype, "glm::greaterThan",
			"greaterThan", indent);

		register_function_2(btype, type, type, "glm::greaterThanEqual",
			"greaterThanEqual", indent);
		register_function_2(btype, itype, itype,
			"glm::greaterThanEqual", "greaterThanEqual", indent);
		register_function_2(btype, utype, utype,
			"glm::greaterThanEqual", "greaterThanEqual", indent);

		register_function_2(btype, type, type, "glm::equal", indent);
		register_function_2(btype, itype, itype, "glm::equal", indent);
		register_function_2(btype, utype, utype, "glm::equal", indent);
		register_function_2(btype, btype, btype, "glm::equal", indent);

		register_function_2(btype, type, type, "glm::notEqual",
			"notEqual", indent);
		register_function_2(btype, itype, itype, "glm::notEqual",
			"notEqual", indent);
		register_function_2(btype, utype, utype, "glm::notEqual",
			"notEqual", indent);
		register_function_2(btype, btype, btype, "glm::notEqual",
			"notEqual", indent);
	}
}

void register_vector_functions(const int indent)
{
	ParameterType itype, utype, btype, type;
	unsigned int i;

	for (i = 2; i < 5; ++i)
	{
		itype = get_parameter(vt_int, i);
		utype = get_parameter(vt_uint, i);
		btype = get_parameter(vt_bool, i);
		type = get_parameter(vt_float, i);

		register_function_1(pt_bool, btype, "glm::all", "all", indent);
		register_function_1(pt_bool, btype, "glm::any", "any", indent);
		register_function_1(btype, btype, "glm::not_", "not_", indent);

		register_function_1(type, type, "glm::detail::operator-",
			"opNeg", indent);
		register_function_1(itype, itype, "glm::detail::operator-",
			"opNeg", indent);

		register_function_1(utype, utype, "glm::detail::operator~",
			"opCom", indent);
		register_function_1(btype, btype, "glm::detail::operator~",
			"opCom", indent);
	}
}

void register_constructor(const ParameterType type, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectBehaviour(\"" << get_angelscript_string(type);
	std::cout << "\", asBEHAVE_CONSTRUCT," << std::endl << get_indent(indent);
	std::cout << "\t\t\"void f()\", asFUNCTION(constructor_0<" << get_string(type);
	std::cout << ">)," << std::endl << get_indent(indent);
	std::cout << "\t\tasCALL_CDECL_OBJLAST);" << std::endl << get_indent(indent);
	std::cout << "\tassert(r >= 0);" << std::endl;
}

void register_constructor(const ParameterType type, const ParameterType param_0,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectBehaviour(\"" << get_angelscript_string(type);
	std::cout << "\", asBEHAVE_CONSTRUCT," << std::endl << get_indent(indent);
	std::cout << "\t\t\"void f(const " << get_angelscript_string(param_0);
	std::cout << " &in)\", asFUNCTION((constructor_1<" << get_string(type);
	std::cout << ", " << get_string(param_0) << ">))," << std::endl << get_indent(indent);
	std::cout << "\t\tasCALL_CDECL_OBJLAST);" << std::endl << get_indent(indent);
	std::cout << "\tassert(r >= 0);" << std::endl;
}

void register_constructor(const ParameterType type, const ParameterType param_0,
	const ParameterType param_1, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectBehaviour(\"" << get_angelscript_string(type);
	std::cout << "\", asBEHAVE_CONSTRUCT," << std::endl << get_indent(indent);
	std::cout << "\t\t\"void f(const " << get_angelscript_string(param_0);
	std::cout << " &in, const " << get_angelscript_string(param_1);
	std::cout << " &in)\"," << std::endl << get_indent(indent);
	std::cout << "\t\tasFUNCTION((constructor_2<" << get_string(type);
	std::cout << ", " << get_string(param_0) << ", " << get_string(param_1);
	std::cout << ">))," << std::endl << get_indent(indent);
	std::cout << "\t\tasCALL_CDECL_OBJLAST);" << std::endl << get_indent(indent);
	std::cout << "\tassert(r >= 0);" << std::endl;
}

void register_constructor(const ParameterType type, const ParameterType param_0,
	const ParameterType param_1, const ParameterType param_2,
	const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectBehaviour(\"" << get_angelscript_string(type);
	std::cout << "\", asBEHAVE_CONSTRUCT," << std::endl << get_indent(indent);
	std::cout << "\t\t\"void f(const " << get_angelscript_string(param_0);
	std::cout << " &in, const " << get_angelscript_string(param_1);
	std::cout << " &in, const " << get_angelscript_string(param_2);
	std::cout << " &in)\"," << std::endl << get_indent(indent);
	std::cout << "\t\tasFUNCTION((constructor_3<" << get_string(type);
	std::cout << ", " << get_string(param_0) << ", " << get_string(param_1);
	std::cout << ", " << get_string(param_2) << ">))," << std::endl << get_indent(indent);
	std::cout << "\t\tasCALL_CDECL_OBJLAST);" << std::endl << get_indent(indent);
	std::cout << "\tassert(r >= 0);" << std::endl;
}

void register_constructor(const ParameterType type, const ParameterType param_0,
	const ParameterType param_1, const ParameterType param_2,
	const ParameterType param_3, const int indent)
{
	std::cout << get_indent(indent) << "\tr = engine->RegisterObjectBehaviour(\"" << get_angelscript_string(type);
	std::cout << "\", asBEHAVE_CONSTRUCT," << std::endl << get_indent(indent);
	std::cout << "\t\t\"void f(const " << get_angelscript_string(param_0);
	std::cout << " &in, const " << get_angelscript_string(param_1);
	std::cout << " &in, const " << get_angelscript_string(param_2);
	std::cout << " &in, const " << get_angelscript_string(param_3);
	std::cout << " &in)\"," << std::endl << get_indent(indent);
	std::cout << "\t\tasFUNCTION((constructor_4<" << get_string(type);
	std::cout << ", " << get_string(param_0) << ", " << get_string(param_1);
	std::cout << ", " << get_string(param_2) << ", " << get_string(param_3);
	std::cout << ">))," << std::endl << get_indent(indent);
	std::cout << "\t\tasCALL_CDECL_OBJLAST);" << std::endl << get_indent(indent);
	std::cout << "\tassert(r >= 0);" << std::endl;
}

void register_vector_constructors(const ParameterType type,
	const ParameterType param_0, const ParameterType param_1,
	const ParameterType param_2, const int indent)
{
	ValueType value;
	unsigned int count, n;

	value = get_value(type);
	count = get_count(type);

	n = count - (get_count(param_0) + get_count(param_1) + get_count(param_2));

	if (n == 1)
	{
		register_constructor(type, param_0, param_1, param_2, get_parameter(value, 1), indent);
		return;
	}

	register_constructor(type, param_0, param_1, param_2, get_parameter(vt_int, n), indent);
	register_constructor(type, param_0, param_1, param_2, get_parameter(vt_uint, n), indent);
	register_constructor(type, param_0, param_1, param_2, get_parameter(vt_bool, n), indent);
	register_constructor(type, param_0, param_1, param_2, get_parameter(vt_float, n), indent);
}

void register_vector_constructors(const ParameterType type, const ParameterType param_0,
	const ParameterType param_1, const int indent)
{
	ValueType value;
	unsigned int i, count, n;

	value = get_value(type);
	count = get_count(type);

	n = count - (get_count(param_0) + get_count(param_1));

	if (n == 1)
	{
		register_constructor(type, param_0, param_1, get_parameter(value, 1), indent);
		return;
	}

	register_constructor(type, param_0, param_1, get_parameter(vt_int, n), indent);
	register_constructor(type, param_0, param_1, get_parameter(vt_uint, n), indent);
	register_constructor(type, param_0, param_1, get_parameter(vt_bool, n), indent);
	register_constructor(type, param_0, param_1, get_parameter(vt_float, n), indent);

	if (count < 4)
	{
		return;
	}

	register_vector_constructors(type, param_0, param_1, get_parameter(value, 1), indent);

	for (i = 2; i < n; ++i)
	{
		register_vector_constructors(type, param_0, param_1, get_parameter(vt_int, i), indent);
		register_vector_constructors(type, param_0, param_1, get_parameter(vt_uint, i), indent);
		register_vector_constructors(type, param_0, param_1, get_parameter(vt_bool, i), indent);
		register_vector_constructors(type, param_0, param_1, get_parameter(vt_float, i), indent);
	}
}

void register_vector_constructors(const ParameterType type,
	const ParameterType param_0, const int indent)
{
	ValueType value;
	unsigned int i, count, n;

	value = get_value(type);
	count = get_count(type);

	n = count - get_count(param_0);

	if (n == 1)
	{
		register_constructor(type, param_0, get_parameter(value, 1), indent);
		return;
	}

	register_constructor(type, param_0, get_parameter(vt_int, n), indent);
	register_constructor(type, param_0, get_parameter(vt_uint, n), indent);
	register_constructor(type, param_0, get_parameter(vt_bool, n), indent);
	register_constructor(type, param_0, get_parameter(vt_float, n), indent);

	if (count < 3)
	{
		return;
	}

	register_vector_constructors(type, param_0, get_parameter(value, 1), indent);

	for (i = 2; i < n; ++i)
	{
		register_vector_constructors(type, param_0, get_parameter(vt_int, i), indent);
		register_vector_constructors(type, param_0, get_parameter(vt_uint, i), indent);
		register_vector_constructors(type, param_0, get_parameter(vt_bool, i), indent);
		register_vector_constructors(type, param_0, get_parameter(vt_float, i), indent);
	}
}

void register_vector_constructors(const ParameterType type,
	const int indent)
{
	ValueType value;
	unsigned int i, count;

	register_constructor(type, indent);

	value = get_value(type);
	count = get_count(type);

	register_constructor(type, get_parameter(value, 1), indent);

	for (i = 2; i < 5; ++i)
	{
		if (i >= count)
		{
			register_constructor(type, get_parameter(vt_int, i), indent);
			register_constructor(type, get_parameter(vt_uint, i), indent);
			register_constructor(type, get_parameter(vt_bool, i), indent);
			register_constructor(type, get_parameter(vt_float, i), indent);
		}
	}

	register_vector_constructors(type, get_parameter(value, 1), indent);

	for (i = 2; i < count; ++i)
	{
		register_vector_constructors(type, get_parameter(vt_int, i), indent);
		register_vector_constructors(type, get_parameter(vt_uint, i), indent);
		register_vector_constructors(type, get_parameter(vt_bool, i), indent);
		register_vector_constructors(type, get_parameter(vt_float, i), indent);
	}
}

typedef std::set<UnaryOperatorType> UnaryOperatorSet;
typedef std::set<BinaryOperatorType> BinaryOperatorSet;

void register_vec(const UnaryOperatorSet &uos, const BinaryOperatorSet &bos,
	const ParameterType type, const int indent)
{
	UnaryOperatorSet::const_iterator uos_it, uos_end;
	BinaryOperatorSet::const_iterator bos_it, bos_end;
	ParameterType value;

	std::cout << get_indent(indent) << "\t/* " << get_string(type) << " */" << std::endl;

	value = get_type(type);

	register_vector_element(type, indent);
	register_index_operator(type, value, indent);
	register_vector_constructors(type, indent);

	uos_end = uos.end();

	for (uos_it = uos.begin(); uos_it != uos_end; ++uos_it)
	{
		register_operator(type, type, *uos_it, indent);
		register_operator(type, value, *uos_it, indent);
	}

	bos_end = bos.end();

	for (bos_it = bos.begin(); bos_it != bos_end; ++bos_it)
	{
		register_operator(type, type, *bos_it, false, "glm::detail",
			indent);
		register_operator(type, value, *bos_it, true, "glm::detail",
			indent);
		register_operator(type, value, *bos_it, false, "glm::detail",
			indent);
	}

	register_to_string_function(type, indent);
}

void register_vecs(const UnaryOperatorSet &uos, const BinaryOperatorSet &bos,
	const ValueType type, const int indent)
{
	register_vec(uos, bos, get_parameter(type, 2), indent);
	register_vec(uos, bos, get_parameter(type, 3), indent);
	register_vec(uos, bos, get_parameter(type, 4), indent);
}

void register_matrix_constructors(const ParameterType type,
	const int indent)
{
	ValueType value_type;
	unsigned int i, j, row, col;

	register_constructor(type, indent);

	get_matrix_counts(type, row, col);
	value_type = get_value(type);

	register_constructor(type, get_parameter(value_type, 1), indent);

	for (i = 2; i < 5; ++i)
	{
		for (j = 2; j < 5; ++j)
		{
			register_constructor(type, get_matrix(i, j), indent);
		}
	}

	switch (row)
	{
		case 2:
			register_constructor(type,
				get_parameter(value_type, col),
				get_parameter(value_type, col), indent);
			break;
		case 3:
			register_constructor(type,
				get_parameter(value_type, col),
				get_parameter(value_type, col),
				get_parameter(value_type, col), indent);
			break;
		case 4:
			register_constructor(type,
				get_parameter(value_type, col),
				get_parameter(value_type, col),
				get_parameter(value_type, col),
				get_parameter(value_type, col), indent);
			break;
	}
}

void register_mat(const ParameterType type, const int indent)
{
	ValueType value_type;
	ParameterType value;
	unsigned int row, col, i;

	std::cout << get_indent(indent) << "\t/* " << get_string(type) << " */" << std::endl;

	register_matrix_constructors(type, indent);

	value = get_type(type);
	value_type = get_value(type);
	get_matrix_counts(type, row, col);

	register_index_operator(type, get_parameter(value_type, col), indent);

	register_operator(type, type, uot_add_assign, indent);
	register_operator(type, value, uot_add_assign, indent);
	register_operator(type, type, uot_sub_assign, indent);
	register_operator(type, value, uot_sub_assign, indent);
	register_operator(type, value, uot_mul_assign, indent);
	register_operator(type, value, uot_div_assign, indent);

	register_operator(type, type, bot_add, false, "glm::detail", indent);
	register_operator(type, value, bot_add, false, "glm::detail", indent);
	register_operator(type, type, bot_sub, false, "glm::detail", indent);
	register_operator(type, value, bot_sub, false, "glm::detail", indent);
	register_operator(type, value, bot_mul, false, "glm::detail", indent);
	register_operator(type, value, bot_div, false, "glm::detail", indent);

	register_function_2(type, type, type, "glm::matrixCompMult",
		"matrixCompMult", indent);
	register_function_1(get_matrix(col, row), type, "transpose",
		"transpose", indent);
	register_function_2(type, get_parameter(value_type, col),
		get_parameter(value_type, row), "glm::outerProduct<float>",
		"outerProduct", indent);

	if (row == col)
	{
		register_function_1(type, type, "glm::inverse", "inverse",
			indent);
		register_function_1(value, type, "glm::determinant",
			"determinant", indent);
		register_operator(type, type, uot_mul_assign, indent);
 	}

	for (i = 2; i < 5; ++i)
	{
		register_operator(type, get_matrix(i, col), get_matrix(i, row),
			bot_mul, false, "glm::detail", indent);
	}

	register_operator(type, get_parameter(value_type, col),
		get_parameter(value_type, row), bot_mul, false, "glm::detail",
		indent);
	register_operator(type, get_parameter(value_type, row),
		get_parameter(value_type, col), bot_mul, true, "glm::detail",
		indent);

	register_to_string_function(type, indent);
}

void register_mats(const int indent)
{
	unsigned int i, j;

	for (i = 2; i < 5; ++i)
	{
		for (j = 2; j < 5; ++j)
		{
			register_mat(get_matrix(i, j), indent);
		}
	}
}

void register_quaternion(const int indent)
{
	ParameterType value, type;

	type = pt_quat;

	std::cout << get_indent(indent) << "\t/* " << get_string(type) << " */" << std::endl;

	register_constructor(type, indent);
	register_constructor(type, pt_float, pt_vec3, indent);
	register_constructor(type, pt_float, pt_float, pt_float, pt_float, indent);

	register_quat_element(indent);

	value = get_type(type);

	register_index_operator(type, value, indent);

	register_operator(type, value, uot_mul_assign, indent);
	register_operator(type, value, uot_div_assign, indent);

	register_operator(type, type, bot_add, false, "glm::detail", indent);
	register_operator(type, type, bot_mul, false, "glm::detail", indent);

	register_function_1(type, type, "glm::detail::operator-", "opNeg",
		indent);

	register_function_2(pt_vec3, type, pt_vec3, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(pt_vec3, pt_vec3, type, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(pt_vec4, type, pt_vec4, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(pt_vec4, pt_vec4, type, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(type, type, pt_float, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(type, pt_float, type, "glm::detail::operator*",
		"opMul", indent);
	register_function_2(type, type, pt_float, "glm::detail::operator/",
		"opDiv", indent);

	register_function_1(pt_float, type, "length", "length", indent);
	register_function_1(type, type, "glm::normalize", "normalize", indent);
	register_function_2(pt_float, type, type, "dot", "dot", indent);
	register_function_3(type, type, type, pt_float, "glm::mix", "mix",
		indent);
	register_function_1(type, type, "glm::conjugate", "conjugate", indent);
	register_function_1(type, type, "glm::inverse", "inverse", indent);
	register_function_3(type, type, pt_float, pt_vec3, "glm::rotate",
		"rotate", indent);

//	register_to_string_function(type);
}

void register_types(const int indent)
{
	unsigned int i, j;

	for (i = 2; i < 5; ++i)
	{
		register_type(get_parameter(vt_int, i), indent);
		register_type(get_parameter(vt_uint, i), indent);
		register_type(get_parameter(vt_bool, i), indent);
		register_type(get_parameter(vt_float, i), indent);
	}

	for (i = 2; i < 5; ++i)
	{
		for (j = 2; j < 5; ++j)
		{
			register_type(get_matrix(i, j), indent);
		}
	}

	register_type(pt_quat, indent);
}

int main(int argc, char **argv)
{
	UnaryOperatorSet uos;
	BinaryOperatorSet bos;

	std::cout << "#include \"scriptglm/scriptglm.h\"" << std::endl;
	std::cout << "#include <glm/glm.hpp>" << std::endl;
	std::cout << "#include <glm/gtx/color_space.hpp>" << std::endl;
	std::cout << "#include <glm/gtx/string_cast.hpp>" << std::endl;
	std::cout << "#include <glm/gtc/quaternion.hpp>" << std::endl;
	std::cout << "#include <new>" << std::endl;
	std::cout << std::endl;
	std::cout << "namespace" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T, typename U, unsigned int N>" << std::endl;
	std::cout << "\tinline U *get_value_at(unsigned int i, T &obj)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tif (i >= N)" << std::endl;
	std::cout << "\t\t{" << std::endl;
	std::cout << "\t\t\tasIScriptContext *ctx = asGetActiveContext();" << std::endl;
	std::cout << "\t\t\tctx->SetException(\"Out of range\");" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t\treturn 0;" << std::endl;
	std::cout << "\t\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\treturn &obj[i];" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T>" << std::endl;
	std::cout << "\tinline void constructor_0(T *self)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tnew(self) T();" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T, typename P0>" << std::endl;
	std::cout << "\tinline void constructor_1(const P0 &p0, T *self)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tnew(self) T(p0);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T, typename P0, typename P1>" << std::endl;
	std::cout << "\tinline void constructor_2(const P0 &p0, const P1 &p1, T *self)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tnew(self) T(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T, typename P0, typename P1, typename P2>" << std::endl;
	std::cout << "\tinline void constructor_3(const P0 &p0, const P1 &p1, const P2 &p2, T *self)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tnew(self) T(p0, p1, p2);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "\ttemplate<typename T, typename P0, typename P1, typename P2, typename P3>" << std::endl;
	std::cout << "\tinline void constructor_4(const P0 &p0, const P1 &p1, const P2 &p2, const P3 &p3," << std::endl;
	std::cout << "\t\tT *self)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tnew(self) T(p0, p1, p2, p3);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float dot(const float &p0, const float &p1)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::dot(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float dot(const glm::vec2 &p0, const glm::vec2 &p1)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::dot(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float dot(const glm::vec3 &p0, const glm::vec3 &p1)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::dot(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float dot(const glm::vec4 &p0, const glm::vec4 &p1)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::dot(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float dot(const glm::quat &p0, const glm::quat &p1)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::dot(p0, p1);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline float length(const glm::quat &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::length(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat2x2 transpose(const glm::mat2x2 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat3x2 transpose(const glm::mat2x3 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat4x2 transpose(const glm::mat2x4 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat2x3 transpose(const glm::mat3x2 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat3x3 transpose(const glm::mat3x3 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat4x3 transpose(const glm::mat3x4 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat2x4 transpose(const glm::mat4x2 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat3x4 transpose(const glm::mat4x3 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tinline glm::mat4x4 transpose(const glm::mat4x4 &p)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\treturn glm::transpose(p);" << std::endl;
	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	uos.insert(uot_add_assign);
	uos.insert(uot_sub_assign);
	uos.insert(uot_mul_assign);
	uos.insert(uot_div_assign);

	bos.insert(bot_add);
	bos.insert(bot_sub);
	bos.insert(bot_mul);
	bos.insert(bot_div);

	std::cout << "\tvoid RegisterGlmFloatVecs(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_vecs(uos, bos, vt_float, 1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	bos.insert(bot_shl);
	bos.insert(bot_shr);
	uos.insert(uot_shl_assign);
	uos.insert(uot_shr_assign);

	std::cout << "\tvoid RegisterGlmIntVecs(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_vecs(uos, bos, vt_int, 1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tvoid RegisterGlmUintVecs(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_vecs(uos, bos, vt_uint, 1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	uos.clear();
	bos.clear();

	uos.insert(uot_and_assign);
	uos.insert(uot_or_assign);
	uos.insert(uot_xor_assign);
	bos.insert(bot_and);
	bos.insert(bot_or);
	bos.insert(bot_xor);

	std::cout << "\tvoid RegisterGlmBoolVecs(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_vecs(uos, bos, vt_bool, 1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tvoid RegisterGlmVecFuncs(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_vector_float_functions(1);
	register_vector_compare_functions(1);
	register_vector_functions(1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tvoid RegisterGlmMats(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_mats(1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;

	std::cout << "\tvoid RegisterGlmQuat(asIScriptEngine* engine)" << std::endl;
	std::cout << "\t{" << std::endl;
	std::cout << "\t\tint r;" << std::endl;
	std::cout << std::endl;

	register_quaternion(1);

	std::cout << "\t}" << std::endl;
	std::cout << std::endl;
	std::cout << "}" << std::endl;
	std::cout << std::endl;

	std::cout << "void RegisterGlm(asIScriptEngine* engine)" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "\tint r;" << std::endl;
	std::cout << std::endl;

	register_types(0);

	std::cout << std::endl;

	std::cout << "\tRegisterGlmFloatVecs(engine);" << std::endl;
	std::cout << "\tRegisterGlmIntVecs(engine);" << std::endl;
	std::cout << "\tRegisterGlmUintVecs(engine);" << std::endl;
	std::cout << "\tRegisterGlmBoolVecs(engine);" << std::endl;
	std::cout << "\tRegisterGlmVecFuncs(engine);" << std::endl;
	std::cout << "\tRegisterGlmMats(engine);" << std::endl;
	std::cout << "\tRegisterGlmQuat(engine);" << std::endl;
	std::cout << "}" << std::endl;
}
