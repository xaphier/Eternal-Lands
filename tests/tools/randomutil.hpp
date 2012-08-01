/****************************************************************************
 *            randomutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bce26840_f8b1_4916_b33a_77b7e980a033
#define	UUID_bce26840_f8b1_4916_b33a_77b7e980a033

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include <boost/random.hpp>

#define EL_CHECK_EQUAL_VEC_MAT(p0, p1)	\
	BOOST_CHECK_EQUAL_COLLECTIONS(glm::value_ptr(p0),	\
		glm::value_ptr(p0) + sizeof(p0) / sizeof(*glm::value_ptr(p0)),	\
		glm::value_ptr(p1),	\
		glm::value_ptr(p1) + sizeof(p1) / sizeof(*glm::value_ptr(p1)))

namespace eternal_lands
{

	class RandomUtil
	{
		private:
			boost::mt19937 m_rng;
			boost::uniform_int<Uint32> m_name_range;
			boost::uniform_real<float> m_float_range;
			boost::uniform_int<Sint32> m_int_range;
			boost::variate_generator<boost::mt19937&,
				boost::uniform_int<Uint32> > m_random_name;
			boost::variate_generator<boost::mt19937&,
				boost::uniform_real<float> > m_random_float;
			boost::variate_generator<boost::mt19937&,
				boost::uniform_int<Sint32> > m_random_int;

		public:
			inline RandomUtil(): m_name_range(0, 61),
				m_float_range(-1000.0f, 1000.0f),
				m_int_range(-1000000, 1000000),
				m_random_name(m_rng, m_name_range),
				m_random_float(m_rng, m_float_range),
				m_random_int(m_rng, m_int_range)
			{
			}

			inline ~RandomUtil() noexcept
			{
			}

			inline String get_random_name()
			{
				StringStream str;
				Uint32 i, n;
				char c;

				for (i = 0; i < 64; ++i)
				{
					n = m_random_name();

					if (n < 26)
					{
						c = 'a' + n;
					}
					else
					{
						if (n < 52)
						{
							c = 'A' + n - 26;
						}
						else
						{
							c = '0' + n - 52;
						}
					}

					str << c;
				}

				return String(str.str());
			}

			inline float get_random_float()
			{
				return m_random_float();
			}

			inline float get_random_int_float()
			{
				return m_random_int();
			}

	};

}

#endif	/* UUID_bce26840_f8b1_4916_b33a_77b7e980a033 */
