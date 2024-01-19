#include "Path.h"

#include <Windows.h>


namespace Rut::RxSys
{
	size_t GetModuleDir(char* pBuffer, size_t nMaxBytes)
	{
		size_t size = (size_t)::GetCurrentDirectoryA((DWORD)nMaxBytes, pBuffer);
		if ((size == 0) || (size == (nMaxBytes - 1))) { return 0; }
		memcpy(pBuffer + size, "\\", 2);
		return ++size;
	}

	size_t GetModuleDir(wchar_t* pBuffer, size_t nMaxChar)
	{
		size_t size = (size_t)::GetCurrentDirectoryW((DWORD)nMaxChar, pBuffer);
		if ((size == 0) || (size == (nMaxChar - 1))) { return 0; }
		memcpy(pBuffer + size, L"\\", 4);
		return ++size;
	}

	size_t GetModulePath(char* cpBuffer, size_t nMaxBytes, void* pBase)
	{
		return (size_t)::GetModuleFileNameA((HMODULE)pBase, cpBuffer, (DWORD)nMaxBytes);
	}

	size_t GetModulePath(wchar_t* wpBuf, size_t nMaxChar, void* pBase)
	{
		return (size_t)::GetModuleFileNameW((HMODULE)pBase, wpBuf, (DWORD)nMaxChar);
	}


	bool MakeDir(const char* cpDir)
	{
		return ::CreateDirectoryA(cpDir, nullptr);
	}

	bool MakeDir(const wchar_t* wpDir)
	{
		return ::CreateDirectoryW(wpDir, nullptr);
	}

	bool DirExist(const char* cpPath)
	{
		return (::GetFileAttributesA(cpPath) & FILE_ATTRIBUTE_DIRECTORY) ? false : true;
	}

	bool DirExist(const wchar_t* wpPath)
	{
		return (::GetFileAttributesW(wpPath) & FILE_ATTRIBUTE_DIRECTORY) ? false : true;
	}

	bool FileExist(const char* cpPath)
	{
		return (::GetFileAttributesA(cpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}

	bool FileExist(const wchar_t* wpPath)
	{
		return (::GetFileAttributesW(wpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}
}