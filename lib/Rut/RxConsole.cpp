#include "RxConsole.h"
#include "RxStr.h"
#include "RxSys.h"

#include <string>
#include <clocale>


namespace Rut::RxConsole
{
	bool Alloc(const wchar_t* lpTitle, bool isEdit)
	{
		Rut::RxSys::ConsoleAlloc(lpTitle);
		Rut::RxSys::ConsoleQuickEditMode(isEdit);
		Rut::RxSys::ConsoleRedirectionSTDIO();

		//system("chcp 65001");
		//system("chcp 936");
		::setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));

		return true;
	}
}


namespace Rut::RxConsole
{
	static constexpr size_t PUT_BUFFER_MAX = 1024;


	bool Put(const char* cpStr, size_t nChar)
	{
		return Rut::RxSys::PutConsole(cpStr, nChar);
	}

	bool Put(std::string_view msStr)
	{
		return Put(msStr.data(), msStr.size());
	}

	bool Put(const wchar_t* wpStr, size_t nChar)
	{
		return Rut::RxSys::PutConsole(wpStr, nChar);
	}

	bool Put(std::wstring_view wsStr)
	{
		return Put(wsStr.data(), wsStr.size());
	}

	bool PutMBCS(const char* cpStr, size_t nCodePage)
	{
		std::wstring out_str;
		size_t cch = RxStr::ToWCS(cpStr, out_str, nCodePage);
		return Put(out_str.c_str(), cch);
	}

	bool PutMBCS(std::string_view msStr, size_t nCodePage)
	{
		return PutMBCS(msStr.data(), nCodePage);
	}

	bool PutFormat(const char* cpFormat, ...)
	{
		char buffer[PUT_BUFFER_MAX];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = RxSys::Sprintf_V(buffer, PUT_BUFFER_MAX, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}

	bool PutFormat(const wchar_t* cpFormat, ...)
	{
		wchar_t buffer[PUT_BUFFER_MAX];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = RxSys::Sprintf_V(buffer, PUT_BUFFER_MAX, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}
}
