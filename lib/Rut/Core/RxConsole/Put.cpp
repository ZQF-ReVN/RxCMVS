#include "Put.h"
#include "../../RxStr.h"
#include "../../Platform/Platform.h"

#include <string>


namespace Rut::RxConsole
{
	static constexpr size_t sg_uiBufferCount = 1024;


	bool Put(const char* cpStr)
	{
		return Put(cpStr, (uint32_t)strlen(cpStr));
	}

	bool Put(const char* cpStr, uint32_t nChar)
	{
		return Platform::PutConsole(cpStr, nChar);
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
		return Platform::PutConsole(wpStr, nChar);
	}

	bool Put(std::wstring_view wsStr)
	{
		return Put(wsStr.data(), (uint32_t)wsStr.size());
	}

	bool PutMBCS(const char* cpStr, uint32_t uiCodePage)
	{
		std::wstring out_str;
		size_t cch = RxStr::ToWCS(cpStr, out_str, uiCodePage);
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
		size_t cch = Platform::Sprintf_V(buffer, sg_uiBufferCount, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}

	bool PutFormat(const wchar_t* cpFormat, ...)
	{
		wchar_t buffer[sg_uiBufferCount];

		va_list args = nullptr;
		va_start(args, cpFormat);
		int32_t cch = Platform::Sprintf_V(buffer, sg_uiBufferCount, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}
}
