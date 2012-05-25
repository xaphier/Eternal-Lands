/****************************************************************************
 *            hardwarebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ebe8ffbc_ae1a_4db5_bdd0_e78938bd6dfc
#define	UUID_ebe8ffbc_ae1a_4db5_bdd0_e78938bd6dfc

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class HardwareBuffer.
 * This file contains the @c class HardwareBuffer.
 */
namespace eternal_lands
{

	/**
	 * Defines how to access the hardware buffer.
	 * @{
	 */
	enum HardwareBufferAccessType
	{
		hbat_read_only = GL_READ_ONLY,
		hbat_read_write = GL_READ_WRITE,
		hbat_write_only = GL_WRITE_ONLY
	};
	/**
	 * @}
	 */

	/**
	 * Defines for what we use the hardware buffer. This is not static,
	 * so you can use render-to-vertex-buffer etc.
	 * @{
	 */
	enum BufferTargetType
	{
		btt_vertex = GL_ARRAY_BUFFER,
		btt_index = GL_ELEMENT_ARRAY_BUFFER,
		btt_pixel_pack = GL_PIXEL_PACK_BUFFER,
		btt_pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
		btt_uniform = GL_UNIFORM_BUFFER,
		btt_copy_read = GL_COPY_READ_BUFFER,
		btt_copy_write = GL_COPY_WRITE_BUFFER
	};
	/**
	 * @}
	 */

	/**
	 * Defines how to use the hardware buffer.
	 * @{
	 */
	enum HardwareBufferUsageType
	{
		hbut_dynamic_copy = GL_DYNAMIC_COPY,
		hbut_dynamic_draw = GL_DYNAMIC_DRAW,
		hbut_dynamic_read = GL_DYNAMIC_READ,
		hbut_static_copy = GL_STATIC_COPY,
		hbut_static_draw = GL_STATIC_DRAW,
		hbut_static_read = GL_STATIC_READ,
		hbut_stream_copy = GL_STREAM_COPY,
		hbut_stream_draw = GL_STREAM_DRAW,
		hbut_stream_read = GL_STREAM_READ
	};
	/**
	 * @}
	 */

	/**
	 * @brief @c class for opengl hardware buffers.
	 *
	 * @c class for opengl hardware buffers.
	 */
	class HardwareBuffer: public boost::noncopyable
	{
		private:
			/**
			 * This is the size of the buffer in bytes.
			 */
			Uint64 m_size;

			/**
			 * This is the OpenGL buffer id.
			 */
			GLuint m_id;

		public:
			/**
			 * Default constructor, creates the OpenGL id.
			 */
			HardwareBuffer();

			/**
			 * Default destructor, frees the memory and the
			 * OpenGL id.
			 */
			~HardwareBuffer() noexcept;

			/**
			 * Returns the size of the buffer in bytes.
			 */
			inline Uint64 get_size() const noexcept
			{
				return m_size;
			}

			/**
			 * Sets the size of the buffer in bytes.
			 * @param target The buffer target to use.
			 * @param size The new size to use.
			 * @param usage The usage of the buffer.
			 */
			void set_size(const BufferTargetType target,
				const Uint64 size,
				const HardwareBufferUsageType usage);

			void set(const BufferTargetType target,
				const AbstractReadMemory &buffer,
				const HardwareBufferUsageType usage);

			void update(const BufferTargetType target,
				const AbstractReadMemory &buffer,
				const Uint64 offset = 0);

			/**
			 * Binds the buffer.
			 * @param target The buffer target to use.
			 */
			void bind(const BufferTargetType target);

			/**
			 * Unbinds the currently bound buffer.
			 * @param target The buffer target to use.
			 */
			static void unbind(const BufferTargetType target);

			/**
			 * Binds the buffer.
			 * @param target The buffer target to use.
			 */
			void bind(const BufferTargetType target,
				const Uint32 index);

			/**
			 * Unbinds the currently bound buffer.
			 * @param target The buffer target to use.
			 */
			static void unbind(const BufferTargetType target,
				const Uint32 index);

			/**
			 * Map the buffer the currently bound buffer.
			 * @param target The buffer target to use.
			 * @param access The access type that is performed.
			 * @return Pointer of the buffer data.
			 */
			static void* map(const BufferTargetType target,
				const HardwareBufferAccessType access);

			/**
			 * Map the buffer the currently bound buffer.
			 * @param target The buffer target to use.
			 * @param offset The offset of the mapped range.
			 * @param size The size of the mapped range.
			 * @param read The access to the buffer is reading.
			 * @param write The access to the buffer is writing.
			 * @param invalidate_range The content of the mapped
			 * range of the buffer can be invalidated before
			 * mapping it.
			 * @return Pointer of the buffer data.
			 */
			void* map_range(const BufferTargetType target,
				const Uint32 offset, const Uint32 size,
				const bool read, const bool write,
				const bool invalidate_range);

			/**
			 * Map the buffer the currently bound buffer.
			 * @param target The buffer target to use.
			 * @param read The access to the buffer is reading.
			 * @param write The access to the buffer is writing.
			 * @param invalidate_buffer The content of the whole
			 * buffer can be invalidated before mapping it.
			 * @return Pointer of the buffer data.
			 */
			void* map(const BufferTargetType target,
				const bool read, const bool write,
				const bool invalidate_buffer);

			/**
			 * Unmap the buffer the currently bound buffer.
			 * @param target The buffer target to use.
			 * @return bool Returns true on success, else false.
			 */
			static bool unmap(const BufferTargetType target);

	};

}

#endif	/* UUID_ebe8ffbc_ae1a_4db5_bdd0_e78938bd6dfc */
