/****************************************************************************
 *            hardwarebuffermapper.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarebuffermapper.hpp"
#include "exceptions.hpp"
#include "abstractwritememory.hpp"
#include "hardwarewritememory.hpp"
#include "mappedhardwarewritememory.hpp"
#include "opengl3mappedhardwarewritememory.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	HardwareBufferMapper::HardwareBufferMapper(
		const GlobalVarsConstSharedPtr &global_vars):
		m_global_vars(global_vars)
	{
	}

	HardwareBufferMapper::~HardwareBufferMapper() noexcept
	{
	}

	AbstractWriteMemorySharedPtr
		HardwareBufferMapper::write_map_hardware_buffer(
			const HardwareBufferSharedPtr &buffer) const
	{
		if (buffer->get_size() == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("buffer size is zero"))
				<< errinfo_parameter_name(UTF8("buffer")));
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			return boost::make_shared<Opengl3HardwareWriteMemory>(
				buffer, btt_copy_write);
		}
/*
		if (get_global_vars()->get_opengl_es())
		{
			return boost::make_shared<HardwareWriteMemory>(buffer,
				btt_vertex);
		}
*/
		return boost::make_shared<HardwareWriteMemory>(buffer,
			btt_vertex);
	}

}
