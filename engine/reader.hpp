/****************************************************************************
 *            reader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_62cf1278_7951_42f0_a25b_1b688c51351a
#define	UUID_62cf1278_7951_42f0_a25b_1b688c51351a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Reader.
 * This file contains the @c class Reader.
 */
namespace eternal_lands
{

	/**
	 * @brief This class reads data from a buffer.
	 *
	 * This class reads data from a memory buffer, supporting endian
	 * swapping.
	 */
	class Reader: public boost::enable_shared_from_this<Reader>
	{
		private:
			AbstractReadMemoryBufferSharedPtr m_buffer;
			String m_name;
			Uint64 m_position;

		public:
			Reader(const AbstractReadMemoryBufferSharedPtr &buffer,
				const String &name);
			Reader(const Reader &reader);
			~Reader() throw();

			void read(void* buffer, const Uint64 size);
			void read(MemoryBuffer &buffer);
			void read_le(MemoryBuffer &buffer,
				const Uint16 swap_size);
			void read_be(MemoryBuffer &buffer,
				const Uint16 swap_size);

			/**
			 * Returns the total size of the reader source buffer.
			 * @return The total size of the reader source buffer.
			 */
			Uint64 get_size() const;

			/**
			 * Reads an utf-8 encoded string and returns it.
			 * @return The read string.
			 */
			String read_utf8_string(const Uint32 size);

			/**
			 * Reads an Uint8 and returns it.
			 * @return The read Uint8.
			 */
			Uint8 read_u8();

			/**
			 * Reads a Sint8 and returns it.
			 * @return The read Sint8.
			 */
			Sint8 read_s8();

			/**
			 * Reads an Uint16, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Uint16.
			 */
			Uint16 read_u16_le();

			/**
			 * Reads an Uint16, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Uint16.
			 */
			Uint16 read_u16_be();

			/**
			 * Reads a Sint16, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Sint16.
			 */
			Sint16 read_s16_le();

			/**
			 * Reads a Sint16, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Sint16.
			 */
			Sint16 read_s16_be();

			/**
			 * Reads an Uint32, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Uint32.
			 */
			Uint32 read_u32_le();

			/**
			 * Reads an Uint32, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Uint32.
			 */
			Uint32 read_u32_be();

			/**
			 * Reads a Sint32, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Sint32.
			 */
			Sint32 read_s32_le();

			/**
			 * Reads a Sint32, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Sint32.
			 */
			Sint32 read_s32_be();

			/**
			 * Reads an Uint64, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Uint64.
			 */
			Uint64 read_u64_le();

			/**
			 * Reads an Uint64, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Uint64.
			 */
			Uint64 read_u64_be();

			/**
			 * Reads a Sint64, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read Sint64.
			 */
			Sint64 read_s64_le();

			/**
			 * Reads a Sint64, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read Sint64.
			 */
			Sint64 read_s64_be();

			/**
			 * Reads a float, swap it to to current endian from
			 * little endian and returns it.
			 * @return The read float.
			 */
			float read_float_le();

			/**
			 * Reads an float, swap it to to current endian from
			 * big endian and returns it.
			 * @return The read float.
			 */
			float read_float_be();

			/**
			 */
			inline Uint64 get_bytes_left() const
			{
				assert(get_position() <= get_size());

				return get_size() - get_position();
			}

			/**
			 */
			inline bool check_size(const Uint64 size) const
			{
				return size <= get_bytes_left();
			}

			/**
			 * Returns the current position.
			 * @return The current position.
			 */
			inline Uint64 get_position() const
			{
				return m_position;
			}

			/**
			 * Sets the current position.
			 * @param position The new current position.
			 */
			inline void set_position(const Uint64 position)
			{
				assert(position <= get_size());

				m_position = position;
			}

			/**
			 * Skip bytes.
			 * @param count The number of bytes to skip.
			 */
			inline void skip(const Uint64 count)
			{
				set_position(get_position() + count);
			}

			/**
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_62cf1278_7951_42f0_a25b_1b688c51351a */

