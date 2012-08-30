/****************************************************************************
 *            abstractframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractframebuffer.hpp"
#include "texture.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	AbstractFrameBuffer::AbstractFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height):
		m_name(name), m_width(width), m_height(height)
	{
	}

	AbstractFrameBuffer::~AbstractFrameBuffer() noexcept
	{
	}

	void AbstractFrameBuffer::attach(const TextureSharedPtr &texture,
		const FrameBufferAttachmentType attachment, const Uint16 layer)
	{
		FrameBufferAttachmentTypeTextureSharedPtrMap::iterator found;

		found = m_textures.find(attachment);

		if (found != m_textures.end())
		{
			if (found->second == texture)
			{
//				return;
			}
		}

		m_textures[attachment] = texture;

		attach_texture(texture, attachment, layer);

		m_attachments[attachment] = true;
	}

	bool AbstractFrameBuffer::detach(const TextureSharedPtr &texture)
	{
		FrameBufferAttachmentTypeTextureSharedPtrMap::iterator it, end;
		bool result;

		end = m_textures.end();
		result = false;

		for (it = m_textures.begin(); it != end; ++it)
		{
			if (texture == it->second)
			{
				detach_texture(it->first);

				m_attachments[it->first] = false;

				result = true;
			}
		}

		return result;
	}

	bool AbstractFrameBuffer::detach(
		const FrameBufferAttachmentType attachment)
	{
		FrameBufferAttachmentTypeTextureSharedPtrMap::iterator found;

		found = m_textures.find(attachment);

		if (found == m_textures.end())
		{
			return false;
		}

		detach_texture(attachment);
		m_attachments[attachment] = false;

		m_textures.erase(found);

		return true;
	}

	bool AbstractFrameBuffer::get_is_attached(
		const TextureSharedPtr &texture) const
	{
		FrameBufferAttachmentTypeTextureSharedPtrMap::const_iterator
			it, end;

		end = m_textures.end();

		for (it = m_textures.begin(); it != end; ++it)
		{
			if (texture == it->second)
			{
				return true;
			}
		}

		return false;
	}

	bool AbstractFrameBuffer::get_has_texture_attachment(
		const FrameBufferAttachmentType attachment) const
	{
		return m_textures.find(attachment) != m_textures.end();
	}

	const TextureSharedPtr &AbstractFrameBuffer::get_texture(
		const FrameBufferAttachmentType attachment) const
	{
		FrameBufferAttachmentTypeTextureSharedPtrMap::const_iterator
			found;

		found = m_textures.find(attachment);

		if (found == m_textures.end())
		{
			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_message(UTF8("Attachment not found"))
				<< errinfo_item_name(
					FrameBufferAttachmentUtil::get_str(
						attachment)));
		}

		return found->second;
	}

	void AbstractFrameBuffer::attach_depth_render_buffer()
	{
		bool depth, stencil;

		depth = false;
		stencil = false;

		do_attach_depth_render_buffer(depth, stencil);

		if (depth)
		{
			m_attachments[fbat_depth] = true;
		}

		if (stencil)
		{
			m_attachments[fbat_stencil] = true;
		}
	}

}
