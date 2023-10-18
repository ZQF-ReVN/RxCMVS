#pragma once
#include <cstdint>


namespace Rut::Platform
{
	std::uintmax_t GetFileSize(const char* cpPath);
	std::uintmax_t GetFileSize(const wchar_t* wpPath);

	void* FileOpen(const char* cpPath, size_t nMode);
	void* FileOpen(const wchar_t* wpPath, size_t nMode);
	bool FileClose(void* hFile);
	bool FileFlush(void* hFile);
	size_t FileGetSize(void* hFile);
	size_t FileSetPtr(void* hFile, size_t nPos, size_t nWay);
	size_t FileGetPtr(void* hFile);
	size_t FileRead(void* hFile, void* pBuffer, size_t nSize);
	size_t FileWrite(void* hFile, void* pData, size_t nSize);
}