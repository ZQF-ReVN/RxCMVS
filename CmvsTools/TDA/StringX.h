#pragma once
#include <string>


namespace TDA
{
	class StringX
	{
	public:
		static std::locale&  GetCVT_UTF8();

		static std::wstring StrToWStr(std::string& msString, size_t uCodePage);
		static std::string  WStrToStr(std::wstring& wsString, size_t uCodePage);
		static size_t        StrToWStr(std::string& msString, std::wstring& wsString, size_t uCodePage);
		static size_t        WStrToStr(std::wstring& wsString, std::string& msString, size_t uCodePage);

		static std::wstring StrToWStr_CVT(std::string& msString,size_t uCodePage);
		static std::string  WStrToStr_CVT(std::wstring& wsString, size_t uCodePage);
		static void          StrToWStr_CVT(std::string& msString, std::wstring& wsString, size_t uCodePage);
		static void          WStrToStr_CVT(std::wstring& wsString, std::string& msString, size_t uCodePage);

	};
}