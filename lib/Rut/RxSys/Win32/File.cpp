#include "File.h"

#include <Windows.h>


namespace Rut::RxSys
{
	size_t GetFileSize(const std::filesystem::path& phPath)
	{
		WIN32_FIND_DATAW find_data;
		HANDLE hfind = ::FindFirstFileW(phPath.wstring().data(), &find_data);
		if (hfind == INVALID_HANDLE_VALUE) { return 0; }
		::CloseHandle(hfind);

		if (sizeof(size_t) == 4)
		{
			return (size_t)find_data.nFileSizeLow;
		}
		else if (sizeof(size_t) == 8)
		{
			uint64_t size_h = (uint64_t)find_data.nFileSizeHigh;
			uint64_t size = (uint64_t)find_data.nFileSizeLow;
			size |= (size_h << 32);
			return (size_t)size;
		}

		return 0;
	}

	uint64_t GetFileSize64(const std::filesystem::path& phPath)
	{
		WIN32_FIND_DATAW find_data;
		HANDLE hfind = ::FindFirstFileW(phPath.wstring().data(), &find_data);
		if (hfind == INVALID_HANDLE_VALUE) { return 0; }
		::CloseHandle(hfind);

		uint64_t size_h = (uint64_t)find_data.nFileSizeHigh;
		uint64_t size = (uint64_t)find_data.nFileSizeLow;
		size |= (size_h << 32);
		return size;
	}


	/*
        0x01::0000 0001:0:Read
        0x02::0000 0010:1:Write
        0x04::0000 0100:2:Create_Always
        0x08::0000 1000:3:Create_New   (if exist failed)
        0x10::0001 0000:4:Open_Always
        0x20::0010 0000:5:Open_Existing     (if not exist failed)
	*/
	static void GetFlag(size_t nMode, PDWORD pAccess, PDWORD pAttributes)
	{
		constexpr uint8_t RIO_DATA_IN = 0x01;
		constexpr uint8_t RIO_DATA_OUT = 0x02;
		constexpr uint8_t RIO_CREATE_ALWAYS = 0x04;
		constexpr uint8_t RIO_CREATE_NEW = 0x08;
		constexpr uint8_t RIO_OPEN_ALWAYS = 0x10;
		constexpr uint8_t RIO_OPEN_EXISTING = 0x20;

		DWORD flag_access = 0;
		if (nMode & RIO_DATA_IN) { flag_access |= GENERIC_READ; }
		if (nMode & RIO_DATA_OUT) { flag_access |= GENERIC_WRITE; }
		*pAccess = flag_access;


		DWORD flag_attributes = 0;
		if (nMode & RIO_CREATE_ALWAYS) { flag_attributes |= CREATE_ALWAYS; }
		if (nMode & RIO_CREATE_NEW) { flag_attributes |= CREATE_NEW; }
		if (nMode & RIO_OPEN_ALWAYS) { flag_attributes |= OPEN_ALWAYS; }
		if (nMode & RIO_OPEN_EXISTING) { flag_attributes |= OPEN_EXISTING; }
		*pAttributes = flag_attributes;
	}

	void* FileOpen(const std::filesystem::path& phPath, size_t nMode)
	{
		DWORD flag_access = 0, flag_attributes = 0;
		GetFlag(nMode, &flag_access, &flag_attributes);
		const HANDLE hfile = ::CreateFileW(phPath.wstring().data(), flag_access, FILE_SHARE_READ, nullptr, flag_attributes, FILE_ATTRIBUTE_NORMAL, nullptr);
		return (hfile == INVALID_HANDLE_VALUE) ? (nullptr) : (void*)hfile;
	}

	bool FileClose(void* hFile)
	{
		return ::CloseHandle((HANDLE)hFile);
	}

	bool FileFlush(void* hFile)
	{
		return ::FlushFileBuffers((HANDLE)hFile);
	}

	size_t FileGetSize(void* hFile)
	{
		return (size_t)::GetFileSize((HANDLE)hFile, nullptr);
	}

	size_t FileSetPtr(void* hFile, size_t nOffset, size_t nWay)
	{
		return (size_t)::SetFilePointer((HANDLE)hFile, (LONG)nOffset, NULL, (DWORD)nWay);
	}

	size_t FileGetPtr(void* hFile)
	{
		constexpr uint8_t RIO_BEGIN = 0x0;
		constexpr uint8_t RIO_CURRENT = 0x1;
		constexpr uint8_t RIO_END = 0x2;

		return ::SetFilePointer((HANDLE)hFile, 0, NULL, RIO_CURRENT);
	}

	size_t FileRead(void* hFile, void* pBuffer, size_t nSize)
	{
		DWORD read = 0;
		const BOOL status = ::ReadFile((HANDLE)hFile, pBuffer, (DWORD)nSize, &read, nullptr);
		return (status) ? ((size_t)read) : (0);
	}

	size_t FileWrite(void* hFile, const void* pData, size_t nSize)
	{
		DWORD write = 0;
		const BOOL status = ::WriteFile((HANDLE)hFile, pData, (DWORD)nSize, &write, nullptr);
		return (status) ? ((size_t)write) : (0);
	}
}