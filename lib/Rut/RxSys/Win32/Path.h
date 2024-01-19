#pragma once


namespace Rut::RxSys
{
	constexpr size_t PLATFORM_MAX_PATH = 260;

	size_t GetModuleDir(char* pBuffer, size_t nMaxBytes);
	size_t GetModuleDir(wchar_t* pBuffer, size_t nMaxChar);

	size_t GetModulePath(char* cpBuffer, size_t nMaxBytes, void* pBase);
	size_t GetModulePath(wchar_t* wpBuf, size_t nMaxChar, void* pBase);

	bool MakeDir(const char* cpDir);
	bool MakeDir(const wchar_t* wpDir);

	bool DirExist(const char* cpPath);
	bool DirExist(const wchar_t* wpPath);

	bool FileExist(const char* cpPath);
	bool FileExist(const wchar_t* wpPath);
}