#include "Cvt.h"

#include <Windows.h>
#include <stdexcept>


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
