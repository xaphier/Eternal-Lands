/****************************************************************************
 *            utf.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const std::string &to_charset, const char* input,
			const size_t size)
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

			inleft = size;
			inbuf = input;

			/* we'll start off allocating an output buffer which is
			 * the same size as our input buffer, but at least 128
			 * bytes big. */
			outlen = std::max(inleft, static_cast<size_t>(128));

			output.reset(new char[outlen]);

			converted = 0;

			while (true)
			{
				errno = 0;
				outbuf = output.get();
				outleft = outlen;

#ifdef	ICONV_SECOND_ARGUMENT_IS_CONST
				converted = iconv(cd, (const char **)&inbuf,
					&inleft, &outbuf, &outleft);
#else	/* ICONV_SECOND_ARGUMENT_IS_CONST */
				converted = iconv(cd, (char **)&inbuf, &inleft,
					&outbuf, &outleft);
#endif	/* ICONV_SECOND_ARGUMENT_IS_CONST */

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
		return convert("UTF-8", "", str.c_str(), str.length());
	}

	std::string string_to_utf8(const std::string &str)
	{
		return convert("", "UTF-8", str.c_str(), str.length());
	}

	Utf32String utf8_to_utf32(const std::string &str)
	{
		std::string tmp;

		tmp = convert("UTF-8", "UTF-32", str.c_str(), str.length());

		return Utf32String(reinterpret_cast<const Utf32Char*>(
			tmp.c_str()), tmp.size() / 4);
	}

	std::string utf32_to_utf8(const Utf32String &str)
	{
		return convert("UTF-32", "UTF-8",
			reinterpret_cast<const char*>(str.c_str()),
			str.length() * 4);
	}

	std::wstring utf8_to_wstring(const std::string &str)
	{
		std::string tmp;

		tmp = convert("UTF-8", "UTF-32", str.c_str(), str.length());

		return std::wstring(reinterpret_cast<const wchar_t*>(
			tmp.c_str()), tmp.size() / 4);
	}

	std::string wstring_to_utf8(const std::wstring &str)
	{
		return convert("UTF-32", "UTF-8",
			reinterpret_cast<const char*>(str.c_str()),
			str.length() * 4);
	}

}
