#include "MemX.h"


namespace TDA
{
	BOOL MemX::WriteMemory(LPVOID lpAddress, LPCVOID lpBuffer, SIZE_T nSize)
	{
		DWORD old = 0;
		BOOL protect = VirtualProtectEx(GetCurrentProcess(), lpAddress, nSize, PAGE_EXECUTE_READWRITE, &old);
		BOOL written = WriteProcessMemory(GetCurrentProcess(), lpAddress, lpBuffer, nSize, NULL);

		if (protect && written) return TRUE;

		MessageBoxW(NULL, L"WriteMemory Failed!!", NULL, NULL);

		return FALSE;
	}

	BOOL MemX::ReadMemory(LPVOID lpAddress, LPVOID lpBuffer, SIZE_T nSize)
	{
		DWORD old = 0;
		BOOL protect = VirtualProtectEx(GetCurrentProcess(), lpAddress, nSize, PAGE_EXECUTE_READWRITE, &old);
		BOOL written = ReadProcessMemory(GetCurrentProcess(), lpAddress, lpBuffer, nSize, NULL);

		if (protect && written) return TRUE;

		MessageBoxW(NULL, L"ReadMemory Failed!!", NULL, NULL);

		return FALSE;
	}

	BOOL MemX::WriteHookCode(DWORD dwRawAddress, DWORD dwNewAddress, SIZE_T szHookCode)
	{
		UCHAR code[0xF];
		memset(code, 0x90, 0xF);

		*(UCHAR*)(code + 0) = 0xE9;
		*(DWORD*)(code + 1) = dwNewAddress - dwRawAddress - 5;

		if (WriteMemory((LPVOID)dwRawAddress, &code, szHookCode)) return TRUE;

		MessageBoxW(NULL, L"WriteHookCode Failed!!", NULL, NULL);

		return FALSE;
	}

	BOOL MemX::WriteHookCode_RET(DWORD dwRawAddress, DWORD dwRetAddress, DWORD dwNewAddress)
	{
		UCHAR code[0xF];
		memset(code, 0x90, 0xF);

		*(UCHAR*)(code + 0) = 0xE9;
		*(DWORD*)(code + 1) = dwNewAddress - dwRawAddress - 5;

		if (WriteMemory((LPVOID)dwRawAddress, &code, dwRetAddress - dwRawAddress)) return TRUE;

		MessageBoxW(NULL, L"WriteHookCode Failed!!", NULL, NULL);

		return FALSE;
	}

	BOOL MemX::SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize)
	{
		DWORD old = 0;
		DWORD rva = 0;
		BYTE rawJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
		BYTE retJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
		BYTE tarCal[] = { 0xE8,0x00,0x00,0x00,0x00 };

		BOOL protect = VirtualProtect((LPVOID)dwRawAddr, 0x1000, PAGE_EXECUTE_READWRITE, &old);
		PBYTE alloc = (PBYTE)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (alloc && protect)
		{
			//Copy the Code for the original address to alloc address
			memcpy(alloc, (PVOID)dwRawAddr, szRawSize);

			//Write Jmp Code
			rva = (DWORD)alloc - dwRawAddr - sizeof(rawJmp);
			memcpy(&rawJmp[1], &rva, sizeof(DWORD));
			memcpy((PBYTE)dwRawAddr, rawJmp, sizeof(rawJmp));

			//Write Call TarFunc Code
			rva = dwTarAddr - (DWORD)(&alloc[szRawSize]) - sizeof(tarCal);
			memcpy(&tarCal[1], &rva, sizeof(DWORD));
			memcpy(&alloc[szRawSize], tarCal, sizeof(tarCal));

			//Write Ret Code
			rva = (dwRawAddr + szRawSize) - (DWORD)(&alloc[szRawSize + sizeof(tarCal)]) - sizeof(retJmp);
			memcpy(&retJmp[1], &rva, sizeof(DWORD));
			memcpy(&alloc[szRawSize + sizeof(tarCal)], retJmp, sizeof(retJmp));

			return TRUE;
		}
		else
		{
			MessageBoxW(NULL, L"SetHook Failed!!", NULL, NULL);
			return FALSE;
		}
	}

	DWORD MemX::MemSearch(DWORD pFind, SIZE_T szFind, PBYTE pToFind, SIZE_T szToFind, BOOL backward = FALSE)
	{
		if ((pFind >= 0x7FFF0000) || (pFind <= 0x00010000) || !szToFind) return NULL;

		if (!backward)
		{
			for (size_t ite = 0; ite < szFind; ite++)
			{
				if (!memcmp(pToFind, (void*)pFind++, szToFind)) return (pFind - 1);
			}
		}
		else
		{
			for (size_t ite = 0; ite < szFind; ite++)
			{
				if (!memcmp(pToFind, (void*)pFind--, szToFind)) return (pFind + 1);
			}
		}

		MessageBoxW(NULL, L"MemSearch Failed!!", NULL, NULL);
		ExitProcess(0);
		return NULL;
	}
}

