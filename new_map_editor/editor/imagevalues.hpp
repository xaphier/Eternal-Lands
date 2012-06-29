/****************************************************************************
 *            imagevalues.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0718c037_a791_43bb_a030_4271c16870ba
#define	UUID_0718c037_a791_43bb_a030_4271c16870ba

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "imagevalue.hpp"

namespace eternal_lands
{

	class ImageValues
	{
		private:
			ReadWriteMemorySharedPtr m_buffer;

		public:
			ImageValues();
			~ImageValues() noexcept;
			void pack(const ImageValueVector &image_values);
			ImageValueVector unpack() const;

	};

}

#endif	/* UUID_0718c037_a791_43bb_a030_4271c16870ba */
