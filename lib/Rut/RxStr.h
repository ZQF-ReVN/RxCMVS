#pragma once
#include <string>
#include <vector>


namespace Rut::RxStr
{
	size_t Sprintf(char* cpBuffer, size_t nMaxBytes, const char* cpFormat, ...);
	size_t Sprintf(wchar_t* wpBuffer, size_t nMaxBytes, const wchar_t* wpFormat, ...);

	std::wstring ToWCS(const std::string_view msStr, size_t uCodePage);
	std::string ToMBCS(const std::wstring_view wsStr, size_t uCodePage);
	size_t ToWCS(const std::string_view msStr, std::wstring& wsStr, size_t uCodePage);
	size_t ToMBCS(const std::wstring_view wsStr, std::string& msStr, size_t uCodePage);

	std::wstring Trim(std::wstring wsLine, const wchar_t* wFilterChar = L" \r\n\t");


	class FormatLine
	{
	private:
		std::wstring m_wsInsert;
		std::vector<const wchar_t*> m_vecBreakChar;

	public:
		FormatLine(std::wstring wsInsert, std::vector<const wchar_t*> vecBreakChar);
		~FormatLine();

		void SetInsertString(std::wstring wsInsert);
		void SetBreakChar(std::vector<const wchar_t*> vecBreakChar);

		bool BreakLine(std::wstring& wsLine, size_t szMaxLine);

	};
}

