/****************************************************************************
 *            extendedexception.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2895e87c_d039_42e6_8628_577d13de5d2a
#define	UUID_2895e87c_d039_42e6_8628_577d13de5d2a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	class IoErrorException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class FileNotFoundException: public virtual IoErrorException {};
	class ReadErrorException: public virtual IoErrorException {};
	class InvalidParameterException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class ItemNotFoundException:
		public virtual InvalidParameterException {};
	class DuplicateItemException:
		public virtual InvalidParameterException {};
	class BufferTooSmallException:
		public virtual InvalidParameterException {};
	class NullPtrException: public virtual InvalidParameterException {};
	class InternalErrorException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class EnumNotInSwitchException:
		public virtual InternalErrorException {};
	class NotImplementedException:
		public virtual InternalErrorException {};
	class UnkownFormatException: public virtual InvalidParameterException,
		public virtual IoErrorException,
		public virtual NotImplementedException {};
	class BadCastException: public virtual ::std::bad_cast,
		public virtual ::boost::exception {};
	class RangeErrorException: public virtual InvalidParameterException {};
	class ArraySizeErrorException: public virtual RangeErrorException {};
	class InvalidTokenException:
		public virtual InvalidParameterException {};
	class OpenGlException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class LuaException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class LuaVarTypeException: public virtual LuaException {};
	class LuaTableSizeException: public virtual LuaException {};
	class VariantException: public virtual ::std::exception,
		public virtual ::boost::exception {};
	class UncompressException: public virtual ::std::exception,
		public virtual ::boost::exception {};

	typedef boost::error_info<struct errinfo_parameter_name_, StringType>
		errinfo_parameter_name;
	typedef boost::error_info<struct errinfo_message_, StringType>
		errinfo_message;
	typedef boost::error_info<struct errinfo_file_position_, Uint64>
		errinfo_file_position;
	typedef boost::error_info<struct errinfo_value_, Uint32> errinfo_value;
	typedef boost::error_info<struct errinfo_expected_value_, Uint32>
		errinfo_expected_value;
	typedef boost::error_info<struct errinfo_size_, Uint64> errinfo_size;
	typedef boost::error_info<struct errinfo_item_id_, Uint32>
		errinfo_item_id;
	typedef boost::error_info<struct errinfo_item_name_, StringType>
		errinfo_item_name;
	typedef boost::error_info<struct errinfo_string_value_, StringType>
		errinfo_string_value;
	typedef boost::error_info<struct errinfo_expected_string_value_,
		StringType> errinfo_expected_string_value;
	typedef boost::error_info<struct errinfo_range_min_, Sint64>
		errinfo_range_min;
	typedef boost::error_info<struct errinfo_range_max_, Sint64>
		errinfo_range_max;
	typedef boost::error_info<struct errinfo_range_index_, Sint64>
		errinfo_range_index;
	typedef boost::error_info<struct errinfo_float_range_min_, float>
		errinfo_float_range_min;
	typedef boost::error_info<struct errinfo_float_range_max_, float>
		errinfo_float_range_max;
	typedef boost::error_info<struct errinfo_float_range_index_, float>
		errinfo_float_range_index;
	typedef boost::error_info<struct errinfo_table_size_, Uint64>
		errinfo_table_size;
	typedef boost::error_info<struct errinfo_table_index_, Uint64>
		errinfo_table_index;
	typedef boost::error_info<struct errinfo_array_index_, Uint64>
		errinfo_array_index;
	typedef boost::error_info<struct errinfo_lua_type_, Sint32>
		errinfo_lua_type;
	typedef boost::error_info<struct errinfo_lua_type_name_, StringType>
		errinfo_lua_type_name;
	typedef boost::error_info<struct errinfo_name_, StringType>
		errinfo_name;
	typedef boost::error_info<struct errinfo_type_name_, StringType>
		errinfo_type_name;
	typedef boost::error_info<struct errinfo_any_type_name_, StringType>
		errinfo_variant_type_name;
	typedef boost::error_info<struct errinfo_gl_error_, Uint64>
		errinfo_gl_error;
	typedef boost::error_info<struct errinfo_vertex_shader_, StringType>
		errinfo_vertex_shader_source;
	typedef boost::error_info<struct errinfo_fragment_shader_, StringType>
		errinfo_fragment_shader_source;

#define EL_THROW_EXCEPTION(exception)	\
	throw exception <<\
	::boost::throw_function(BOOST_CURRENT_FUNCTION) <<\
	::boost::throw_file(__FILE__) <<\
	::boost::throw_line((int)__LINE__)

#define	VALUE_NOT_IN_SWITCH(type, name)	\
	EL_THROW_EXCEPTION(::eternal_lands::BadCastException()	\
		<< ::eternal_lands::errinfo_value(static_cast<Uint32>(type))	\
		<< ::boost::errinfo_type_info_name(name))

#define EL_THROW_MESSAGE_EXCEPTION(description, arguments, exception)	\
	do	\
	{	\
		BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		throw exception <<\
			::boost::throw_function(BOOST_CURRENT_FUNCTION) <<\
			::boost::throw_file(__FILE__) <<\
			::boost::throw_line((int)__LINE__)	<<\
			::eternal_lands::errinfo_message(format_string.str());	\
	}	\
	while (false)


#define CHECK_GL_ERROR()	\
	do	\
	{	\
		GLint gl_error;	\
	\
		gl_error = glGetError();	\
	\
		if (gl_error != GL_NO_ERROR)	\
		{	\
			EL_THROW_EXCEPTION(::eternal_lands::OpenGlException()	\
				<< ::eternal_lands::errinfo_message(	\
					::eternal_lands::string_to_utf8(	\
						reinterpret_cast<const char*>(	\
							gluErrorString(gl_error))))	\
				<< ::eternal_lands::errinfo_gl_error(gl_error));	\
		}	\
	}	\
	while (false)

#ifndef	NDEBUG
#define DEBUG_CHECK_GL_ERROR() CHECK_GL_ERROR()
#else
#define DEBUG_CHECK_GL_ERROR()	\
	do	\
	{	\
	}	\
	while (false)
#endif

}

#endif	/* UUID_2895e87c_d039_42e6_8628_577d13de5d2a */
