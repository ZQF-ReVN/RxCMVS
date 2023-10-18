#include "String.h"

#include <stdio.h>
#include <Windows.h>


namespace Rut::Platform
{
	size_t StrLen(const char* cpStr)
	{
		return ::strlen(cpStr);
	}

	size_t StrLen(const wchar_t* wpStr)
	{
		return ::wcslen(wpStr);
	}


	size_t StrCpy(char* cpDes, size_t nMaxBytes, const char* cpSrc)
	{
		errno_t err = ::strcpy_s(cpDes, nMaxBytes, cpSrc);
		if (err) { return 0; }
		return nMaxBytes;
	}

	size_t StrCpy(wchar_t* wpDes, size_t nMaxChars, const wchar_t* wpSrc)
	{
		errno_t err = ::wcscpy_s(wpDes, nMaxChars, wpSrc);
		if (err) { return 0; }
		return nMaxChars;
	}


	size_t Sprintf_V(char* cpBuffer, size_t nMaxBytes, const char* cpFormat, va_list ArgList)
	{
		return (size_t)::vsprintf_s(cpBuffer, nMaxBytes, cpFormat, ArgList);
	}

	size_t Sprintf_V(wchar_t* cpBuffer, size_t nMaxChar, const wchar_t* cpFormat, va_list ArgList)
	{
		return (size_t)::vswprintf_s(cpBuffer, nMaxChar, cpFormat, ArgList);
	}


	size_t StrToWStr(const char* cpStr, size_t msStrBytes, wchar_t* wpStr, size_t wsStrChars, size_t nCodePage)
	{
		return ::MultiByteToWideChar((UINT)nCodePage, MB_ERR_INVALID_CHARS, cpStr, (int)msStrBytes, wpStr, (int)wsStrChars);
	}

	size_t WStrToStr(const wchar_t* wpStr, size_t wsStrChars, char* cpStr, size_t msStrBytes, size_t nCodePage, bool* isError, const char* cpDefault)
	{
		BOOL not_all_cvt = TRUE;
		size_t cch = ::WideCharToMultiByte((UINT)nCodePage, 0, wpStr, (int)wsStrChars, cpStr, (int)msStrBytes, cpDefault, &not_all_cvt);
		if (isError) { *isError = not_all_cvt; }
		return cch;
	}
}