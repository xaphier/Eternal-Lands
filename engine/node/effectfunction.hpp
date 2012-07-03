/****************************************************************************
 *            effectfunction.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e
#define	UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"
#include "effectfunctionutil.hpp"

/**
 * @file
 * @brief The @c class EffectFunction.
 * This file contains the @c class EffectFunction.
 */
namespace eternal_lands
{

	class EffectFunction: public EffectNode
	{
		private:
			EffectFunctionType m_type;

		public:
			EffectFunction(const String &name,
				const EffectFunctionType type,
				Mt19937RandomUuidGenerator &generator);
			virtual ~EffectFunction() noexcept;
			virtual void write(OutStream &str) const;

			inline EffectFunctionType get_type() const noexcept
			{
				return m_type;
			}

	};

}

#endif	/* UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e */
