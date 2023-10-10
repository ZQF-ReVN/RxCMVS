#include "MBCS_Cvt.h"

#include <cstdint>
#include <fstream>


namespace Rut::RxCvt
{
	typedef uint16_t(__cdecl* pToMbcsChar)(uint16_t usUnicodeChar);
	typedef wchar_t(__cdecl* pToUnicodeChar)(uint16_t usMultibyteChar);


	static uint16_t* ReadTable(const char* cpTable)
	{
		uint16_t* table_ptr = new uint16_t[0x10000u];
		std::ifstream ifs{ cpTable, std::ios::binary };
		if (!ifs) { throw std::runtime_error("RxCvt Read Table Error!"); }
		ifs.read((char*)table_ptr, static_cast<std::streamsize>(0x10000u) * 2);
		return table_ptr;
	}


	static uint16_t UnicodeCharToGBK(uint16_t usUnicodeChar)
	{
		static uint16_t* u16le2gbk_table = nullptr;
		if (!u16le2gbk_table) { u16le2gbk_table = ReadTable("u16le_to_gbk_table.bin"); }

		return (wchar_t)u16le2gbk_table[usUnicodeChar];
	}

	static uint16_t UnicodeCharToSJIS(uint16_t usUnicodeChar)
	{
		static uint16_t* u16le2sjis_table = nullptr;
		if (!u16le2sjis_table) { u16le2sjis_table = ReadTable("u16le_to_sjis_table.bin"); }

		return (wchar_t)u16le2sjis_table[usUnicodeChar];
	}

	static wchar_t GBKToUnicodeChar(uint16_t usGBKChar)
	{
		static uint16_t* gbk2u16le_table = nullptr;
		if (!gbk2u16le_table) { gbk2u16le_table = ReadTable("gbk_to_u16le_table.bin"); }

		return (wchar_t)gbk2u16le_table[usGBKChar];
	}

	static wchar_t SJISToUnicodeChar(uint16_t usSJISChar)
	{
		static uint16_t* sjis2u16le_table = nullptr;
		if (!sjis2u16le_table) { sjis2u16le_table = ReadTable("sjis_to_u16le_table.bin"); }

		return (wchar_t)sjis2u16le_table[usSJISChar];
	}
	

	static pToUnicodeChar GetToUnicodeCharFunc(size_t nCodePage)
	{
		switch (nCodePage)
		{
		case 932: return SJISToUnicodeChar; break;
		case 936: return GBKToUnicodeChar; break;
		default: throw std::runtime_error("CodePage Error!");
		}
	}

	static pToMbcsChar GetToMbcsCharFunc(size_t nCodePage)
	{
		switch (nCodePage)
		{
		case 932: return UnicodeCharToSJIS; break;
		case 936: return UnicodeCharToGBK; break;
		default: throw std::runtime_error("CodePage Error!");
		}
	}


	static size_t GetMbcsStrToUnicodeStrCharCount(const char* cpStr)
	{
		size_t ite_byte = 0;
		size_t char_count = 0;
		while (true)
		{
			uint8_t tmp_char = (uint8_t)cpStr[ite_byte++];
			if (tmp_char == '\0') { break; }

			if (tmp_char >= 0x81u)
			{
				ite_byte++;
				char_count++;
			}
			else
			{
				char_count++;
			}
		}

		return char_count;
	}

	static size_t CvtMbcsStrToUnicodeStr(const char* cpStr, wchar_t* wpStr, pToUnicodeChar fnToUnicodeChar)
	{
		size_t ite_wide = 0;
		size_t ite_byte = 0;
		while (true)
		{
			uint16_t dbcs = (uint8_t)cpStr[ite_byte++];
			if (dbcs == '\0') { break; }
			if (dbcs >= 0x81u) { dbcs = (dbcs << 0x8) | ((uint8_t)cpStr[ite_byte++]); }
			uint16_t code = fnToUnicodeChar(dbcs);
			if (code == 0) { return 0; }
			wpStr[ite_wide++] = code;
		}
		return ite_wide;
	}

	size_t MbcsStrToUnicodeStr(const char* cpStr, wchar_t* wpStr, size_t nCodePage)
	{
		if (wpStr)
		{
			return CvtMbcsStrToUnicodeStr(cpStr, wpStr, GetToUnicodeCharFunc(nCodePage));
		}
		else
		{
			return GetMbcsStrToUnicodeStrCharCount(cpStr);
		}
	}


	static size_t GetUnicodeStrToMbcsStrBytes(const wchar_t* wpStr)
	{
		size_t ite_byte = 0;
		size_t count_size = 0;
		while (true)
		{
			wchar_t tmp_char = wpStr[ite_byte++];
			if (tmp_char == L'\0') { break; }

			if (tmp_char <= 0x7Fu)
			{
				ite_byte++;
				count_size++;
			}
			else
			{
				count_size += 2;
			}
		}

		return count_size;
	}

	static size_t CvtUnicodeStrToMbcsStr(const wchar_t* wpStr, char* cpStr, pToMbcsChar fnToMbcsChar, bool* isError, const char* cpDefault)
	{
		size_t ite_byte = 0;
		size_t ite_wide = 0;
		while (true)
		{
			uint16_t wchar = (uint16_t)wpStr[ite_wide++];
			if (wchar == L'\0') { break; }

			uint16_t code = fnToMbcsChar(wchar);
			if (code == 0)
			{
				cpStr[ite_byte++] = cpDefault[0];
				if (isError)
				{
					*isError = true;
				}
				continue;
			}

			if (code <= 0x7F)
			{
				cpStr[ite_byte++] = (uint8_t)code;
			}
			else
			{
				cpStr[ite_byte++] = (uint8_t)(code >> 0);
				cpStr[ite_byte++] = (uint8_t)(code >> 8);
			}
		}

		return ite_byte;
	}

	size_t UnicodeStrToMbcsStr(const wchar_t* wpStr, char* cpStr, size_t nCodePage, bool* isError, const char* cpDefault)
	{
		if (cpStr)
		{
			return CvtUnicodeStrToMbcsStr(wpStr, cpStr, GetToMbcsCharFunc(nCodePage), isError, cpDefault);
		}
		else
		{
			return GetUnicodeStrToMbcsStrBytes(wpStr);
		}
	}
}