#pragma once
#include <Windows.h>
#include <iostream>


namespace TDA
{
	class ConsoleX
	{
	public:
		static FILE* SetConsole(LPCWSTR lpTitle);
		static BOOL  PutConsoleA(LPCSTR lpFormat, ...);
		static BOOL  PutConsoleW(LPCWSTR lpFormat, ...);
	};
}

