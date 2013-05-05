/****************************************************************************
 *            hardwarebuffermapper.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7ad657d8_0e4e_47ee_b135_1a3b98c1c1e2
#define	UUID_7ad657d8_0e4e_47ee_b135_1a3b98c1c1e2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class HardwareBufferMapper.
 * This file contains the @c class HardwareBufferMapper.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for opengl hardware buffers.
	 *
	 * @c class for opengl hardware buffers.
	 */
	class HardwareBufferMapper: public boost::noncopyable
	{
		private:
			const GlobalVarsConstSharedPtr m_global_vars;

			inline const GlobalVarsConstSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

		public:
			/**
			 * Default constructor.
			 */
			HardwareBufferMapper(
				const GlobalVarsConstSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			~HardwareBufferMapper() noexcept;

			AbstractWriteMemorySharedPtr write_map_hardware_buffer(
				const HardwareBufferSharedPtr &buffer) const;

	};

}

#endif	/* UUID_7ad657d8_0e4e_47ee_b135_1a3b98c1c1e2 */
