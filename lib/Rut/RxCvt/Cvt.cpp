#include "Cvt.h"

#include <stdexcept>


namespace Rut::RxCvt
{
	size_t StrToWStr(const char* cpStr, size_t msStrbytes, wchar_t* wpStr, size_t wsStrLen, size_t nCodePage);
	size_t WStrToStr(const wchar_t* wpStr, size_t wsStrLen, char* cpStr, size_t msStrbytes, size_t nCodePage, bool* isError = nullptr, const char* cpDefault = "?");


	std::wstring ToWCS(const std::string_view msStr, size_t uCodePage)
	{
		std::wstring wstr;
		ToWCS(msStr, wstr, uCodePage);
		return wstr;
	}

	std::string ToMBCS(const std::wstring_view wsStr, size_t uCodePage)
	{
		std::string mstr;
		ToMBCS(wsStr, mstr, uCodePage);
		return mstr;
	}

	size_t ToWCS(const std::string_view msStr, std::wstring& wsStr, size_t uCodePage)
	{
		size_t mstr_len = msStr.size();
		if (mstr_len == 0) { wsStr.clear(); return 0; }

		const char* mstr_ptr = msStr.data();
		size_t wstr_len = StrToWStr(mstr_ptr, mstr_len, nullptr, 0, uCodePage);
		if (wstr_len == 0) { throw std::runtime_error("RxCvt::ToWCS Error!"); }

		wsStr.resize(wstr_len);
		wstr_len = StrToWStr(mstr_ptr, mstr_len, wsStr.data(), wstr_len, uCodePage);
		if (wstr_len == 0) { throw std::runtime_error("RxCvt::ToWCS Error!"); }

		return wstr_len;
	}

	size_t ToMBCS(const std::wstring_view wsStr, std::string& msStr, size_t uCodePage)
	{
		size_t wstr_len = wsStr.length();
		if (wstr_len == 0) { msStr.clear(); return 0; }

		const wchar_t* wstr_ptr = wsStr.data();
		size_t mstr_bytes = WStrToStr(wstr_ptr, wstr_len, nullptr, 0, uCodePage, nullptr, nullptr);
		if (mstr_bytes == 0) { throw std::runtime_error("RxCvt::ToMBCS Error!"); }

		bool is_error = false;

		msStr.resize(mstr_bytes);
		mstr_bytes = WStrToStr(wstr_ptr, wstr_len, msStr.data(), mstr_bytes, uCodePage, &is_error, "?");
		if (is_error || (mstr_bytes == 0) ) { throw std::runtime_error("RxCvt::ToMBCS Error!"); }

		return mstr_bytes;
	}
}


#ifdef WIN32
#include <Windows.h>


namespace Rut::RxCvt
{
	static size_t StrToWStr(const char* cpStr, size_t msStrbytes, wchar_t* wpStr, size_t wsStrLen, size_t nCodePage)
	{
		return MultiByteToWideChar((UINT)nCodePage, MB_ERR_INVALID_CHARS, cpStr, (int)msStrbytes, wpStr, (int)wsStrLen);
	}

	static size_t WStrToStr(const wchar_t* wpStr, size_t wsStrLen, char* cpStr, size_t msStrbytes, size_t nCodePage, bool* isError, const char* cpDefault)
	{
		BOOL not_all_cvt = TRUE;
		size_t cch = WideCharToMultiByte((UINT)nCodePage, 0, wpStr, (int)wsStrLen, cpStr, (int)msStrbytes, cpDefault, &not_all_cvt);
		if (isError) { *isError = not_all_cvt; }
		return cch;
	}
}

#else
#include "MBCS_Cvt.h"
#include "UTF8_Cvt.h"


namespace Rut::RxCvt
{
	// IF wpStr == nullptr return Wide Char count
	static size_t StrToWStr(const char* cpStr, size_t msStrbytes, wchar_t* wpStr, size_t wsStrCCH, size_t nCodePage)
	{
		return MbcsStrToUnicodeStr(cpStr, wpStr, nCodePage);
	}

	// IF cpStr == nullptr return Multibyte String bytes
	static size_t WStrToStr(const wchar_t* wpStr, size_t wsStrCCH, char* cpStr, size_t msStrbytes, size_t nCodePage, bool* isError, const char* cpDefault)
	{
		if (nCodePage == 65001)
		{
			return WideStrToU8Str(wpStr,cpStr);
		}
		else
		{
			return UnicodeStrToMbcsStr(wpStr, cpStr, nCodePage, isError, cpDefault);
		}
	}
}

#endif // WIN32


#ifdef _RXCVT_MAKE_TABLE_

#include <Windows.h>


namespace Rut::RxCvt
{
	class CharsetTableMaker
	{
	public:
		CharsetTableMaker()
		{

		}

		void GBK2UTF16()
		{
			constexpr size_t table_codes = 0x10000;
			constexpr size_t table_bytes = 0x10000 * 2;

			uint16_t* table_gbk_ptr = new uint16_t[table_codes];
			uint16_t* table_u16_ptr = new uint16_t[table_codes];
			memset(table_gbk_ptr, 0x0, table_bytes);
			memset(table_u16_ptr, 0x0, table_bytes);

			// Make GBK to UTF-16 Table
			{
				for (uint8_t ite_ascii = 0x0; ite_ascii <= 0x7F; ite_ascii++) // ASCII Table
				{
					table_gbk_ptr[ite_ascii] = this->ToWideChar(ite_ascii, 936);
				}

				for (uint8_t gbk_first = 0x81; gbk_first <= 0xFE; gbk_first++) // GBK Table
				{
					for (uint8_t gbk_second = 0x40; gbk_second <= 0xFE; gbk_second++)
					{
						uint16_t gbk_pos = (gbk_first << 8) | (gbk_second);
						uint16_t gbk_code = (gbk_first) | (gbk_second << 8);
						uint16_t u16_code = this->ToWideChar(gbk_code, 936);
						table_gbk_ptr[gbk_pos] = u16_code;
					}
				}
			}

			// Make UTF-16 to GBK Table
			{
				for (uint16_t ite_ascii = 0x0; ite_ascii <= 0x7F; ite_ascii++) // ASCII Table
				{
					table_u16_ptr[ite_ascii] = ite_ascii;
				}

				for (uint16_t ite_char = 0x8140u; ite_char < 0xFFFF; ite_char++)
				{
					uint16_t gbk_pos = ite_char;
					uint16_t gbk_code = (ite_char >> 0x8) | (ite_char << 0x8);
					uint16_t u16_code = table_gbk_ptr[gbk_pos];
					if (u16_code)
					{
						table_u16_ptr[u16_code] = gbk_code;
					}
				}
			}

			// Test
			{
				uint16_t u16_c = L'我';
				uint16_t ret_gbk_c = table_u16_ptr[u16_c];
				char gbk_buffer[3] = { 0 };
				gbk_buffer[0] = ret_gbk_c & 0x00FF;
				gbk_buffer[1] = ret_gbk_c >> 8;
				std::cout << gbk_buffer;

				uint16_t gbk_c = '我';
				wchar_t ret_u16_c = table_gbk_ptr[gbk_c];
			}

			// Save Table
			std::ofstream("gbk_to_u16le_table.bin", std::ios::binary).write((char*)table_gbk_ptr, table_bytes);
			std::ofstream("u16le_to_gbk_table.bin", std::ios::binary).write((char*)table_u16_ptr, table_bytes);

			// Delete Buffer
			delete[] table_gbk_ptr;
			delete[] table_u16_ptr;
		}

		void SJIS2UTF16()
		{
			constexpr size_t table_codes = 0x10000;
			constexpr size_t table_bytes = 0x10000 * 2;

			uint16_t* table_sjis_ptr = new uint16_t[table_codes];
			uint16_t* table_u16_ptr = new uint16_t[table_codes];
			memset(table_u16_ptr, 0x0, table_bytes);
			memset(table_sjis_ptr, 0x0, table_bytes);

			// Make SJIS to UTF-16 Table
			{
				for (uint8_t ite_ascii = 0x0; ite_ascii <= 0x7F; ite_ascii++) // ASCII Table
				{
					table_sjis_ptr[ite_ascii] = this->ToWideChar(ite_ascii, 936);
				}

				for (uint8_t sjis_first = 0x81; sjis_first <= 0x9F; sjis_first++) // SJIS Table
				{
					for (uint8_t sjis_second = 0x40; sjis_second <= 0x7E; sjis_second++)
					{
						uint16_t sjis_pos = (sjis_first << 8) | (sjis_second);
						uint16_t sjis_code = (sjis_first) | (sjis_second << 8);
						table_sjis_ptr[sjis_pos] = this->ToWideChar(sjis_code, 932);
					}

					for (uint8_t sjis_second = 0x80; sjis_second <= 0xFC; sjis_second++)
					{
						uint16_t sjis_pos = (sjis_first << 8) | (sjis_second);
						uint16_t sjis_code = (sjis_first) | (sjis_second << 8);
						table_sjis_ptr[sjis_pos] = this->ToWideChar(sjis_code, 932);
					}
				}

				for (uint8_t sjis_first = 0xE0; sjis_first <= 0xEF; sjis_first++) // SJIS Table
				{
					for (uint8_t sjis_second = 0x40; sjis_second <= 0x7E; sjis_second++)
					{
						uint16_t sjis_pos = (sjis_first << 8) | (sjis_second);
						uint16_t sjis_code = (sjis_first) | (sjis_second << 8);
						table_sjis_ptr[sjis_pos] = this->ToWideChar(sjis_code, 932);
					}

					for (uint8_t sjis_second = 0x80; sjis_second <= 0xFC; sjis_second++)
					{
						uint16_t sjis_pos = (sjis_first << 8) | (sjis_second);
						uint16_t sjis_code = (sjis_first) | (sjis_second << 8);
						table_sjis_ptr[sjis_pos] = this->ToWideChar(sjis_code, 932);
					}
				}
			}

			{
				for (uint16_t ite_ascii = 0x0; ite_ascii <= 0x7F; ite_ascii++) // ASCII Table
				{
					table_u16_ptr[ite_ascii] = ite_ascii;
				}

				for (uint16_t ite_char = 0x8140u; ite_char < 0xFFFF; ite_char++)
				{
					uint16_t sjis_pos = ite_char;
					uint16_t sjis_code = (ite_char >> 0x8) | (ite_char << 0x8);
					uint16_t u16_code = table_sjis_ptr[sjis_pos];
					if (u16_code)
					{
						table_u16_ptr[u16_code] = sjis_code;
					}
				}
			}

			std::ofstream("sjis_to_u16le_table.bin", std::ios::binary).write((char*)table_sjis_ptr, table_bytes);
			std::ofstream("u16le_to_sjis_table.bin", std::ios::binary).write((char*)table_u16_ptr, table_bytes);

			delete[] table_sjis_ptr;
			delete[] table_u16_ptr;
		}

		void TableToClangStyle(const wchar_t* wpTablePath, const wchar_t* wpTextPath)
		{
			constexpr size_t table_codes = 0x10000;
			constexpr size_t table_bytes = 0x10000 * 2;

			uint16_t* table_ptr = new uint16_t[table_codes];
			std::ifstream(wpTablePath, std::ios::binary).read((char*)table_ptr, table_bytes);

			std::string text;
			size_t per_line_count = 0xF;
			for (size_t ite_char = 0; ite_char < table_codes; ite_char++)
			{
				char buffer[0xF] = { 0 };
				sprintf_s(buffer, 0xF, "0x%04X, ", table_ptr[ite_char]);
				text.append(buffer);
				if ((ite_char % per_line_count) == 0)
				{
					text.append(1, '\n');
				}
			}
			delete[] table_ptr;

			std::ofstream(wpTextPath) << text;
		}

	private:
		static uint16_t ToWideChar(uint16_t msChar, uint32_t uCodePage)
		{
			uint32_t utf16_code = 0;
			int len = MultiByteToWideChar(uCodePage, MB_ERR_INVALID_CHARS, (char*)&msChar, 2, (wchar_t*)&utf16_code, 2);
			if (len == 0) { utf16_code = 0; }
			if (len > 2) { throw std::runtime_error("ToWideChar Error!"); }
			if (utf16_code >> 16) { throw std::runtime_error("ToWideChar Error!"); }
			return utf16_code;
		}

	};
}

#endif // _RXCVT_MAKE_TABLE_


