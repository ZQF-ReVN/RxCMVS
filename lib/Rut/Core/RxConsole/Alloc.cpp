#include "Alloc.h"
#include "../../Platform/Platform.h"

#include <stdio.h>
#include <locale.h>
#include <Windows.h>


namespace Rut::RxConsole
{
	static void SetConsoleTop()
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
			}

			Sleep(1000);
		}

		::MessageBoxW(NULL, L"Topmost Console Window Failed!", NULL, NULL);
	}

	static void SetConsoleNoQuickEdit()
	{
		DWORD mode = 0;
		::GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		::SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_QUICK_EDIT_MODE);
	}

	static void SetConsoleLocale()
	{
		//system("chcp 65001");
		//system("chcp 936");
		setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));
	}

	static FILE* SetConsoleSTDIO()
	{
		FILE* fp_console = nullptr;
		freopen_s(&fp_console, "CONIN$", "r+t", stdin);
		freopen_s(&fp_console, "CONOUT$", "w+t", stdout);
		return fp_console;
	}


	FILE* Alloc(const wchar_t* lpTitle, bool isEdit, bool isTop)
	{
		Platform::AllocConsole(lpTitle);
		if (isTop == true) { Platform::CreateThread(SetConsoleTop); }
		if (isEdit == false) { SetConsoleNoQuickEdit(); }
		SetConsoleLocale();

		return SetConsoleSTDIO();
	}
}


