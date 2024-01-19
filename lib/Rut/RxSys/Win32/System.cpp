#include "System.h"

#include <stdio.h>
#include <Windows.h>


namespace Rut::RxSys
{
	bool ConsoleAlloc(const wchar_t* wpTile)
	{
		if (::AllocConsole() && ::AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if (wpTile) 
			{ 
				::SetConsoleTitleW(wpTile);
			}
			return true;
		}
		return false;
	}

	void ConsoleRedirectionSTDIO()
	{
		FILE* fp_console = nullptr;
		::freopen_s(&fp_console, "CONIN$", "r+t", stdin);
		::freopen_s(&fp_console, "CONOUT$", "w+t", stdout);
	}

	void ConsoleQuickEditMode(bool bStatus)
	{
		DWORD mode = 0;
		::GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		if (bStatus)
		{
			::SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ENABLE_QUICK_EDIT_MODE);
		}
		else
		{
			::SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_QUICK_EDIT_MODE);
		}

	}

	bool PutConsole(const char* cpText, size_t nSize)
	{
		DWORD written = 0;
		return ::WriteConsoleA(::GetStdHandle(STD_OUTPUT_HANDLE), cpText, (DWORD)nSize, &written, nullptr);
	}

	bool PutConsole(const wchar_t* wpText, size_t nSize)
	{
		DWORD written = 0;
		return ::WriteConsoleW(::GetStdHandle(STD_OUTPUT_HANDLE), wpText, (DWORD)nSize, &written, nullptr);
	}
}