/****************************************************************************
 *            framebufferbuilder.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce
#define	UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

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
				const
			{
				return m_global_vars;
			}

		public:
			FrameBufferBuilder(
				const GlobalVarsSharedPtr &global_vars);
			~FrameBufferBuilder() throw();
			AbstractFrameBufferSharedPtr build(
				const String &name, const Uint32 width,
				const Uint32 height, const Uint16 mipmaps,
				const TextureFormatType format);
			AbstractFrameBufferSharedPtr build_filter(
				const String &name, const Uint32 width,
				const Uint32 height,
				const TextureFormatType format);
			AbstractFrameBufferSharedPtr build(
				const String &name, const Uint32 width,
				const Uint32 height, const Uint32 layers,
				const Uint16 mipmaps, const Uint16 samples,
				const TextureFormatType format);

	};

}

#endif	/* UUID_f426f5da_c597_483a_a2d1_b0d3d00be4ce */
