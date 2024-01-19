#include "RxStr.h"
#include "RxSys.h"

#include <stdexcept>


namespace Rut::RxStr
{
	size_t Sprintf(char* cpBuffer, size_t nMaxBytes, const char* cpFormat, ...)
	{
		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = RxSys::Sprintf_V(cpBuffer, nMaxBytes, cpFormat, args);
		va_end(args);
		return cch;
	}

	size_t Sprintf(wchar_t* wpBuffer, size_t nMaxBytes, const wchar_t* wpFormat, ...)
	{
		va_list args = nullptr;
		va_start(args, wpFormat);
		size_t cch = RxSys::Sprintf_V(wpBuffer, nMaxBytes, wpFormat, args);
		va_end(args);
		return cch;
	}


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
		size_t wstr_len = RxSys::StrToWStr(mstr_ptr, mstr_len, nullptr, 0, uCodePage);
		if (wstr_len == 0) 
		{
			throw std::runtime_error("RxCvt::ToWCS Error!");
		}

		wsStr.resize(wstr_len);
		wstr_len = RxSys::StrToWStr(mstr_ptr, mstr_len, wsStr.data(), wstr_len, uCodePage);
		if (wstr_len == 0) 
		{ 
			throw std::runtime_error("RxCvt::ToWCS Error!");
		}

		return wstr_len;
	}

	size_t ToMBCS(const std::wstring_view wsStr, std::string& msStr, size_t uCodePage)
	{
		size_t wstr_len = wsStr.length();
		if (wstr_len == 0) { msStr.clear(); return 0; }

		const wchar_t* wstr_ptr = wsStr.data();
		size_t mstr_bytes = RxSys::WStrToStr(wstr_ptr, wstr_len, nullptr, 0, uCodePage, nullptr, nullptr);
		if (mstr_bytes == 0) 
		{
			throw std::runtime_error("RxCvt::ToMBCS Error!");
		}

		bool is_error = false;

		msStr.resize(mstr_bytes);
		mstr_bytes = RxSys::WStrToStr(wstr_ptr, wstr_len, msStr.data(), mstr_bytes, uCodePage, &is_error, "?");
		if (is_error || (mstr_bytes == 0)) 
		{ 
			throw std::runtime_error("RxCvt::ToMBCS Error!");
		}

		return mstr_bytes;
	}


	std::wstring Trim(std::wstring wsLine, const wchar_t* wFilterChar)
	{
		wsLine.erase(0, wsLine.find_first_not_of(wFilterChar));
		wsLine.erase(wsLine.find_last_not_of(wFilterChar) + 1);
		return wsLine;
	}


	FormatLine::FormatLine(std::wstring wsInsert, std::vector<const wchar_t*> vecBreakChar)
	{
		m_wsInsert = wsInsert;
		m_vecBreakChar = vecBreakChar;
		//FormatLine formatLine(L"[n]", { L"。", L"？", L"，", L"。" });
	}

	FormatLine::~FormatLine()
	{

	}

	void FormatLine::SetBreakChar(std::vector<const wchar_t*> vecBreakChar)
	{
		m_vecBreakChar = vecBreakChar;
	}

	void FormatLine::SetInsertString(std::wstring wsInsert)
	{
		m_wsInsert = wsInsert;
	}

	bool FormatLine::BreakLine(std::wstring& wsLine, size_t szMaxLine)
	{
		size_t findPos = 0;
		size_t strLen = wsLine.size();
		if (strLen <= szMaxLine) return false;

		for (auto breakChar : m_vecBreakChar)
		{
			findPos = strLen - 3;

			for (;;)
			{
				findPos = wsLine.rfind(breakChar, findPos);
				if (findPos-- == std::wstring::npos) break;
				if ((findPos < szMaxLine) && ((strLen - findPos) < szMaxLine))
				{
					wsLine.insert(findPos + 2, m_wsInsert);
					return true;
				}
			}

			findPos = 0;
		}

		return false;
	}
}