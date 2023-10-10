#include "UTF8_Cvt.h"

#include <string>
#include <cassert>


namespace Rut::RxCvt
{
	static uint32_t GetUnicode(const wchar_t* wpStr, size_t& iteStr)
	{
		uint32_t unicode = 0;

		// UTF16 To Unicode
		if (sizeof(wchar_t) == 2)
		{
			uint32_t u16_h = wpStr[iteStr++];
			if (u16_h == 0) { return 0; }
			if (unicode >= 0xD800 && unicode <= 0xDBFF) // Surrogate pair
			{
				// codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)
				uint32_t u16_l = wpStr[iteStr++];
				unicode = 0x10000u + (((u16_h - 0xD800U) << 10) | (u16_l - 0xDC00U));
			}
			else
			{
				unicode = u16_h;
			}
		}
		else
		{
			unicode = wpStr[iteStr++];
			if (unicode == 0) { return 0; }
		}

		return unicode;
	}

	static size_t GetEncodeU8Size(const wchar_t* wpStr)
	{
		size_t ite_uc = 0;
		size_t ite_u8 = 0;

		while (true)
		{
			uint32_t unicode = GetUnicode(wpStr, ite_uc);
			if (unicode == 0) { break; }

			if (unicode <= 0x7Fu) // 1 bytes
			{
				ite_u8++;
			}
			else if (unicode <= 0x7FFu)  // 2 bytes
			{
				ite_u8 += 2;
			}
			else if (unicode <= 0xFFFF) // 3 bytes
			{
				ite_u8 += 3;
			}
			else
			{
				assert(unicode <= 0x10FFFF); // 4 bytes 
				ite_u8 += 4;
			}
		}

		return ite_u8;
	}

	static size_t EncodeU8Str(const wchar_t* wpStr, char* u8Str)
	{
		size_t ite_uc = 0;
		size_t ite_u8 = 0;

		while (true)
		{
			uint32_t unicode = GetUnicode(wpStr, ite_uc);
			if (unicode == 0) { break; }

			if (unicode <= 0x7Fu) // 1 bytes
			{
				u8Str[ite_u8++] = (unicode & 0xFFu);
			}
			else if (unicode <= 0x7FFu)  // 2 bytes
			{
				u8Str[ite_u8++] = (0xC0u | ((unicode >> 6u) & 0xFFu));
				u8Str[ite_u8++] = (0x80u | (unicode & 0x3Fu));
			}
			else if (unicode <= 0xFFFF) // 3 bytes
			{
				u8Str[ite_u8++] = (0xE0u | ((unicode >> 12u) & 0xFFu));
				u8Str[ite_u8++] = (0x80u | ((unicode >> 6u) & 0x3Fu));
				u8Str[ite_u8++] = (0x80u | (unicode & 0x3Fu));
			}
			else
			{
				assert(unicode <= 0x10FFFF); // 4 bytes 
				u8Str[ite_u8++] = (0xF0u | ((unicode >> 18u) & 0xFFu));
				u8Str[ite_u8++] = (0x80u | ((unicode >> 12u) & 0x3Fu));
				u8Str[ite_u8++] = (0x80u | ((unicode >> 6u) & 0x3Fu));
				u8Str[ite_u8++] = (0x80u | (unicode & 0x3Fu));
			}
		}

		return ite_u8;
	}

	size_t WideStrToU8Str(const wchar_t* wpStr, char* u8Str)
	{
		if (u8Str)
		{
			return EncodeU8Str(wpStr, u8Str);
		}
		else
		{
			return GetEncodeU8Size(wpStr);
		}
	}

	static size_t GetDecodeU8Size(const char* u8Str)
	{
		return 0;
	}

	static size_t DecodeU8Str(const char* u8Str, wchar_t* wpStr)
	{
		return 0;
	}

	size_t U8StrToWideStr(const char* u8Str, wchar_t* wpStr)
	{
		if (wpStr)
		{
			return DecodeU8Str(u8Str, wpStr);
		}
		else
		{
			return GetDecodeU8Size(u8Str);
		}
	}
}