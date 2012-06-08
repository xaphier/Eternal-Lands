/****************************************************************************
 *            uniformdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8e5e11fe_fade_499e_9d58_743bb21b7ca5
#define	UUID_8e5e11fe_fade_499e_9d58_743bb21b7ca5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "autoparameterutil.hpp"

/**
 * @file
 * @brief The @c class UniformDescription.
 * This file contains the @c class UniformDescription.
 */
namespace eternal_lands
{

	class UniformDescription
	{
		private:
			AutoParameterType m_auto_parameter;
			Uint32 m_offset, m_size, m_array_stride;
			Uint32 m_matrix_stride;
			bool m_is_row_major;
			bool m_compact;

			void update_compact();

		public:
			UniformDescription();
			UniformDescription(
				const AutoParameterType auto_parameter);
			UniformDescription(
				const AutoParameterType auto_parameter,
				const Uint32 offset, const Uint32 size,
				const Uint32 array_stride,
				const Uint32 matrix_stride,
				const bool is_row_major);
			~UniformDescription() noexcept;
			void write(const String &indent, OutStream &str) const;
			void log(OutStream &str) const;

			inline Uint32 get_auto_parameter_max_size() const
			{
				return AutoParameterUtil::get_max_size(
					get_auto_parameter());
			}

			inline const String &get_name() const
			{
				return AutoParameterUtil::get_str(
					get_auto_parameter());
			}

			inline ParameterType get_type() const
			{
				return AutoParameterUtil::get_type(
					get_auto_parameter());
			}

			inline AutoParameterType get_auto_parameter() const
				noexcept
			{
				return m_auto_parameter;
			}

			inline Uint32 get_offset() const noexcept
			{
				return m_offset;
			}

			inline Uint32 get_size() const noexcept
			{
				return m_size;
			}

			inline Uint32 get_array_stride() const noexcept
			{
				return m_array_stride;
			}

			inline Uint32 get_matrix_stride() const noexcept
			{
				return m_matrix_stride;
			}

			inline bool get_is_row_major() const noexcept
			{
				return m_is_row_major;
			}

			inline bool get_compact() const noexcept
			{
				return m_compact;
			}

			inline void set_auto_parameter(
				const AutoParameterType auto_parameter) noexcept
			{
				m_auto_parameter = auto_parameter;
				update_compact();
			}

			inline void set_offset(const Uint32 offset) noexcept
			{
				m_offset = offset;
				update_compact();
			}

			inline void set_size(const Uint32 size) noexcept
			{
				m_size = size;
				update_compact();
			}

			inline void set_array_stride(const Uint32 array_stride)
				noexcept
			{
				m_array_stride = array_stride;
				update_compact();
			}

			inline void set_matrix_stride(
				const Uint32 matrix_stride) noexcept
			{
				m_matrix_stride = matrix_stride;
				update_compact();
			}

			inline void set_is_row_major(const bool is_row_major)
				noexcept
			{
				m_is_row_major = is_row_major;
				update_compact();
			}

	};

}

#endif	/* UUID_8e5e11fe_fade_499e_9d58_743bb21b7ca5 */
