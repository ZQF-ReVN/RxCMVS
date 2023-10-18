#include "File.h"

#include <Windows.h>


namespace Rut::Platform
{
	std::uintmax_t GetFileSize(const char* cpPath)
	{
		const HANDLE hfile = ::CreateFileA(cpPath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return 0; }
		std::uintmax_t h_size = 0;
		std::uintmax_t size = ::GetFileSize(hfile, (LPDWORD)(&h_size));
		size |= (h_size << 32);
		CloseHandle(hfile);
		return size;
	}

	std::uintmax_t GetFileSize(const wchar_t* wpPath)
	{
		const HANDLE hfile = ::CreateFileW(wpPath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return 0; }
		std::uintmax_t h_size = 0;
		std::uintmax_t size = ::GetFileSize(hfile, (LPDWORD)(&h_size));
		size |= (h_size << 32);
		CloseHandle(hfile);
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
		constexpr uint8_t RIO_IN = 0x01;
		constexpr uint8_t RIO_OUT = 0x02;
		constexpr uint8_t RIO_CREATE_ALWAYS = 0x04;
		constexpr uint8_t RIO_CREATE_NEW = 0x08;
		constexpr uint8_t RIO_OPEN_ALWAYS = 0x10;
		constexpr uint8_t RIO_OPEN_EXISTING = 0x20;

		DWORD flag_access = 0;
		if (nMode & RIO_IN) { flag_access |= GENERIC_READ; }
		if (nMode & RIO_OUT) { flag_access |= GENERIC_WRITE; }
		*pAccess = flag_access;


		DWORD flag_attributes = 0;
		if (nMode & RIO_CREATE_ALWAYS) { flag_attributes |= CREATE_ALWAYS; }
		if (nMode & RIO_CREATE_NEW) { flag_attributes |= CREATE_NEW; }
		if (nMode & RIO_OPEN_ALWAYS) { flag_attributes |= OPEN_ALWAYS; }
		if (nMode & RIO_OPEN_EXISTING) { flag_attributes |= OPEN_EXISTING; }
		*pAttributes = flag_attributes;
	}

	void* FileOpen(const char* cpPath, size_t nMode)
	{
		DWORD flag_access = 0, flag_attributes = 0;
		GetFlag(nMode, &flag_access, &flag_attributes);
		const HANDLE hfile = ::CreateFileA(cpPath, flag_access, FILE_SHARE_READ, nullptr, flag_attributes, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return nullptr; }
		return (void*)hfile;
	}

	void* FileOpen(const wchar_t* wpPath, size_t nMode)
	{
		DWORD flag_access = 0, flag_attributes = 0;
		GetFlag(nMode, &flag_access, &flag_attributes);
		const HANDLE hfile = ::CreateFileW(wpPath, flag_access, FILE_SHARE_READ, nullptr, flag_attributes, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return nullptr; }
		return (void*)hfile;
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
		return ::SetFilePointer((HANDLE)hFile, nOffset, NULL, nWay);
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
		const BOOL status = ::ReadFile((HANDLE)hFile, pBuffer, nSize, &read, nullptr);
		return (status) ? (read) : (0);
	}

	size_t FileWrite(void* hFile, void* pData, size_t nSize)
	{
		DWORD write = 0;
		const BOOL status = ::WriteFile((HANDLE)hFile, pData, nSize, &write, nullptr);
		return (status) ? (write) : (0);
	}
}