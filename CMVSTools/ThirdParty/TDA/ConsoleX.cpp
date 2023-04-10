#include "ConsoleX.h"


namespace TDA
{
	BOOL __stdcall SetConsoleTop(LPVOID lpParameter)
	{
		HWND consoleHWND = NULL;
		WCHAR consoleTitle[MAX_PATH] = { 0 };

		for (size_t ite = 20; ite-- > 0;)
		{
			GetConsoleTitleW(consoleTitle, MAX_PATH);
			consoleHWND = FindWindowW(NULL, (LPWSTR)consoleTitle);
			if (consoleHWND != NULL)
			{
				SetWindowPos(consoleHWND, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				return TRUE;
			}

			Sleep(1000);
		}

		MessageBoxW(NULL, L"Topmost Console Window Failed!", NULL, NULL);
		return FALSE;
	}

	VOID __stdcall SetConsoleNoQuickEdit()
	{
		DWORD mode = 0;
		GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode & ~ENABLE_QUICK_EDIT_MODE);
	}

	VOID __stdcall SetConsoleLocale()
	{
		//system("chcp 65001");
		//system("chcp 936");
		setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));
	}

	FILE* __stdcall SetConsoleSTDIO()
	{
		FILE* fpConsole = NULL;

		freopen_s(&fpConsole, "CONIN$", "r+t", stdin);
		freopen_s(&fpConsole, "CONOUT$", "w+t", stdout);

		return fpConsole;
	}

	FILE* ConsoleX::SetConsole(LPCWSTR lpTitle)
	{
		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		SetConsoleTitleW(lpTitle);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetConsoleTop, NULL, NULL, NULL);
		SetConsoleNoQuickEdit();
		SetConsoleLocale();

		return SetConsoleSTDIO();
	}
	

	BOOL ConsoleX::PutConsoleA(LPCSTR lpFormat, ...)
	{
		static CHAR buffer[MAX_PATH] = { 0 };

		va_list args = NULL;
		va_start(args, lpFormat);
		vsprintf_s(buffer, lpFormat, args);
		va_end(args);

		return WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, lstrlenA(buffer), NULL, NULL);
	}

	BOOL ConsoleX::PutConsoleW(LPCWSTR lpFormat, ...)
	{
		static WCHAR buffer[MAX_PATH] = { 0 };

		va_list args = NULL;
		va_start(args, lpFormat);
		vswprintf_s(buffer, lpFormat, args);
		va_end(args);

		return WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), buffer, lstrlenW(buffer), NULL, NULL);
	}
}

