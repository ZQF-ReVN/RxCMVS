#pragma once
#include <Windows.h>


namespace TDA
{
	class MemX
	{
	public:
		static BOOL WriteMemory(LPVOID lpAddress, LPCVOID lpBuffer, SIZE_T nSize);
		static BOOL ReadMemory(LPVOID lpAddress, LPVOID lpBuffer, SIZE_T nSize);
		static BOOL WriteHookCode(DWORD dwRawAddress, DWORD dwNewAddress, SIZE_T szHookCode);
		static BOOL WriteHookCode_RET(DWORD dwRawAddress, DWORD dwRetAddress, DWORD dwNewAddress);
		static BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize);
		static DWORD MemSearch(DWORD pFind, SIZE_T szFind, PBYTE pToFind, SIZE_T szToFind, BOOL backward);

	};
}


