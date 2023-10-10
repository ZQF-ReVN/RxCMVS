#pragma once


namespace Rut::RxCvt
{
	size_t MbcsStrToUnicodeStr(const char* cpStr, wchar_t* wpStr, size_t nCodePage);
	size_t UnicodeStrToMbcsStr(const wchar_t* wpStr, char* cpStr, size_t nCodePage, bool* isError, const char* cpDefault);
}