#pragma once
#include <cstdarg>


namespace Rut::RxSys
{
	size_t StrLen(const char* cpStr);
	size_t StrLen(const wchar_t* wpStr);

	bool StrCpy(char* cpDes, size_t nMaxBytes, const char* cpSrc);
	bool StrCpy(wchar_t* wpDes, size_t nMaxChars, const wchar_t* wpSrc);

	size_t Sprintf_V(char* cpBuffer, size_t nMaxBytes, const char* cpFormat, va_list ArgList);
	size_t Sprintf_V(wchar_t* cpBuffer, size_t nMaxChar, const wchar_t* cpFormat, va_list ArgList);

	size_t StrToWStr(const char* cpStr, size_t msStrBytes, wchar_t* wpStr, size_t wsStrChars, size_t nCodePage);
	size_t WStrToStr(const wchar_t* wpStr, size_t wsStrChars, char* cpStr, size_t msStrBytes, size_t nCodePage, bool* isError, const char* cpDefault);

}