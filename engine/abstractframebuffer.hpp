/****************************************************************************
 *            abstractframebuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0
#define	UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "framebufferattachmentutil.hpp"

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
			typedef std::map<FrameBufferAttachmentType,
				TextureSharedPtr>
				FrameBufferAttachmentTypeTextureSharedPtrMap;
			const String m_name;
			FrameBufferAttachmentTypeTextureSharedPtrMap m_textures;
			const Uint32 m_width;
			const Uint32 m_height;
			BitSet16 m_attachments;

			virtual void detach_texture(
				const FrameBufferAttachmentType attachment) = 0;
			virtual void attach_texture(
				const TextureSharedPtr &texture,
				const FrameBufferAttachmentType attachment,
				const Uint16 layer, const Uint16 mipmap) = 0;
			virtual void do_attach_depth_render_buffer(
				bool &depth, bool &stencil) = 0;

		protected:
			AbstractFrameBuffer(const String &name,
				const Uint32 width, const Uint32 height);

		public:
			virtual ~AbstractFrameBuffer() noexcept;
			virtual void bind() = 0;
			virtual void clear(const float depth,
				const GLint stencil) = 0;
			virtual void clear(const glm::vec4 &color,
				const Uint16 index) = 0;
			virtual void unbind() = 0;
			virtual void blit_to_back_buffer(const glm::uvec4 &rect,
				const Uint16 layer, const bool color,
				const bool depth, const bool stencil) = 0;
			void attach_depth_render_buffer();
			void attach(const TextureSharedPtr &texture,
				const FrameBufferAttachmentType attachment,
				const Uint16 layer, const Uint16 mipmap);
			bool detach(const TextureSharedPtr &texture);
			bool detach(
				const FrameBufferAttachmentType attachment);
			bool get_is_attached(const TextureSharedPtr &texture)
				const;
			bool get_has_texture_attachment(
				const FrameBufferAttachmentType attachment)
				const;
			const TextureSharedPtr &get_texture(
				const FrameBufferAttachmentType attachment)
				const;
			virtual void set_draw_buffers(
				const glm::bvec4 &enabled) = 0;
			virtual void blit_buffers() = 0;

			inline bool get_has_attachment(
				const FrameBufferAttachmentType attachment)
				const noexcept
			{
				return m_attachments[attachment];
			}

			inline glm::uvec4 get_view_port(const Uint16 mipmap)
				noexcept
			{
				return glm::uvec4(0, 0, get_width(mipmap),
					get_height(mipmap));
			}

			inline Uint32 get_width(const Uint16 mipmap) const
				noexcept
			{
				return std::max(1u, m_width >> mipmap);
			}

			inline Uint32 get_height(const Uint16 mipmap) const
				noexcept
			{
				return std::max(1u, m_height >> mipmap);
			}

			inline bool get_depth() const noexcept
			{
				return m_attachments[fbat_depth];
			}

			inline bool get_stencil() const noexcept
			{
				return m_attachments[fbat_stencil];
			}

			inline bool get_color() const noexcept
			{
				BitSet16 result;

				result = m_attachments;

				result[fbat_depth] = false;
				result[fbat_stencil] = false;

				return result.any();
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_6e639bfd_c4c1_4b90_b034_c6f3884ef2e0 */
