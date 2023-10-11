#include "Alloc.h"

#include <stdio.h>
#include <locale.h>
#include <Windows.h>


namespace Rut::RxConsole
{
	static bool __stdcall SetConsoleTop(LPVOID lpParameter)
	{
		HWND consoleHWND = NULL;
		WCHAR consoleTitle[MAX_PATH] = { 0 };

		for (size_t ite = 20; ite-- > 0;)
		{
			::GetConsoleTitleW(consoleTitle, MAX_PATH);
			consoleHWND = FindWindowW(NULL, (LPWSTR)consoleTitle);
			if (consoleHWND != NULL)
			{
				::SetWindowPos(consoleHWND, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				return true;
			}

			Sleep(1000);
		}

		::MessageBoxW(NULL, L"Topmost Console Window Failed!", NULL, NULL);
		return false;
	}

	static void __stdcall SetConsoleNoQuickEdit()
	{
		DWORD mode = 0;
		::GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		::SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_QUICK_EDIT_MODE);
	}

	static void __stdcall SetConsoleLocale()
	{
		//system("chcp 65001");
		//system("chcp 936");
		setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));
	}

	static FILE* __stdcall SetConsoleSTDIO()
	{
		FILE* fp_console = nullptr;
		freopen_s(&fp_console, "CONIN$", "r+t", stdin);
		freopen_s(&fp_console, "CONOUT$", "w+t", stdout);
		return fp_console;
	}


	FILE* Alloc(const wchar_t* lpTitle, bool isEdit, bool isTop)
	{
		::AllocConsole();
		::AttachConsole(ATTACH_PARENT_PROCESS);

		::SetConsoleTitleW(lpTitle);
		if (isTop == true) { ::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SetConsoleTop, nullptr, 0, nullptr); }
		if (isEdit == false) { SetConsoleNoQuickEdit(); }
		SetConsoleLocale();

		return SetConsoleSTDIO();
	}
}


