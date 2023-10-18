#pragma once
#include <string>


namespace Rut::RxConsole
{
	bool Put(const char* cpStr);
	bool Put(const char* cpStr, uint32_t nChar);
	bool Put(std::string_view msStr);
	bool Put(const wchar_t* wpStr);
	bool Put(const wchar_t* wpStr, uint32_t nChar);
	bool Put(std::wstring_view wsStr);
	bool PutFormat(const char* cpFormat, ...);
	bool PutFormat(const wchar_t* cpFormat, ...);
	bool PutMBCS(const char* cpStr, uint32_t uiCodePage);
	bool PutMBCS(std::string_view msStr, uint32_t uiCodePage);
}

