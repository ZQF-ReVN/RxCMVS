#include "Path.h"

#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


namespace Rut::Platform
{
	size_t GetModuleDir(char* pBuffer, size_t nMaxBytes)
	{
		size_t size = GetCurrentDirectoryA(nMaxBytes, pBuffer);
		if ((size == 0) || (size == (nMaxBytes - 1))) { return 0; }
		memcpy(pBuffer + size, "\\", 2);
		return ++size;
	}

	size_t GetModuleDir(wchar_t* pBuffer, size_t nMaxChar)
	{
		size_t size = GetCurrentDirectoryW(nMaxChar, pBuffer);
		if ((size == 0) || (size == (nMaxChar - 1))) { return 0; }
		memcpy(pBuffer + size, L"\\", 4);
		return ++size;
	}

	size_t GetModulePath(char* cpBuffer, size_t nMaxBytes, void* pBase)
	{
		return ::GetModuleFileNameA((HMODULE)pBase, cpBuffer, nMaxBytes);
	}

	size_t GetModulePath(wchar_t* wpBuf, size_t nMaxChar, void* pBase)
	{
		return ::GetModuleFileNameW((HMODULE)pBase, wpBuf, nMaxChar);
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
		return ::PathIsDirectoryA(cpPath);
	}

	bool DirExist(const wchar_t* wpPath)
	{
		return ::PathIsDirectoryW(wpPath);
	}

	bool FileExist(const char* cpPath)
	{
		return ::PathFileExistsA(cpPath);
	}

	bool FileExist(const wchar_t* wpPath)
	{
		return ::PathFileExistsW(wpPath);
	}

	bool Exist(const char* cpPath)
	{
		return (::GetFileAttributesA(cpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}

	bool Exist(const wchar_t* wpPath)
	{
		return (::GetFileAttributesW(wpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}
}