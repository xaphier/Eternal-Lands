/****************************************************************************
 *            writer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_86cc4d92_7ef1_4462_b92a_f89f0f778f09
#define	UUID_86cc4d92_7ef1_4462_b92a_f89f0f778f09

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Writer.
 * This file contains the @c class Writer.
 */
namespace eternal_lands
{

	/**
	 * @brief This class writes data to a stream.
	 *
	 * This class writes data to a stream, supporting endian swapping.
	 */
	class Writer: public boost::enable_shared_from_this<Writer>
	{
		private:
			OutStreamSharedPtr m_stream;
			const String m_name;

		public:
			Writer(const OutStreamSharedPtr &stream,
				const String &name);
			Writer(const String &name);
			~Writer() noexcept;

			inline void write(const void* buffer, const Uint64 size)
			{
				m_stream->write(
					static_cast<const char*>(buffer), size);
			}

			void write(const AbstractReadMemory &buffer);
			void write_le(const AbstractReadMemory &buffer,
				const Uint16 swap_size);
			void write_be(const AbstractReadMemory &buffer,
				const Uint16 swap_size);

			/**
			 * Writes an utf-8 encoded string.
			 * @param str The string to write.
			 * @param size The number of chars to write.
			 */
			void write_utf8_string(const String &str,
				const Uint32 size);

			/**
			 * Writes an Uint8.
			 * @param value The Uint8 to write.
			 */
			void write_u8(const Uint8 value);

			/**
			 * Writes a Sint8.
			 * @param value The Sint8 to write.
			 */
			void write_s8(const Sint8 value);

			/**
			 * Writes an Uint16 swapped to little endian.
			 * @param value The Uint16 to write.
			 */
			void write_u16_le(const Uint16 value);

			/**
			 * Writes a Sint16 swapped to little endian.
			 * @param value The Sint16 to write.
			 */
			void write_s16_le(const Sint16 value);

			/**
			 * Writes an Uint16 swapped to big endian.
			 * @param value The Uint16 to write.
			 */
			void write_u16_be(const Uint16 value);

			/**
			 * Writes a Sint16 swapped to big endian.
			 * @param value The Sint16 to write.
			 */
			void write_s16_be(const Sint16 value);

			/**
			 * Writes an Uint32 swapped to little endian.
			 * @param value The Uint32 to write.
			 */
			void write_u32_le(const Uint32 value);

			/**
			 * Writes a Sint32 swapped to little endian.
			 * @param value The Sint32 to write.
			 */
			void write_s32_le(const Sint32 value);

			/**
			 * Writes an Uint32 swapped to big endian.
			 * @param value The Uint32 to write.
			 */
			void write_u32_be(const Uint32 value);

			/**
			 * Writes a Sint32 swapped to big endian.
			 * @param value The Sint32 to write.
			 */
			void write_s32_be(const Sint32 value);

			/**
			 * Writes an Uint64 swapped to little endian.
			 * @param value The Uint64 to write.
			 */
			void write_u64_le(const Uint64 value);

			/**
			 * Writes a Sint64 swapped to little endian.
			 * @param value The Sint64 to write.
			 */
			void write_s64_le(const Sint64 value);

			/**
			 * Writes an Uint64 swapped to big endian.
			 * @param value The Uint64 to write.
			 */
			void write_u64_be(const Uint64 value);

			/**
			 * Writes a Sint64 swapped to big endian.
			 * @param value The Sint64 to write.
			 */
			void write_s64_be(const Sint64 value);

			/**
			 * Writes a half float swapped to little endian.
			 * @param value The float to write as half float.
			 */
			void write_half_le(const float value);

			/**
			 * Writes a half float swapped to big endian.
			 * @param value The float to write as half float.
			 */
			void write_half_be(const float value);

			/**
			 * Writes a float swapped to little endian.
			 * @param value The float to write.
			 */
			void write_float_le(const float value);

			/**
			 * Writes a float swapped to big endian.
			 * @param value The float to write.
			 */
			void write_float_be(const float value);

			/**
			 * Writes a dynamic Uint64.
			 * @return The read Uint64.
			 */
			void write_dynamic_uint(const Uint64 value);

			/**
			 * Writes an utf-8 encoded string. The size is stored
			 * as dynamic uint.
			 * @param str The string to write.
			 */
			inline void write_dynamic_utf8_string(
				const String &str)
			{
				Uint64 size;

				size = str.get().size();

				write_dynamic_uint(size);
				write_utf8_string(str, size);
			}

			/**
			 * Returns the current position.
			 * @return The current position.
			 */
			inline Uint64 get_position() const
			{
				return m_stream->tellp();
			}

			/**
			 * Sets the current position.
			 * @param position The new current position.
			 */
			inline void set_position(const Uint64 position)
			{
				m_stream->seekp(position);
			}

			/**
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_86cc4d92_7ef1_4462_b92a_f89f0f778f09 */
