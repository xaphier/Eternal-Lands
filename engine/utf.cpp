/****************************************************************************
 *            utf.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "utf.hpp"
#include "exceptions.hpp"
#include <iconv.h>

namespace eternal_lands
{

	namespace
	{

		std::string convert(const std::string &from_charset,
			const std::string &to_charset, const std::string &input)
		{
			std::string result;
			boost::scoped_array<char> output;
			size_t inleft, outleft, converted;
			char *outbuf;
			const char *inbuf;
			size_t outlen;
			iconv_t cd;

			cd = iconv_open(to_charset.c_str(),
				from_charset.c_str());

			if (cd == (iconv_t)-1)
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("no conversion"
					" from charset '%1%' to charset '%2%' "
					"possible."), from_charset % to_charset,
					NotImplementedException());
			}

			inleft = input.length();
			inbuf = input.c_str();

			/* we'll start off allocating an output buffer which is
			 * the same size as our input buffer. */
			outlen = inleft;

			output.reset(new char[outlen]);

			converted = 0;

			while (true)
			{
				errno = 0;
				outbuf = output.get();
				outleft = outlen;

				converted = iconv(cd, (char **)&inbuf, &inleft,
					&outbuf, &outleft);

				if ((converted != (size_t)-1) ||
					(errno == EINVAL))
				{
					/*
					 * EINVAL An incomplete multibyte
					 * sequence has been encoun­tered in the
					 * input. We'll just truncate it and
					 * ignore it.
					 */
					converted = outbuf - output.get();

					result.append(output.get(), converted);

					break;
				}

				if (errno != E2BIG)
				{
					/*
					 * EILSEQ An invalid multibyte sequence
					 * has been encountered in the input.
					 * Bad input, we can't really recover
					 * from this.
					 */
					iconv_close(cd);

					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< errinfo_parameter_name(UTF8(
							"input")));
				}

				/*
				 * E2BIG There is not sufficient room at
				 * *outbuf. We just need to grow our outbuffer
				 * and try again.
				 */
				converted = outbuf - output.get();

				result.append(output.get(), converted);
			}

			outleft = outlen;
			outbuf = output.get();

			/* flush the iconv conversion */
			iconv(cd, 0, 0, &outbuf, &outleft);
			iconv_close(cd);

			converted = outbuf - output.get();

			result.append(output.get(), converted);

			return result;
		}

	}

	std::string utf8_to_string(const std::string &str)
	{
		return convert("UTF-8", "", str);
	}

	std::string string_to_utf8(const std::string &str)
	{
		return convert("", "UTF-8", str);
	}

	std::wstring utf8_to_utf32(const std::string &str)
	{
		std::string tmp;

		tmp = convert("UTF-8", "UTF-32", str);

		return std::wstring(reinterpret_cast<const wchar_t*>(
			tmp.c_str()), tmp.size() / 4);
	}

}
