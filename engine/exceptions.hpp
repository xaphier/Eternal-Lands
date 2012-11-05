/****************************************************************************
 *            exceptions.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2895e87c_d039_42e6_8628_577d13de5d2a
#define	UUID_2895e87c_d039_42e6_8628_577d13de5d2a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class IoErrorException: public virtual std::exception,
		public virtual boost::exception {};
	class FileNotFoundException: public virtual IoErrorException {};
	class ReadErrorException: public virtual IoErrorException {};
	class InvalidParameterException: public virtual std::exception,
		public virtual boost::exception {};
	class ItemNotFoundException:
		public virtual InvalidParameterException {};
	class DuplicateItemException:
		public virtual InvalidParameterException {};
	class BufferTooSmallException:
		public virtual InvalidParameterException {};
	class NullPtrException: public virtual InvalidParameterException {};
	class InternalErrorException: public virtual std::exception,
		public virtual boost::exception {};
	class EnumNotInSwitchException:
		public virtual InternalErrorException {};
	class NotImplementedException:
		public virtual InternalErrorException {};
	class UnkownFormatException: public virtual InvalidParameterException,
		public virtual IoErrorException,
		public virtual NotImplementedException {};
	class BadCastException: public virtual std::bad_cast,
		public virtual boost::exception {};
	class RangeErrorException: public virtual InvalidParameterException {};
	class ArraySizeErrorException: public virtual RangeErrorException {};
	class SizeErrorException: public virtual RangeErrorException {};
	class InvalidTokenException:
		public virtual InvalidParameterException {};
	class OpenGlException: public virtual std::exception,
		public virtual boost::exception {};
	class ASException: public virtual std::exception,
		public virtual boost::exception {};
	class VariantException: public virtual std::exception,
		public virtual boost::exception {};
	class DecompressException: public virtual std::exception,
		public virtual boost::exception {};
	class CompressException: public virtual std::exception,
		public virtual boost::exception {};
	class BadAllocException: public virtual std::bad_alloc,
		public virtual boost::exception {};

	typedef boost::error_info<struct errinfo_parameter_name_, std::string>
		errinfo_parameter_name;
	typedef boost::error_info<struct errinfo_message_, std::string>
		errinfo_message;
	typedef boost::error_info<struct errinfo_file_position_, Uint64>
		errinfo_file_position;
	typedef boost::error_info<struct errinfo_value_, Sint64> errinfo_value;
	typedef boost::error_info<struct errinfo_expected_value_, Sint64>
		errinfo_expected_value;
	typedef boost::error_info<struct errinfo_size_, Uint64> errinfo_size;
	typedef boost::error_info<struct errinfo_item_id_, Uint32>
		errinfo_item_id;
	typedef boost::error_info<struct errinfo_item_name_, std::string>
		errinfo_item_name;
	typedef boost::error_info<struct errinfo_string_value_, std::string>
		errinfo_string_value;
	typedef boost::error_info<struct errinfo_expected_string_value_,
		std::string> errinfo_expected_string_value;
	typedef boost::error_info<struct errinfo_range_min_, Sint64>
		errinfo_range_min;
	typedef boost::error_info<struct errinfo_range_max_, Sint64>
		errinfo_range_max;
	typedef boost::error_info<struct errinfo_range_index_, Sint64>
		errinfo_range_index;
	typedef boost::error_info<struct errinfo_range_expected_, Sint64>
		errinfo_range_expected;
	typedef boost::error_info<struct errinfo_float_range_min_, float>
		errinfo_float_range_min;
	typedef boost::error_info<struct errinfo_float_range_max_, float>
		errinfo_float_range_max;
	typedef boost::error_info<struct errinfo_float_range_index_, float>
		errinfo_float_range_index;
	typedef boost::error_info<struct errinfo_size_, Uint64>
		errinfo_size;
	typedef boost::error_info<struct errinfo_array_index_0_, Uint64>
		errinfo_array_index_0;
	typedef boost::error_info<struct errinfo_array_index_1_, Uint64>
		errinfo_array_index_1;
	typedef boost::error_info<struct errinfo_array_index_2_, Uint64>
		errinfo_array_index_2;
	typedef boost::error_info<struct errinfo_array_size_0_, Uint64>
		errinfo_array_size_0;
	typedef boost::error_info<struct errinfo_array_size_1_, Uint64>
		errinfo_array_size_1;
	typedef boost::error_info<struct errinfo_array_size_2_, Uint64>
		errinfo_array_size_2;
	typedef boost::error_info<struct errinfo_width_, Uint64>
		errinfo_width;
	typedef boost::error_info<struct errinfo_height_, Uint64>
		errinfo_height;
	typedef boost::error_info<struct errinfo_depth_, Uint64>
		errinfo_depth;
	typedef boost::error_info<struct errinfo_expected_width_, Uint64>
		errinfo_expected_width;
	typedef boost::error_info<struct errinfo_expected_height_, Uint64>
		errinfo_expected_height;
	typedef boost::error_info<struct errinfo_expected_depth_, Uint64>
		errinfo_expected_depth;
	typedef boost::error_info<struct errinfo_name_, std::string>
		errinfo_name;
	typedef boost::error_info<struct errinfo_type_name_, std::string>
		errinfo_type_name;
	typedef boost::error_info<struct errinfo_any_type_name_, std::string>
		errinfo_variant_type_name;
	typedef boost::error_info<struct errinfo_vertex_shader_, std::string>
		errinfo_vertex_shader_source;
	typedef boost::error_info<struct errinfo_tess_control_shader_,
		std::string> errinfo_tess_control_shader_source;
	typedef boost::error_info<struct errinfo_tess_evaluation_shader_,
		std::string> errinfo_tess_evaluation_shader_source;
	typedef boost::error_info<struct errinfo_geometry_shader_, std::string>
		errinfo_geometry_shader_source;
	typedef boost::error_info<struct errinfo_fragment_shader_, std::string>
		errinfo_fragment_shader_source;
	typedef boost::error_info<struct errinfo_object_name_, std::string>
		errinfo_object_name;
	typedef boost::error_info<struct errinfo_opengl_error_, GLint>
		errinfo_opengl_error;
	typedef boost::error_info<struct errinfo_stack_string_, std::string>
		errinfo_stack_string;

	String get_gl_error_string(const GLenum error);
	String get_stack_string();

#define EL_THROW_EXCEPTION(exception) throw exception	\
	<< boost::throw_function(BOOST_CURRENT_FUNCTION)	\
	<< boost::throw_file(__FILE__)	\
	<< boost::throw_line((int)__LINE__)	\
	<< errinfo_stack_string(get_stack_string())

#define	VALUE_NOT_IN_SWITCH(type, name)	\
	EL_THROW_EXCEPTION(BadCastException()	\
		<< errinfo_value(static_cast<Uint32>(type))	\
		<< boost::errinfo_type_info_name(name))

#define EL_THROW_MESSAGE_EXCEPTION(description, arguments, exception)	\
	do	\
	{	\
		BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		EL_THROW_EXCEPTION(exception	\
			<< errinfo_message(format_string.str()));	\
	}	\
	while (false)

#define CHECK_GL_ERROR()	\
	do	\
	{	\
		GLint error;	\
	\
		error = glGetError();	\
	\
		if (error != GL_NO_ERROR)	\
		{	\
			EL_THROW_EXCEPTION(OpenGlException()	\
				<< errinfo_message(get_gl_error_string(error))	\
				<< errinfo_opengl_error(error));	\
		}	\
	}	\
	while (false)

#define CHECK_GL_ERROR_NAME(name)	\
	do	\
	{	\
		GLint error;	\
	\
		error = glGetError();	\
	\
		if (error != GL_NO_ERROR)	\
		{	\
			EL_THROW_EXCEPTION(OpenGlException()	\
				<< errinfo_message(get_gl_error_string(error))	\
				<< errinfo_opengl_error(error)	\
				<< errinfo_name(name));	\
		}	\
	}	\
	while (false)

#ifndef	NDEBUG
#define DEBUG_CHECK_GL_ERROR() CHECK_GL_ERROR()
#define DEBUG_CHECK_GL_ERROR_NAME(name) CHECK_GL_ERROR_NAME(name)
#else
#define DEBUG_CHECK_GL_ERROR()	\
	do	\
	{	\
	}	\
	while (false)
#define DEBUG_CHECK_GL_ERROR_NAME(name)	\
	do	\
	{	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define RANGE_CECK_MAX(index, max, message)	\
	assert(((index) < (max)) && (message))
#else
#define RANGE_CECK_MAX(index, max, message)	\
	do	\
	{	\
		if ((index) >= (max))	\
		{	\
			EL_THROW_EXCEPTION(RangeErrorException()	\
				<< errinfo_message((message))	\
				<< errinfo_range_index((index))	\
				<< errinfo_range_max((max)));	\
		}	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define RANGE_CECK_MIN(index, min, message)	\
	assert(((index) >= (min)) && (message))
#else
#define RANGE_CECK_MIN(index, min, message)	\
	do	\
	{	\
		if ((index) < (min))	\
		{	\
			EL_THROW_EXCEPTION(RangeErrorException()	\
				<< errinfo_message((message))	\
				<< errinfo_range_index((index))	\
				<< errinfo_range_min((min)));	\
		}	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define RANGE_CECK(index, min, max, message)	\
	assert(((index) >= (min)) && (message));	\
	assert(((index) < (max)) && (message))
#else
#define RANGE_CECK(index, min, max, message)	\
	do	\
	{	\
		if (((index) < (min)) || ((index) >= (max)))	\
		{	\
			EL_THROW_EXCEPTION(RangeErrorException()	\
				<< errinfo_message((message))	\
				<< errinfo_range_index((index))	\
				<< errinfo_range_min((min))	\
				<< errinfo_range_max((max)));	\
		}	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define RANGE_CECK_EQUAL(index, expected, message)	\
	assert(((index) == (expected)) && (message))
#else
#define RANGE_CECK_EQUAL(index, expected, message)	\
	do	\
	{	\
		if ((index) != (expected))	\
		{	\
			EL_THROW_EXCEPTION(RangeErrorException()	\
				<< errinfo_message((message))	\
				<< errinfo_range_index((index))	\
				<< errinfo_range_expected((expected)));	\
		}	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define CECK_TABLE_SIZES_EQUAL(size, expected, message)	\
	assert(((size.x) == (expected.x)) &&	\
		((size.y) == (expected.y)) &&	\
		((size.z) == (expected.z)) && (message))
#else
#define CECK_TABLE_SIZES_EQUAL(size, expected, message)	\
	do	\
	{	\
		if (((size.x) != (expected.x)) ||	\
			((size.y) != (expected.y)) ||	\
			((size.z) != (expected.z)))	\
		{	\
			EL_THROW_EXCEPTION(SizeErrorException()	\
				<< errinfo_message((message))	\
				<< errinfo_width((size.x))	\
				<< errinfo_height((size.y))	\
				<< errinfo_depth((size.z))	\
				<< errinfo_expected_width((expected.x))	\
				<< errinfo_expected_height((expected.y))	\
				<< errinfo_expected_depth((expected.z)));	\
		}	\
	}	\
	while (false)
#endif

#ifndef	NDEBUG
#define NULL_PTR_CHECK(ptr, parameter)	\
	assert(((ptr) != nullptr) && (parameter));
#else
#define NULL_PTR_CHECK(ptr, parameter)	\
	do	\
	{	\
		if ((ptr) == nullptr)	\
		{	\
			EL_THROW_EXCEPTION(NullPtrException()	\
				<< errinfo_parameter_name((parameter)));	\
		}	\
	}	\
	while (false)
#endif

}

#endif	/* UUID_2895e87c_d039_42e6_8628_577d13de5d2a */
