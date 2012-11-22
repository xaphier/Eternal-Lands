/****************************************************************************
 *            texture.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd
#define	UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "texturetargetutil.hpp"
#include "abstractrendertarget.hpp"

/**
 * @file
 * @brief The @c class Texture.
 * This file contains the @c class Texture.
 */
namespace eternal_lands
{

	/**
	 * Types of texture filter modes.
	 * @{
	 */
	enum TextureFilterType
	{
		tft_nearest = GL_NEAREST,
		tft_linear = GL_LINEAR
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture mipmap modes.
	 * @{
	 */
	enum TextureMipmapType
	{
		tmt_none = 0,
		tmt_nearest = 1,
		tmt_linear = 2
	};
	/**
	 * @}
	 */

	/**
	 * Types of texture wrapping modes.
	 * @{
	 */
	enum TextureWrapType
	{
		twt_clamp = GL_CLAMP_TO_EDGE,
		twt_mirrored_repeat = GL_MIRRORED_REPEAT,
		twt_repeat = GL_REPEAT
	};
	/**
	 * @}
	 */

	/**
	 * @brief @c class for textures.
	 *
	 * @c class for textures.
	 */
	class Texture: public AbstractRenderTarget
	{
		private:
			GLfloat m_anisotropic_filter;
			GLuint m_texture_id;
			Uint32 m_depth;
			Uint32 m_size;
			TextureTargetType m_target;
			TextureFilterType m_mag_filter;
			TextureFilterType m_min_filter;
			TextureMipmapType m_mipmap_filter;
			TextureWrapType m_wrap_s;
			TextureWrapType m_wrap_t;
			TextureWrapType m_wrap_r;
			Uint16 m_mipmap_count;
			Uint16 m_used_mipmap_count;
			bool m_rebuild;

			void set_texture_image_1d(const Uint32 width,
				const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_2d(const Uint32 width,
				const Uint32 height, const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_3d(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map(const Uint32 width,
				const Uint32 height, const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map_face(const Uint32 width,
				const Uint32 height, const Uint16 mipmap,
				const CubeMapFaceType face,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_cube_map_face(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap, const CubeMapFaceType face,
				const ImageSharedPtr &image);
			void set_texture_image_2d_layer(const Uint32 width,
				const Uint32 height, const Uint32 layer,
				const Uint16 mipmap,
				const ImageSharedPtr &image);
			void set_texture_image_1d(const String &name,
				const void* buffer, const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const Uint64 buffer_size, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void set_texture_image_2d(const String &name,
				const void* buffer, const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const Uint64 buffer_size, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void set_texture_image_3d(const String &name,
				const void* buffer, const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const Uint64 buffer_size, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void set_texture_image_cube_map_face(const String &name,
				const void* buffer, const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const Uint64 buffer_size,
				const CubeMapFaceType face, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void set_texture_image_cube_map_array_face(
				const String &name, const void* buffer,
				const glm::uvec3 &offset,
				const glm::uvec3 &size, const Uint64 buffer_size,
				const CubeMapFaceType face, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void set_texture_image_1d(const Uint32 width,
				const Uint16 mipmap);
			void set_texture_image_2d(const Uint32 width,
				const Uint32 height, const Uint16 mipmap);
			void set_texture_image_3d(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap);
			void set_texture_image_cube_map(const Uint32 width,
				const Uint32 height, const Uint16 mipmap);
			void set_texture_image_cube_map_face(const Uint32 width,
				const Uint32 height, const Uint16 mipmap,
				const CubeMapFaceType face);
			void set_texture_image_cube_map(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap);
			void set_texture_image_cube_map_face(const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap,
				const CubeMapFaceType face);
			void set_texture_image_2d_multisample();
			void set_texture_image_3d_multisample();
			void get_image_size(const ImageSharedPtr &image,
				Uint32 &width, Uint32 &height, Uint32 &depth,
				Uint32 &mipmap_count, Uint32 &layer_count)
				const;
			static GLenum get_min_filter(
				const TextureFilterType texture_filter,
				const TextureMipmapType texture_mipmap);
			void set_texture_filter();
			void set_texture_wrap();
			void set_texture_parameter();
			void build_texture_id();
			void calc_size();
			void do_sub_texture(const String &name,
				const void* buffer, const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const Uint64 buffer_size,
				const CubeMapFaceType face, const GLenum format,
				const GLenum type, const Uint16 mipmap,
				const bool compressed);
			void do_set_image(const ImageSharedPtr &image);
			void do_set_images(const ImageSharedPtrVector &images);
			void do_init(const Uint32 width, const Uint32 height,
				const Uint32 depth, const Uint16 mipmap_count,
				const Uint16 samples = 0);
			void do_attach_ext(const GLenum attachment,
				const Uint32 level, const Uint32 layer);
			void do_attach(const GLenum attachment,
				const Uint32 level, const Uint32 layer);
			void do_attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer);

			inline bool get_use_mipmaps() const
			{
				return get_used_mipmap_count() > 0;
			}

			inline Uint16 get_used_mipmap_count() const
			{
				return m_used_mipmap_count;
			}

			inline void set_depth(const Uint32 depth)
			{
				m_depth = depth;
			}

			inline void set_target(const TextureTargetType value)
				noexcept
			{
				m_target = value;
			}

			inline void set_mipmap_count(const Uint16 value)
				noexcept
			{
				m_mipmap_count = value;
			}

			inline GLuint get_texture_id() const
			{
				return m_texture_id;
			}

		public:
			Texture(const String &name, const Uint32 width,
				const Uint32 height, const Uint32 depth,
				const Uint16 mipmap_count, const Uint16 samples,
				const TextureFormatType format,
				const TextureTargetType target);
			~Texture() noexcept;
			void unload() noexcept;
			static String get_str(const TextureFilterType value);
			static String get_str(const TextureMipmapType value);
			static String get_str(const TextureWrapType value);

			/**
			 * @brief Gets the number of faces.
			 *
			 * Returns the number of faces. This is 6 for cube maps
			 * and 1 for all other image types.
			 * @return The number of faces.
			 */
			inline Uint16 get_face_count() const
			{
				return get_cube_map() ? 6 : 1;
			}

			/**
			 * Returns true if it's a cube map image, false else.
			 * @return True for cube map, false else.
			 */
			inline bool get_cube_map() const
			{
				return (get_target() ==
					ttt_texture_cube_map_array) ||
					(get_target() == ttt_texture_cube_map);
			}

			inline void bind()
			{
				assert(!m_rebuild);

				glBindTexture(get_gl_target(),
					get_texture_id());
			}

			inline Uint64 get_size() const noexcept
			{
				return m_size;
			}

			inline TextureTargetType get_target() const noexcept
			{
				return m_target;
			}

			inline GLenum get_gl_target() const noexcept
			{
				return TextureTargetUtil::get_gl_type(
					get_target());
			}

			inline TextureFilterType get_mag_filter() const noexcept
			{
				return m_mag_filter;
			}

			inline TextureFilterType get_min_filter() const noexcept
			{
				return m_min_filter;
			}

			inline TextureMipmapType get_mipmap_filter() const
				noexcept
			{
				return m_mipmap_filter;
			}

			inline GLfloat get_anisotropic_filter() const noexcept
			{
				return m_anisotropic_filter;
			}

			inline Uint16 get_mipmap_count() const noexcept
			{
				return m_mipmap_count;
			}

			inline TextureWrapType get_wrap_s() const noexcept
			{
				return m_wrap_s;
			}

			inline TextureWrapType get_wrap_t() const noexcept
			{
				return m_wrap_t;
			}

			inline TextureWrapType get_wrap_r() const noexcept
			{
				return m_wrap_r;
			}

			inline Uint32 get_depth() const noexcept
			{
				return m_depth;
			}

			inline GLenum get_gl_format() const noexcept
			{
				return TextureFormatUtil::get_gl_format(
					get_format());
			}

			inline void set_mag_filter(
				const TextureFilterType value) noexcept
			{
				m_mag_filter = value;
			}

			inline void set_min_filter(
				const TextureFilterType value) noexcept
			{
				m_min_filter = value;
			}

			inline void set_mipmap_filter(
				const TextureMipmapType value) noexcept
			{
				m_mipmap_filter = value;
			}

			inline void set_anisotropic_filter(const GLfloat value)
				noexcept
			{
				m_anisotropic_filter = value;
			}

			inline void set_wrap_s(const TextureWrapType value)
				noexcept
			{
				m_wrap_s = value;
			}

			inline void set_wrap_t(const TextureWrapType value)
				noexcept
			{
				m_wrap_t = value;
			}

			inline void set_wrap_r(const TextureWrapType value)
				noexcept
			{
				m_wrap_r = value;
			}

			static inline void unbind() noexcept
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			void set_image(const ImageSharedPtr &image);
			void set_images(const ImageSharedPtrVector &images);
			void sub_texture(const String &name,
				const AbstractReadMemorySharedPtr &buffer,
				const glm::uvec3 &offset,
				const glm::uvec3 &size,
				const CubeMapFaceType face,
				const GLenum format, const GLenum type,
				const Uint16 mipmap, const bool compressed);
			void sub_texture(const ImageUpdate &image_update);
			void update_image(const ImageSharedPtr &image,
				const BitSet16 &faces =
					std::numeric_limits<Uint16>::max(),
				const BitSet16 &mipmaps =
					std::numeric_limits<Uint16>::max());
			void update_image_layer(const ImageSharedPtr &image,
				const BitSet16 &faces, const BitSet16 &mipmaps,
				const Uint16 layer);
			void init(const Uint32 width, const Uint32 height,
				const Uint32 depth, const Uint16 mipmap_count,
				const Uint16 samples = 0);
			void attach_ext(const GLenum attachment,
				const Uint32 level, const Uint32 layer);
			void attach(const GLenum attachment, const Uint32 level,
				const Uint32 layer);
			void attach(const GLenum attachment,
				const CubeMapFaceType face, const Uint32 level,
				const Uint32 layer);

			inline void change_format(const TextureFormatType value)
				noexcept
			{
				m_rebuild = true;
				set_format(value);
			}


	};

	OutStream &operator<<(OutStream &str, const TextureFilterType value);
	OutStream &operator<<(OutStream &str, const TextureMipmapType value);
	OutStream &operator<<(OutStream &str, const TextureWrapType value);

}

#endif	/* UUID_f8a7405b_ced9_4d49_9ad0_cf51cc1a3dfd */
