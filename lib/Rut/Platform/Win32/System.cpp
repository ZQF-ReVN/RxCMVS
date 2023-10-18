#include "System.h"

#include <stdio.h>
#include <Windows.h>


namespace Rut::Platform
{
	bool AllocConsole(const wchar_t* wpTile)
	{
		if (::AllocConsole() && ::AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if (wpTile) { ::SetConsoleTitleW(wpTile); }
			return true;
		}
		return false;
	}

	bool PutConsole(const char* cpText, size_t nSize)
	{
		return ::WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cpText, nSize, nullptr, nullptr);
	}

	bool PutConsole(const wchar_t* wpText, size_t nSize)
	{
		return ::WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wpText, nSize, nullptr, nullptr);
	}


	size_t CreateThread(void(*fnThread)())
	{
		return (size_t)::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)fnThread, nullptr, 0, nullptr);
	}


	size_t MemCpy(void* pDes, const void* pSrc, size_t nBytes)
	{
		::memcpy(pDes, pSrc, nBytes);
		return nBytes;
	}
}