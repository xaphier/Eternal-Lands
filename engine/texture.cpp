/****************************************************************************
 *            texture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texture.hpp"
#include "image.hpp"
#include "imageupdate.hpp"
#include "exceptions.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	Texture::Texture(const String &name, const Uint32 width,
		const Uint32 height, const Uint32 depth,
		const Uint16 mipmap_count, const Uint16 samples,
		const TextureFormatType format,
		const TextureTargetType target): AbstractRenderTarget(name,
			width, height, samples, format),
		m_anisotropic_filter(16.0f), m_texture_id(0),
		m_depth(depth), m_size(0), m_target(target),
		m_mag_filter(tft_linear), m_min_filter(tft_linear),
		m_mipmap_filter(tmt_linear), m_wrap_s(twt_repeat),
		m_wrap_t(twt_repeat), m_wrap_r(twt_repeat),
		m_mipmap_count(mipmap_count), m_used_mipmap_count(mipmap_count),
		m_rebuild(true)
	{
		assert(!get_name().get().empty());
	}

	Texture::~Texture() noexcept
	{
		unload();
	}

	GLenum Texture::get_min_filter(const TextureFilterType texture_filter,
		const TextureMipmapType texture_mipmap)
	{
		switch (texture_mipmap)
		{
			case tmt_none:
				return texture_filter;
			case tmt_nearest:
				switch (texture_filter)
				{
					case tft_nearest:
						return GL_NEAREST_MIPMAP_NEAREST;
					case tft_linear:
						return GL_LINEAR_MIPMAP_NEAREST;
				}
				/*
				 * We should be never here. If so, it's a
				 * programming error, because we forgot to add
				 * all types to the switch or an invalid int
				 * was used (with a type cast)!
				 */
				VALUE_NOT_IN_SWITCH(texture_filter,
					"TextureFilterType");
			case tmt_linear:
				switch (texture_filter)
				{
					case tft_nearest:
						return GL_NEAREST_MIPMAP_LINEAR;
					case tft_linear:
						return GL_LINEAR_MIPMAP_LINEAR;
				}
				/*
				 * We should be never here. If so, it's a
				 * programming error, because we forgot to add
				 * all types to the switch or an invalid int
				 * was used (with a type cast)!
				 */
				VALUE_NOT_IN_SWITCH(texture_filter,
					"TextureFilterType");
		}
		/*
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(texture_mipmap, "TextureMipmapType");
	}

	void Texture::set_texture_filter()
	{
		glTexParameteri(get_gl_target(), GL_TEXTURE_MAG_FILTER,
			get_mag_filter());

		if (get_use_mipmaps())
		{
			glTexParameteri(get_gl_target(), GL_TEXTURE_MIN_FILTER,
				get_min_filter(get_min_filter(),
				get_mipmap_filter()));

			if ((get_anisotropic_filter() >= 1.0f) &&
				(get_mipmap_filter() != tmt_none))
			{
				glTexParameterf(get_gl_target(),
					GL_TEXTURE_MAX_ANISOTROPY_EXT,
					get_anisotropic_filter());
			}
		}
		else
		{
			glTexParameteri(get_gl_target(), GL_TEXTURE_MIN_FILTER,
				get_min_filter());
		}
	}

	void Texture::set_texture_wrap()
	{
		switch (get_target())
		{
			case ttt_texture_3d:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map_array:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_R, get_wrap_r());
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_cube_map:
			case ttt_texture_rectangle:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_T, get_wrap_t());
			case ttt_texture_1d:
				glTexParameteri(get_gl_target(),
					GL_TEXTURE_WRAP_S, get_wrap_s());
				return;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				return;
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an invalid int
		 * was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(get_target(), UTF8("TextureTargetType"));
	}

	void Texture::set_texture_parameter()
	{
		if (get_target() == ttt_texture_rectangle)
		{
			if (get_use_mipmaps())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Can't use "
						"mipmaps for ractangle "
						"textures")));
			}

			if (get_wrap_s() != twt_clamp)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Only valid "
						"wrapping mode for ractangle "
						"textures is clamp")));
			}

			if (get_wrap_t() != twt_clamp)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Only valid "
						"wrapping mode for ractangle "
						"textures is clamp")));
			}
		}

		set_texture_filter();
		set_texture_wrap();

		if (TextureFormatUtil::get_depth(get_format()))
		{
			glTexParameteri(get_gl_target(),
				GL_TEXTURE_COMPARE_MODE,
				GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(get_gl_target(),
				GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}
	}

	void Texture::set_texture_image_1d(const Uint32 width,
		const Uint16 mipmap, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 1d texture '%2%' with width %3% using %4%compressed "
			"data and target '%5%' from image '%6%'."), mipmap %
			get_name() % width %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		assert(image->get_size(mipmap).x == width);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage1D(get_gl_target(), mipmap,
				get_gl_format(), width, 0, size,
				image->get_data(0, mipmap));
		}
		else
		{
			glTexImage1D(get_gl_target(), mipmap, get_gl_format(),
				width, 0, image_format,
				image_type, image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint64 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d texture '%2%' with width %3% and height %4% "
			"using %5%compressed data and target '%6%' from image "
			"'%7%'."), mipmap %  get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		assert(image->get_size(mipmap).x == width);
		assert(image->get_size(mipmap).y == height);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				0, size, image->get_data(0, mipmap));
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, image_format, image_type,
				image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 3d texture '%2%' with width %3%, height %4% and "
			"depth %5% using %6%compressed data and target '%7%' "
			"from image '%8%'."), mipmap % get_name() % width %
			height % depth % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image->get_name());

		assert(image->get_size(mipmap).x == width);
		assert(image->get_size(mipmap).y == height);
		assert(image->get_size(mipmap).z == depth);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				depth, 0, size, image->get_data(0, mipmap));
		}
		else
		{
			glTexImage3D(get_gl_target(), mipmap, get_gl_format(),
				width, height, depth, 0, image_format,
				image_type, image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		Uint32 i;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map texture '%2%' with width %3% and height %4%"
			" using %5%compressed data and target '%6%' from image"
			" '%7%'."), mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		for (i = 0; i < 6; ++i)
		{
			set_texture_image_cube_map_face(width, height, mipmap,
				static_cast<CubeMapFaceType>(i), image);
		}
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const CubeMapFaceType face, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		GLenum gl_face;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);
		gl_face = CubeMapFaceUtil::get_gl_type(face);

		assert(image->get_size(mipmap).x == width);
		assert(image->get_size(mipmap).y == height);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(gl_face, mipmap, get_gl_format(),
				width, height, 0, size,
				image->get_data(face, mipmap));
		}
		else
		{
			glTexImage2D(gl_face, mipmap, get_gl_format(), width,
				height, 0, image_format, image_type,
				image->get_data(face, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		Uint32 i;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" cube map array texture '%2%' with width %3%, height "
			"%4% and depth %5% using %6%compressed data and target"
			" '%7%' from image '%8%'."), mipmap % get_name() %
			width % height % depth %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			image->get_name());

		for (i = 0; i < 6; ++i)
		{
			set_texture_image_cube_map_face(width, height, depth,
				mipmap, static_cast<CubeMapFaceType>(i),
				image);
		}
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const CubeMapFaceType face, const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);

		assert(image->get_size(mipmap).x == width);
		assert(image->get_size(mipmap).y == height);
		assert(image->get_size(mipmap).z == depth);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(face, mipmap, get_gl_format(),
				width, height, depth, 0, size,
				image->get_data(face, mipmap));
		}
		else
		{
			glTexImage3D(face, mipmap, get_gl_format(), width,
				height, depth, 0, image_format, image_type,
				image->get_data(face, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d_layer(const Uint32 width,
		const Uint32 height, const Uint32 layer, const Uint16 mipmap,
		const ImageSharedPtr &image)
	{
		GLenum image_format, image_type;
		Uint32 size;
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(
			image->get_texture_format());
		image_format = image->get_format();
		image_type = image->get_type();
		size = image->get_mipmap_size(mipmap);

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d array texture '%2%' with width %3%, height %4% and"
			" layer %5% using %6%compressed data and target '%7%' "
			"from image '%8%'."), mipmap %  get_name() % width %
			height % layer % (compressed ? UTF8("") : UTF8("un")) %
			get_target() % image->get_name());

		assert(image->get_size(mipmap).x == width);
		assert(image->get_size(mipmap).y == height);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(get_gl_target(), mipmap, 0, 0,
				layer, width, height, 1, get_gl_format(), size,
				image->get_data(0, mipmap));
		}
		else
		{
			glTexSubImage3D(get_gl_target(), mipmap, 0, 0, layer,
				width, height, 1, image_format, image_type,
				image->get_data(0, mipmap));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_1d(const String &name,
		const void* buffer, const glm::uvec3 &offset,
		const glm::uvec3 &size, const Uint64 buffer_size,
		const GLenum format, const GLenum type, const Uint16 mipmap,
		const bool compressed)
	{
		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 1d texture '%2%' with width %3% using %4%compressed "
			"data and target '%5%' from source '%6%'."),
			mipmap % get_name() % size[0] % (compressed ? UTF8("") :
				UTF8("un")) % get_target() % name);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage1D(get_gl_target(),
				mipmap, offset[0], size[0], get_gl_format(),
				buffer_size, buffer);
		}
		else
		{
			glTexSubImage1D(get_gl_target(), mipmap, offset[0],
				size[0], format, type, buffer);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const String &name,
		const void* buffer, const glm::uvec3 &offset,
		const glm::uvec3 &size, const Uint64 buffer_size,
		const GLenum format, const GLenum type, const Uint16 mipmap,
		const bool compressed)
	{
		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 2d texture '%2%' with width %3% and height %4% using"
			" %5%compressed data and target '%6%' from source "
			"'%7%'."), mipmap % get_name() % size[0] % size[1] %
			(compressed ? UTF8("") : UTF8("un")) % get_target() %
			name);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage2D(get_gl_target(),
				mipmap, offset[0], offset[1], size[0], size[1],
				get_gl_format(), buffer_size, buffer);
		}
		else
		{
			glTexSubImage2D(get_gl_target(), mipmap, offset[0],
				offset[1], size[0], size[1], format, type,
				buffer);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const String &name,
		const void* buffer, const glm::uvec3 &offset,
		const glm::uvec3 &size, const Uint64 buffer_size,
		const GLenum format, const GLenum type, const Uint16 mipmap,
		const bool compressed)
	{
		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% of"
			" 3d texture '%2%' with width %3%, height %4% and depth"
			" %5% using %6%compressed data and target '%7%' from "
			"source '%8%'."), mipmap % get_name() % size[0] %
			size[1] % size[2] % (compressed ? UTF8("") : UTF8("un"))
			% get_target() % name);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(get_gl_target(), mipmap,
				offset[0], offset[1], offset[2], size[0],
				size[1], size[2], get_gl_format(), buffer_size,
				buffer);
		}
		else
		{
			glTexSubImage3D(get_gl_target(), mipmap, offset[0],
				offset[1], offset[2], size[0], size[1],
				size[2], format, type, buffer);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map_face(const String &name,
		const void* buffer, const glm::uvec3 &offset,
		const glm::uvec3 &size, const Uint64 buffer_size,
		const CubeMapFaceType face, const GLenum format,
		const GLenum type, const Uint16 mipmap, const bool compressed)
	{
		GLenum gl_face;

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% "
			" and face %2% of cube map texture '%3%' with width "
			"%4% and height %5% using %6%compressed data and "
			"target '%7%' from source '%8%'."), mipmap % face %
			get_name() % size[0] % size[1] % (compressed ? UTF8("")
				: UTF8("un")) % get_target() % name);

		gl_face = CubeMapFaceUtil::get_gl_type(face);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage2D(gl_face, mipmap, offset[0],
				offset[1], size[0], size[1], get_gl_format(),
				buffer_size, buffer);
		}
		else
		{
			glTexSubImage2D(gl_face, mipmap, offset[0], offset[1],
				size[0], size[1], format, type, buffer);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map_array_face(const String &name,
		const void* buffer, const glm::uvec3 &offset,
		const glm::uvec3 &size, const Uint64 buffer_size,
		const CubeMapFaceType face, const GLenum format,
		const GLenum type, const Uint16 mipmap, const bool compressed)
	{
		GLenum gl_face;

		LOG_DEBUG(lt_texture, UTF8("Setting texture mipmap leve %1% "
			"and face %2% of cube map array texture '%3%' with "
			"width %4%, height %5% and depth %6% using "
			"%7%compressed data and target '%8%' from source "
			"'%9%'."), mipmap % face % get_name() %	size[0] %
			size[1] % size[2] % (compressed ? UTF8("") :
				UTF8("un")) % get_target() % name);

		gl_face = CubeMapFaceUtil::get_gl_type(face);

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexSubImage3D(gl_face, mipmap, offset[0],
				offset[1], offset[2], size[0], size[1],
				size[2], get_gl_format(), buffer_size, buffer);
		}
		else
		{
			glTexSubImage3D(gl_face, mipmap, offset[0], offset[1],
				offset[2], size[0], size[1], size[2], format,
				type, buffer);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_1d(const Uint32 width,
		const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, 1, 1, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 1d texture '%2%' with width %3% using "
			"%4%compressed format and target '%5%'."), mipmap %
			get_name() % width %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage1D(get_gl_target(), mipmap,
				get_gl_format(), width, 0, size, 0);
		}
		else
		{
			glTexImage1D(get_gl_target(), mipmap, get_gl_format(),
				width, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d(const Uint32 width,
		const Uint32 height, const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, 1, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 2d texture '%2%' with width %3% and height %4%"
			" using %5%compressed format and target '%6%'."),
			mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height, 0, size, 0);
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, depth, get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of 3d texture '%2%' with width %3%, height %4% "
			"and depth %5% using %6%compressed format and target "
			"'%7%'."), mipmap % get_name() % width % height % depth
			% (compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		if (compressed)
		{
			glCompressedTexImage3D(get_gl_target(), mipmap,
				get_gl_format(), width, height,
				depth, 0, size, 0);
		}
		else
		{
			glTexImage3D(get_gl_target(), mipmap, get_gl_format(),
				width, height, depth, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint16 mipmap)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of cube map texture '%2%' with width %3% and "
			"height %4% using %5%compressed format and target "
			"'%6%'."), mipmap % get_name() % width % height %
			(compressed ? UTF8("") : UTF8("un")) % get_target());

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_x);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_x);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_y);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_y);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_positive_z);
		set_texture_image_cube_map_face(width, height, mipmap,
			cmft_negative_z);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint16 mipmap,
		const CubeMapFaceType face)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		CHECK_GL_ERROR_NAME(get_name());

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, 1, get_format());

		if (compressed)
		{
			glCompressedTexImage2D(get_gl_target(), mipmap,
				get_gl_format(), width, height, 0, size, 0);
		}
		else
		{
			glTexImage2D(get_gl_target(), mipmap, get_gl_format(),
				width, height, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_cube_map(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap)
	{
		bool compressed;

		compressed = TextureFormatUtil::get_compressed(get_format());

		LOG_DEBUG(lt_texture, UTF8("Initializing texture mipmap level "
			"%1% of cube map texture '%2%' with width %3%, height "
			"%4% and depth %5% using %6%compressed format and "
			"target '%7%'."), mipmap % get_name() % width % height
			% depth % (compressed ? UTF8("") : UTF8("un")) %
			get_target());

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_x);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_x);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_y);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_y);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_positive_z);
		set_texture_image_cube_map_face(width, height, depth, mipmap,
			cmft_negative_z);
	}

	void Texture::set_texture_image_cube_map_face(const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmap,
		const CubeMapFaceType face)
	{
		Uint32 size;
		GLenum format, type;
		bool compressed;

		CHECK_GL_ERROR_NAME(get_name());

		TextureFormatUtil::get_source_format(get_format(), format,
			type);

		compressed = TextureFormatUtil::get_compressed(get_format());
		size = Image::get_size(width, height, depth, get_format());

		if (compressed)
		{
			glCompressedTexImage3D(face, mipmap, get_gl_format(),
				width, height, depth, 0, size, 0);
		}
		else
		{
			glTexImage3D(face, mipmap, get_gl_format(), width,
				height, depth, 0, format, type, 0);
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_2d_multisample()
	{
		CHECK_GL_ERROR_NAME(get_name());

		glTexImage2DMultisample(get_gl_target(), get_samples(),
			get_gl_format(), get_width(), get_height(), false);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::set_texture_image_3d_multisample()
	{
		CHECK_GL_ERROR_NAME(get_name());

		glTexImage3DMultisample(get_gl_target(), get_samples(),
			get_gl_format(), get_width(), get_height(),
			get_depth(), false);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::get_image_size(const ImageSharedPtr &image,
		Uint32 &width, Uint32 &height, Uint32 &depth,
		Uint32 &mipmap_count, Uint32 &layer_count) const
	{
		width = image->get_width();
		height = image->get_height();
		depth = image->get_depth();
		layer_count = image->get_layer_count();
		mipmap_count = image->get_mipmap_count();

		if (get_target() == image->get_texture_target())
		{
			return;
		}

		switch (get_target())
		{
			case ttt_texture_1d:
				if (image->get_texture_target() ==
					ttt_texture_1d_array)
				{
					if (image->get_layer_count() > 1)
					{
						LOG_DEBUG(lt_texture, UTF8(
							"Only first layer of "
							"%1% from image '%2%' "
							"used for texture "
							"'%3%'."),
							image->get_layer_count()
							% image->get_name() %
							get_name());
					}

					return;
				}

				if (((image->get_texture_target() ==
					ttt_texture_2d) ||
					(image->get_texture_target() ==
						ttt_texture_rectangle)) &&
					(image->get_height() <= 1))
				{
					return;
				}

				break;
			case ttt_texture_1d_array:
				if (image->get_texture_target() ==
					ttt_texture_1d)
				{
					layer_count = 1;
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_rectangle)
				{
					layer_count = image->get_height();
					break;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d)
				{
					layer_count = image->get_height();

					if (image->has_mipmaps())
					{
						LOG_DEBUG(lt_texture, UTF8(
							"No mipmaps from image"
							" '%1%' used for "
							"texture '%2%'."),
							image->get_name() %
							get_name());
					}

					mipmap_count = 0;
					return;
				}

				break;
			case ttt_texture_2d:
				if (image->get_texture_target() ==
					ttt_texture_1d)
				{
					height = 1;
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_1d_array)
				{
					if (image->has_mipmaps())
					{
						LOG_DEBUG(lt_texture, UTF8(
							"No mipmaps from image"
							" '%1%' used for "
							"texture '%2%'."),
							image->get_name() %
							get_name());
					}

					mipmap_count = 0;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_rectangle)
				{
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d_array)
				{
					if (image->get_layer_count() > 1)
					{
						LOG_DEBUG(lt_texture, UTF8(
							"Only first layer of "
							"%1% from image '%2%' "
							"used for texture "
							"'%3%'."),
							image->get_layer_count()
							% image->get_name() %
							get_name());
					}
					return;
				}

				if ((image->get_texture_target() ==
					ttt_texture_3d) &&
					(image->get_height() <= 1))
				{
					return;
				}

				break;
			case ttt_texture_rectangle:
				if (image->has_mipmaps())
				{
					LOG_DEBUG(lt_texture, UTF8("No mipmaps"
						" from image '%1%' used for "
						"texture '%2%'."),
						image->get_name() % get_name());
				}

				mipmap_count = 0;

				if (image->get_texture_target() ==
					ttt_texture_1d)
				{
					height = 1;
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_1d_array)
				{
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d)
				{
					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d_array)
				{
					if (image->get_layer_count() > 1)
					{
						LOG_DEBUG(lt_texture, UTF8(
							"Only first layer of "
							"%1% from image '%2%' "
							"used for texture "
							"'%3%'."),
							image->get_layer_count()
							% image->get_name() %
							get_name());
					}

					return;
				}

				if ((image->get_texture_target() ==
					ttt_texture_3d) &&
					(image->get_height() <= 1))
				{
					return;
				}

				break;
			case ttt_texture_3d:
				if (image->get_texture_target() ==
					ttt_texture_1d)
				{
					height = 1;
					depth = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_1d_array)
				{
					depth = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d)
				{
					depth = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_rectangle)
				{
					depth = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d_array)
				{
					if (image->has_mipmaps())
					{
						LOG_DEBUG(lt_texture, UTF8(
							"No mipmaps from image"
							" '%1%' used for "
							"texture '%2%'."),
							image->get_name() %
							get_name());
					}

					mipmap_count = 0;

					return;
				}

				break;
			case ttt_texture_2d_array:
				if (image->get_texture_target() ==
					ttt_texture_1d)
				{
					height = 1;
					depth = 1;
					layer_count = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_1d_array)
				{
					height = 1;
					depth = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_2d)
				{
					depth = 1;
					layer_count = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_rectangle)
				{
					depth = 1;
					layer_count = 1;

					return;
				}

				if (image->get_texture_target() ==
					ttt_texture_3d)
				{
					layer_count = image->get_depth();

					if (image->has_mipmaps())
					{
						LOG_DEBUG(lt_texture, UTF8(
							"No mipmaps from image"
							" '%1%' used for "
							"texture '%2%'."),
							image->get_name() %
							get_name());
					}

					mipmap_count = 0;

					break;
				}

				break;
			case ttt_texture_cube_map:
				if (image->get_texture_target() ==
					ttt_texture_cube_map_array)
				{
					if (image->get_layer_count() > 1)
					{
						LOG_DEBUG(lt_texture, UTF8(
							"Only first layer of "
							"%1% from image '%2%' "
							"used for texture "
							"'%3%'."),
							image->get_layer_count()
							% image->get_name() %
							get_name());
					}

					return;
				}

				break;
			case ttt_texture_cube_map_array:
				if (image->get_texture_target() ==
					ttt_texture_cube_map)
				{
					layer_count = 1;
					depth = 1;

					return;
				}

				break;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				break;
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Can't use image '%1%' with "
			"texture target type '%2%' as source for texture '%3%'"
			" with texture target type '%4%'"), image->get_name() %
			image->get_texture_target() % get_name() %
			get_target(), InvalidParameterException());
	}

	void Texture::do_set_image(const ImageSharedPtr &image)
	{
		Uint32 width, height, depth, mip, level, mipmap_count;
		Uint32 layer_count;

		CHECK_GL_ERROR();

		get_image_size(image, width, height, depth, mipmap_count,
			layer_count);

		m_rebuild |= (get_width() != width) ||
			(get_height() != height) ||
			(get_depth() != depth);
		set_width(width);
		set_height(height);
		set_depth(depth);

		build_texture_id();
		bind();

		CHECK_GL_ERROR_NAME(get_name());

		if (mipmap_count > 0)
		{
			mipmap_count = std::min(mipmap_count,
				static_cast<Uint32>(get_mipmap_count()));
			level = mipmap_count + 1;

			glTexParameteri(get_gl_target(), GL_TEXTURE_MAX_LEVEL,
				mipmap_count);
		}
		else
		{
			if (get_mipmap_count() > 0)
			{
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

				glTexParameteri(get_gl_target(),
					GL_TEXTURE_MAX_LEVEL,
					get_mipmap_count());

				glTexParameteri(get_gl_target(),
					GL_GENERATE_MIPMAP, GL_TRUE);
				mipmap_count = get_mipmap_count();
			}
			else
			{
				mipmap_count = 0;
			}

			level = 1;
		}

		m_used_mipmap_count = mipmap_count;

		LOG_DEBUG(lt_texture, UTF8("Loading texture %1% of format "
			"%2% and target '%3%'."), get_name() % get_format() %
			get_target());

		for (mip = 0; mip < level; mip++)
		{
			switch (get_target())
			{
				case ttt_texture_1d:
					set_texture_image_1d(width, mip, image);
					break;
				case ttt_texture_1d_array:
					set_texture_image_2d(width, layer_count,
						mip, image);
					break;
				case ttt_texture_2d:
				case ttt_texture_rectangle:
					set_texture_image_2d(width, height,
						mip, image);
					break;
				case ttt_texture_3d:
					set_texture_image_3d(width, height,
						depth, mip, image);
					break;
				case ttt_texture_2d_array:
					set_texture_image_3d(width, height,
						layer_count, mip, image);
					break;
				case ttt_texture_cube_map:
					set_texture_image_cube_map(width,
						height, mip, image);
					break;
				case ttt_texture_cube_map_array:
					set_texture_image_cube_map(width,
						height, layer_count, mip,
						image);
					break;
				case ttt_texture_2d_multisample:
				case ttt_texture_2d_multisample_array:
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< errinfo_message(UTF8("Can't"
							" set image of "
							"multisample "
							"texture")));
					break;
			}
			if (width > 1)
			{
				width = width / 2;
			}
			if (height > 1)
			{
				height = height / 2;
			}
			if (depth > 1)
			{
				depth = depth / 2;
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::do_set_images(const ImageSharedPtrVector &images)
	{
		Uint32 i, width, height, depth, mip, level, mipmap_count;
		Uint32 layer_count;

		CHECK_GL_ERROR();

		RANGE_CECK_MIN(images.size(), 1, UTF8("not enough images."));

		get_image_size(images[0], width, height, depth, mipmap_count,
			layer_count);

		depth = images.size();
		layer_count = images.size();

		m_rebuild |= (get_width() != width) ||
			(get_height() != height) ||
			(get_depth() != depth);

		set_width(width);
		set_height(height);
		set_depth(depth);

		BOOST_FOREACH(const ImageSharedPtr &image, images)
		{
			if (get_width() != image->get_width())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("wrong width"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_width())
					<< errinfo_expected_value(get_width(
						)));
			}

			if (get_height() != image->get_height())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("wrong height"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_height())
					<< errinfo_expected_value(get_height(
						)));
			}

			if (mipmap_count > image->get_mipmap_count())
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("not enought "
						"mipmap levels"))
					<< errinfo_name(get_name())
					<< errinfo_item_name(image->get_name())
					<< errinfo_value(image->get_height())
					<< errinfo_expected_value(get_height(
						)));
			}
		}

		CHECK_GL_ERROR_NAME(get_name());

		build_texture_id();
		bind();

		if (mipmap_count > 0)
		{
			mipmap_count = std::min(mipmap_count,
				static_cast<Uint32>(get_mipmap_count()));
			level = mipmap_count + 1;

			glTexParameteri(get_gl_target(), GL_TEXTURE_MAX_LEVEL,
				mipmap_count);
		}
		else
		{
			if (get_mipmap_count() > 0)
			{
				glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

				glTexParameteri(get_gl_target(),
					GL_TEXTURE_MAX_LEVEL,
					get_mipmap_count());

				glTexParameteri(get_gl_target(),
					GL_GENERATE_MIPMAP, GL_TRUE);

				mipmap_count = get_mipmap_count();
			}
			else
			{
				mipmap_count = 0;
			}

			level = 1;
		}

		m_used_mipmap_count = mipmap_count;

		CHECK_GL_ERROR_NAME(get_name());

		for (mip = 0; mip < level; mip++)
		{
			set_texture_image_3d(width, height, layer_count, mip);

			if (width > 1)
			{
				width = width / 2;
			}

			if (height > 1)
			{
				height = height / 2;
			}
		}

		for (i = 0; i < layer_count; ++i)
		{
			width = get_width();
			height = get_height();

			for (mip = 0; mip < level; mip++)
			{
				set_texture_image_2d_layer(width, height, i,
					mip, images[i]);

				if (width > 1)
				{
					width = width / 2;
				}

				if (height > 1)
				{
					height = height / 2;
				}
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::do_sub_texture(const String &name, const void* buffer,
		const glm::uvec3 &offset, const glm::uvec3 &size,
		const Uint64 buffer_size, const CubeMapFaceType face,
		const GLenum format, const GLenum type, const Uint16 mipmap,
		const bool compressed)
	{
		CHECK_GL_ERROR();

		bind();

		CHECK_GL_ERROR_NAME(get_name());

		switch (get_target())
		{
			case ttt_texture_1d:
				set_texture_image_1d(name, buffer, offset,
					size, buffer_size, format, type,
					mipmap, compressed);
				break;
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_rectangle:
				set_texture_image_2d(name, buffer, offset,
					size, buffer_size, format, type,
					mipmap, compressed);
				break;
			case ttt_texture_3d:
			case ttt_texture_2d_array:
				set_texture_image_3d(name, buffer, offset,
					size, buffer_size, format, type,
					mipmap, compressed);
				break;
			case ttt_texture_cube_map:
				set_texture_image_cube_map_face(name, buffer,
					offset, size, buffer_size, face,
					format, type, mipmap, compressed);
				break;
			case ttt_texture_cube_map_array:
				set_texture_image_cube_map_array_face(name,
					buffer, offset, size, buffer_size, face,
					format, type, mipmap, compressed);
				break;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< errinfo_message(UTF8("Can't set "
						"subimage of multisample "
						"texture")));
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::calc_size()
	{
		Uint32 width, height, depth, mip, faces, layer_count, samples;
		Uint32 used_mipmap_count;

		width = get_width();
		height = get_height();
		depth = get_depth();
		samples = get_samples();
		faces = 1;
		layer_count = 1;

		used_mipmap_count = get_used_mipmap_count();

		m_size = 0;

		switch (get_target())
		{
			case ttt_texture_1d:
				height = 1;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_1d_array:
				layer_count = depth;
				height = 1;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_3d:
				samples = 1;
				break;
			case ttt_texture_2d_array:
				layer_count = depth;
				depth = 1;
				samples = 1;
				break;
			case ttt_texture_cube_map:
				depth = 1;
				faces = 6;
				samples = 1;
				break;
			case ttt_texture_cube_map_array:
				layer_count = depth;
				depth = 1;
				faces = 6;
				samples = 1;
				break;
			case ttt_texture_2d_multisample:
				depth = 1;
				used_mipmap_count = 1;
				break;
			case ttt_texture_2d_multisample_array:
				layer_count = depth;
				depth = 1;
				used_mipmap_count = 1;
				break;
		}

		for (mip = 0; mip < (used_mipmap_count + 1); mip++)
		{
			m_size += (width * height * depth * layer_count * faces
				* samples * 
				TextureFormatUtil::get_size(get_format()) + 7)
				/ 8;
			if ((width == 1) && (height == 1) && (depth == 1))
			{
				break;
			}
			if (width > 1)
			{
				width = width / 2;
			}
			if (height > 1)
			{
				height = height / 2;
			}
			if (depth > 1)
			{
				depth = depth / 2;
			}
		}
	}

	void Texture::do_init(const Uint32 width, const Uint32 height,
		const Uint32 depth, const Uint16 mipmap_count,
		const Uint16 samples)
	{
		Uint32 w, h, d, mip, layer_count;

		CHECK_GL_ERROR();

		m_rebuild |= (get_width() != width) || (get_height() != height)
			|| (get_depth() != depth) ||
			(get_used_mipmap_count() != mipmap_count) ||
			(get_samples() != samples);
		set_width(width);
		set_height(height);
		set_depth(depth);
		set_samples(samples);

		CHECK_GL_ERROR_NAME(get_name());

		build_texture_id();
		bind();

		CHECK_GL_ERROR_NAME(get_name());

		m_used_mipmap_count = mipmap_count;

		w = get_width();
		h = get_height();
		d = get_depth();
		layer_count = get_depth();

		for (mip = 0; mip <= mipmap_count; mip++)
		{
			switch (get_target())
			{
				case ttt_texture_1d:
					set_texture_image_1d(w, mip);
					break;
				case ttt_texture_2d:
				case ttt_texture_1d_array:
				case ttt_texture_rectangle:
					set_texture_image_2d(w, h, mip);
					break;
				case ttt_texture_3d:
					set_texture_image_3d(w, h, d, mip);
					break;
				case ttt_texture_2d_array:
					set_texture_image_3d(w, h, layer_count,
						mip);
					break;
				case ttt_texture_cube_map:
					set_texture_image_cube_map(w, h, mip);
					break;
				case ttt_texture_cube_map_array:
					set_texture_image_cube_map(w, h,
						layer_count, mip);
					break;
				case ttt_texture_2d_multisample:
					set_texture_image_2d_multisample();
					break;
				case ttt_texture_2d_multisample_array:
					set_texture_image_3d_multisample();
					break;
			}
			if (w > 1)
			{
				w = w / 2;
			}
			if (h > 1)
			{
				h = h / 2;
			}
			if (d > 1)
			{
				d = d / 2;
			}
		}

		calc_size();

		CHECK_GL_ERROR_NAME(get_name());

		set_texture_parameter();

		CHECK_GL_ERROR_NAME(get_name());

		unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::do_attach_ext(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
				glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_3d:
				glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT,
					attachment, get_gl_target(),
					get_texture_id(), level, layer);
				break;
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map:
			case ttt_texture_cube_map_array:
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::do_attach(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
				glFramebufferTexture1D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_2d:
			case ttt_texture_rectangle:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level);
				break;
			case ttt_texture_3d:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), level, layer);
				break;
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
				glFramebufferTextureLayer(GL_FRAMEBUFFER,
					attachment, get_texture_id(), level,
					layer);
				break;
			case ttt_texture_2d_multisample:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), 0);
				break;
			case ttt_texture_2d_multisample_array:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, get_gl_target(),
					get_texture_id(), 0, layer);
				break;
			case ttt_texture_cube_map:
			case ttt_texture_cube_map_array:
			default:
				assert(false);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::do_attach(const GLenum attachment,
		const CubeMapFaceType face, const Uint32 level,
		const Uint32 layer)
	{
		CHECK_GL_ERROR();

		switch (get_target())
		{
			case ttt_texture_1d:
			case ttt_texture_2d:
			case ttt_texture_1d_array:
			case ttt_texture_rectangle:
			case ttt_texture_3d:
			case ttt_texture_2d_array:
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				assert(false);
				break;
			case ttt_texture_cube_map:
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					attachment, face, get_texture_id(),
					level);
				break;
			case ttt_texture_cube_map_array:
				glFramebufferTexture3D(GL_FRAMEBUFFER,
					attachment, face, get_texture_id(),
					level, layer);
				break;
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::sub_texture(const String &name,
		const AbstractReadMemorySharedPtr &buffer,
		const glm::uvec3 &offset, const glm::uvec3 &size,
		const CubeMapFaceType face, const GLenum format, const GLenum type,
		const Uint16 mipmap, const bool compressed)
	{
		LOG_DEBUG(lt_texture, UTF8("Set face %1% of sub texture '%2%' "
			"to '%3%' %4%"), face % get_name() % name %
			UTF8("started"));

		try
		{
			do_sub_texture(name, buffer->get_ptr(), offset, size,
				buffer->get_size(), face, format, type,
				mipmap, compressed);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting face %1% of sub "
				"texture '%2%' to '%3%' caught exception "
				"'%4%'"), face % get_name() % name %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting face %1% of sub "
				"texture '%2%' to '%3%' caught exception "
				"'%4%'"), face % get_name() % name %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While setting face %1% of sub "
				"texture '%2%' to '%3%' caught '%4%'"), face %
				get_name() % name % UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Set face %1% of sub texture '%2%' "
			"to '%3%' %4%"), face % get_name() % name %
			UTF8("done"));
	}

	void Texture::sub_texture(const ImageUpdate &image_update)
	{
		sub_texture(image_update.get_name(), image_update.get_buffer(),
			image_update.get_offset(), image_update.get_size(),
			image_update.get_face(), image_update.get_format(),
			image_update.get_type(), image_update.get_mipmap(),
			image_update.get_compressed());
	}

	void Texture::update_image(const ImageSharedPtr &image,
		const BitSet16 &faces, const BitSet16 &mipmaps)
	{
		glm::uvec3 size;
		CubeMapFaceType face;
		Uint32 i, j, count, face_count, buffer_size;

		count = std::min(image->get_mipmap_count(),
			get_used_mipmap_count()) + 1;

		face_count = std::min(image->get_face_count(),
			get_face_count());

		for (i = 0; i < face_count; ++i)
		{
			if (!faces[i] && get_cube_map())
			{
				continue;
			}

			face = static_cast<CubeMapFaceType>(i);

			size = glm::max(image->get_size(), glm::uvec3(1));

			for (j = 0; j < count; ++j)
			{
				buffer_size = image->get_mipmap_size(j);

				do_sub_texture(image->get_name(),
					image->get_data(i, j), glm::uvec3(0),
					size, buffer_size, face,
					image->get_format(), image->get_type(),
					j, image->get_compressed());

				size = glm::max(size / 2u, glm::uvec3(1));
			}
		}
	}

	void Texture::update_image_layer(const ImageSharedPtr &image,
		const BitSet16 &faces, const BitSet16 &mipmaps,
		const Uint16 layer)
	{
		glm::uvec3 size;
		CubeMapFaceType face;
		Uint32 i, j, count, face_count, buffer_size, x, y, z;

		count = std::min(image->get_mipmap_count(),
			get_used_mipmap_count()) + 1;

		face_count = std::min(image->get_face_count(),
			get_face_count());

		x = 0;
		y = 0;
		z = 0;

		switch (get_target())
		{
			case ttt_texture_3d:
			case ttt_texture_1d_array:
			case ttt_texture_2d_array:
			case ttt_texture_cube_map_array:
				return;
			case ttt_texture_2d:
			case ttt_texture_cube_map:
			case ttt_texture_rectangle:
				x = 0;
				y = 0;
				z = layer;
				break;
			case ttt_texture_1d:
				x = 0;
				y = layer;
				z = 0;
				break;
			case ttt_texture_2d_multisample:
			case ttt_texture_2d_multisample_array:
				return;
		}

		for (i = 0; i < face_count; ++i)
		{
			if (!faces[i] && get_cube_map())
			{
				continue;
			}

			face = static_cast<CubeMapFaceType>(i);

			size = glm::max(image->get_size(), glm::uvec3(1));

			for (j = 0; j < count; ++j)
			{
				buffer_size = image->get_mipmap_size(j);

				do_sub_texture(image->get_name(),
					image->get_data(x, y, z, i, j),
					glm::uvec3(0),
					size, buffer_size, face,
					image->get_format(), image->get_type(),
					j, image->get_compressed());

				size = glm::max(size / 2u, glm::uvec3(1));
			}
		}
	}

	void Texture::set_image(const ImageSharedPtr &image)
	{
		String name;

		name = UTF8("(nullptr)");

		if (image.get() != nullptr)
		{
			name = image->get_name();
		}

		LOG_DEBUG(lt_texture, UTF8("Set texture '%1%' to image '%2%' "
			"%3%"), get_name() % name % UTF8("started"));

		try
		{
			do_set_image(image);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" image '%2%' caught exception '%3%'"),
				get_name() % name %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" image '%2%' caught exception '%3%'"),
				get_name() % name % exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" image '%2%' caught '%3%'"), get_name() %
				name % UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Set texture '%1%' to image '%2%' "
			"%3%"), get_name() % name % UTF8("done"));
	}

	void Texture::set_images(const ImageSharedPtrVector &images)
	{
		StringStream str;
		String name;

		BOOST_FOREACH(const ImageSharedPtr &image, images)
		{
			if (image.get() != nullptr)
			{
				str << image->get_name() << std::endl;
			}
			else
			{
				str << UTF8("(nullptr)") << std::endl;
			}
		}

		name = String(str.str());

		LOG_DEBUG(lt_texture, UTF8("Set texture '%1%' to images '%2%' "
			"%3%"), get_name() % name % UTF8("started"));

		try
		{
			do_set_images(images);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" images '%2%' caught exception '%3%'"),
				get_name() % name %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" images '%2%' caught exception '%3%'"),
				get_name() % name % exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While setting texture '%1%' to"
				" images '%2%' caught '%3%'"), get_name() %
				name % UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Set texture '%1%' to images '%2%' "
			"%3%"), get_name() % name % UTF8("done"));
	}

	void Texture::init(const Uint32 width, const Uint32 height,
		const Uint32 depth, const Uint16 mipmaps, const Uint16 samples)
	{
		LOG_DEBUG(lt_texture, UTF8("Init texture '%1%' to <%2%, %3%, "
			"%4%>-%5% (%6%) %7%"), get_name() % width % height %
			depth % mipmaps % samples % UTF8("started"));

		try
		{
			do_init(width, height, depth, mipmaps, samples);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While initializing texture "
				"'%1%' to <%2%, %3%, %4%>-%5% (%6%) caught "
				"exception '%7%'"), get_name() % width % height
				% depth % mipmaps % samples %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While initializing texture "
				"'%1%' to <%2%, %3%, %4%>-%5% (%6%) caught "
				"exception '%7%'"), get_name() % width % height
				% depth % mipmaps % samples % exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While initializing texture "
				"'%1%' to <%2%, %3%, %4%>-%5% (%6%) caught "
				"'%7%'"), get_name() % width % height
				% depth % mipmaps % samples %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Init texture '%1%' to <%2%, %3%, "
			"%4%>-%5% (%6%) %7%"), get_name() % width % height %
			depth % mipmaps % samples % UTF8("done"));
	}

	void Texture::attach_ext(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to level %2% "
			"at layer %3% %4%"), get_name() % level % layer %
			UTF8("started"));

		try
		{
			do_attach_ext(attachment, level, layer);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught exception "
				"'%4%'"), get_name() % level % layer %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught exception "
				"'%4%'"), get_name() % level % layer %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught '%4%'"),
				get_name() % level % layer %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to level %2% "
			"at layer %3% %4%"), get_name() % level % layer %
			UTF8("done"));
	}

	void Texture::attach(const GLenum attachment, const Uint32 level,
		const Uint32 layer)
	{
		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to level %2% "
			"at layer %3% %4%"), get_name() % level % layer %
			UTF8("started"));

		try
		{
			do_attach(attachment, level, layer);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught exception "
				"'%4%'"), get_name() % level % layer %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught exception "
				"'%4%'"), get_name() % level % layer %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While attaching texture '%1%' "
				"to level %2% at layer %3% caught '%4%'"),
				get_name() % level % layer %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to level %2% "
			"at layer %3% %4%"), get_name() % level % layer %
			UTF8("done"));
	}

	void Texture::attach(const GLenum attachment,
		const CubeMapFaceType face, const Uint32 level,
		const Uint32 layer)
	{
		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to face %2% "
			"and level %3% at layer %4% %5%"), get_name() % face %
			level % layer % UTF8("started"));

		try
		{
			do_attach(attachment, face, level, layer);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attachingtexture '%1%' "
				"to face %2% and level %3% at layer %4% caught"
				" exception '%5%'"), get_name() % face %
				level % layer %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While attachingtexture '%1%' "
				"to face %2% and level %3% at layer %4% caught"
				" exception '%5%'"), get_name() % face %
				level % layer % exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While attachingtexture '%1%' "
				"to face %2% and level %3% at layer %4% caught"
				" '%5%'"), get_name() % face % level % layer %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_texture, UTF8("Attach texture '%1%' to face %2% "
			"and level %3% at layer %4% %5%"), get_name() % face %
			level % layer % UTF8("done"));
	}

	void Texture::build_texture_id()
	{
		CHECK_GL_ERROR();

		if (m_rebuild)
		{
			if (m_texture_id != 0)
			{
				glDeleteTextures(1, &m_texture_id);
				m_texture_id = 0;
				m_rebuild = false;
			}
		}

		if (m_texture_id == 0)
		{
			glGenTextures(1, &m_texture_id);
			m_rebuild = false;
		}

		assert(!m_rebuild);
		assert(m_texture_id != 0);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void Texture::unload() noexcept
	{
		glDeleteTextures(1, &m_texture_id);

		m_size = 0;
		m_texture_id = 0;
	}

	String Texture::get_str(const TextureFilterType value)
	{
		switch (value)
		{
			case tft_nearest:
				return String(UTF8("nearest"));
			case tft_linear:
				return String(UTF8("linear"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureFilterType"));
	}

	String Texture::get_str(const TextureMipmapType value)
	{
		switch (value)
		{
			case tmt_none:
				return String(UTF8("none"));
			case tmt_nearest:
				return String(UTF8("nearest"));
			case tmt_linear:
				return String(UTF8("linear"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureMipmapType"));
	}

	String Texture::get_str(const TextureWrapType value)
	{
		switch (value)
		{
			case twt_clamp:
				return String(UTF8("clamp"));
			case twt_mirrored_repeat:
				return String(UTF8("mirrored_repeat"));
			case twt_repeat:
				return String(UTF8("repeat"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("TextureWrapType"));
	}

	OutStream &operator<<(OutStream &str, const TextureFilterType value)
	{
		str << Texture::get_str(value);

		return str;
	}

	OutStream &operator<<(OutStream &str, const TextureMipmapType value)
	{
		str << Texture::get_str(value);

		return str;
	}

	OutStream &operator<<(OutStream &str, const TextureWrapType value)
	{
		str << Texture::get_str(value);

		return str;
	}

}
