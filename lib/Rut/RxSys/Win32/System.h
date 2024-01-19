#pragma once
#include <cstdint>


namespace Rut::RxSys
{
	bool ConsoleAlloc(const wchar_t* wpTile);
	void ConsoleRedirectionSTDIO();
	void ConsoleQuickEditMode(bool bStatus);
	bool PutConsole(const char* cpText, size_t nSize);
	bool PutConsole(const wchar_t* wpText, size_t nSize);
}