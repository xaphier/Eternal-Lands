/****************************************************************************
 *            framebufferbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce
#define	UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "texturetargetutil.hpp"

/**
 * @file
 * @brief The @c class FrameBufferBuilder.
 * This file contains the @c class FrameBufferBuilder.
 */
namespace eternal_lands
{

	class FrameBufferBuilder: public boost::noncopyable
	{
		private:
			const GlobalVarsSharedPtr m_global_vars;

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

		public:
			FrameBufferBuilder(
				const GlobalVarsSharedPtr &global_vars);
			~FrameBufferBuilder() noexcept;
			AbstractFrameBufferSharedPtr build(
				const String &name, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmaps,
				const TextureTargetType target,
				const TextureFormatType format,
				const bool depth_buffer);
			AbstractFrameBufferSharedPtr build_filter(
				const String &name, const Uint32 width,
				const Uint32 height,
				const TextureFormatType format);
			AbstractFrameBufferSharedPtr build(
				const String &name, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmaps, const Uint16 samples,
				const TextureTargetType target,
				const TextureFormatType format,
				const bool layered, const bool depth_buffer);

	};

}

#endif	/* UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce */
