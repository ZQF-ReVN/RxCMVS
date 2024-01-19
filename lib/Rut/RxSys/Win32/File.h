#pragma once
#include <cstdint>
#include <filesystem>


namespace Rut::RxSys
{
	size_t GetFileSize(const std::filesystem::path& phPath);
	uint64_t GetFileSize64(const std::filesystem::path& phPath);

	void* FileOpen(const std::filesystem::path& phPath, size_t nMode);
	bool FileClose(void* hFile);
	bool FileFlush(void* hFile);
	size_t FileGetSize(void* hFile);
	size_t FileSetPtr(void* hFile, size_t nPos, size_t nWay);
	size_t FileGetPtr(void* hFile);
	size_t FileRead(void* hFile, void* pBuffer, size_t nSize);
	size_t FileWrite(void* hFile, const void* pData, size_t nSize);
}