/****************************************************************************
 *            colorcorrection.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ddc2a652_ece7_416f_a314_ec84ea6fb491
#define	UUID_ddc2a652_ece7_416f_a314_ec84ea6fb491

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ColorCorrection.
 * This file contains the @c class ColorCorrection.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for color correction.
	 *
	 * @c class for color correction.
	 */
	class ColorCorrection: public boost::noncopyable
	{
		private:
			const ScriptEngineWeakPtr m_script_engine;
			asIScriptContext* m_context;

			inline ScriptEngineSharedPtr get_script_engine() const
			{
				ScriptEngineSharedPtr result;

				result = m_script_engine.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			ColorCorrection(
				const ScriptEngineSharedPtr &script_engine);

			/**
			 * Default destructor.
			 */
			~ColorCorrection() noexcept;

			TextureSharedPtr get_color_correction(
				const String name);

			static inline Uint16 get_lut_size() noexcept
			{
				return 64;
			}

	};

}

#endif	/* UUID_ddc2a652_ece7_416f_a314_ec84ea6fb491 */
