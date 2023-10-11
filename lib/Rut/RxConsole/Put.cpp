#include "Put.h"
#include "../RxCvt.h"

#include <string>
#include <stdio.h>
#include <Windows.h>


namespace Rut::RxConsole
{
	static constexpr size_t sg_uiBufferCount = 1024;


	bool Put(const char* cpStr)
	{
		return Put(cpStr, (uint32_t)strlen(cpStr));
	}

	bool Put(const char* cpStr, uint32_t nChar)
	{
		return WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cpStr, nChar, NULL, NULL);
	}

	bool Put(std::string_view msStr)
	{
		return Put(msStr.data(), (uint32_t)msStr.size());
	}

	bool Put(const wchar_t* wpStr)
	{
		return Put(wpStr, (uint32_t)wcslen(wpStr));
	}

	bool Put(const wchar_t* wpStr, uint32_t nChar)
	{
		return WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wpStr, nChar, NULL, NULL);
	}

	bool Put(std::wstring_view wsStr)
	{
		return Put(wsStr.data(), (uint32_t)wsStr.size());
	}

	bool PutMBCS(const char* cpStr, uint32_t uiCodePage)
	{
		std::wstring out_str;
		int32_t cch = RxCvt::ToWCS(cpStr, out_str, uiCodePage);
		return Put(out_str.c_str(), cch);
	}

	bool PutMBCS(std::string_view msStr, uint32_t uiCodePage)
	{
		return PutMBCS(msStr.data(), uiCodePage);
	}

	bool PutFormat(const char* cpFormat, ...)
	{
		char buffer[sg_uiBufferCount];

		va_list args = nullptr;
		va_start(args, cpFormat);
		int32_t cch = vsprintf_s(buffer, sg_uiBufferCount, cpFormat, args);
		if (cch <= 0) { return false; }
		va_end(args);

		return Put(buffer, cch);
	}

	bool PutFormat(const wchar_t* cpFormat, ...)
	{
		wchar_t buffer[sg_uiBufferCount];

		va_list args = nullptr;
		va_start(args, cpFormat);
		int32_t cch = vswprintf_s(buffer, sg_uiBufferCount, cpFormat, args);
		if (cch <= 0) { return false; }
		va_end(args);

		return Put(buffer, cch);
	}
}
