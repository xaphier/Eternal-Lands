/****************************************************************************
 *            abstractframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0
#define	UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractFrameBuffer.
 * This file contains the @c class AbstractFrameBuffer.
 */
namespace eternal_lands
{

	class AbstractFrameBuffer: public boost::noncopyable
	{
		private:
			TextureSharedPtr m_texture;
			const Uint32 m_width;
			const Uint32 m_height;
			const Uint32 m_depth;
			bool m_depth_buffer;
			bool m_stencil_buffer;
			bool m_color_buffer;

		protected:
			AbstractFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height,
				const Uint32 depth);

			inline void set_depth_buffer(const bool depth_buffer)
			{
				m_depth_buffer = depth_buffer;
			}

			inline void set_stencil_buffer(
				const bool stencil_buffer)
			{
				m_stencil_buffer = stencil_buffer;
			}

			inline void set_color_buffer(const bool color_buffer)
			{
				m_color_buffer = color_buffer;
			}

		public:
			virtual ~AbstractFrameBuffer() throw();
			virtual void bind(const Uint32 layer) = 0;
			virtual void bind_texture(const Uint32 layer) = 0;
			virtual void blit() = 0;
			virtual void clear(const glm::vec4 &color,
				const float depth) = 0;
			virtual void clear(const glm::vec4 &color) = 0;
			virtual void unbind() = 0;

			inline const TextureSharedPtr &get_texture() const
			{
				return m_texture;
			}

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline Uint32 get_depth() const
			{
				return m_depth;
			}

			inline bool get_depth_buffer() const
			{
				return m_depth_buffer;
			}

			inline bool get_stencil_buffer() const
			{
				return m_stencil_buffer;
			}

			inline bool get_color_buffer() const
			{
				return m_color_buffer;
			}

	};

}

#endif	/* UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0 */
