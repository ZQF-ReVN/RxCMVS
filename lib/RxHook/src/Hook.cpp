#include "Hook.h"
#include "Mem.h"

#include <Windows.h>
#include <detours.h>


namespace Rut::RxHook
{
	void SetHookCode_Jmp(std::uintptr_t uiRawAddress, std::uintptr_t uiNewAddress, std::size_t szHookCode)
	{
		uint8_t code[0x5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
		*(uint32_t*)(code + 1) = uiNewAddress - uiRawAddress - sizeof(code);
		SysMemWrite((LPVOID)uiRawAddress, &code, sizeof(code));

		size_t fil_size = szHookCode - 5;
		if (fil_size) { SysMemFill((void*)(uiRawAddress + 5), 0x90, fil_size); }
	}

	void SetHookCode_Call(std::uintptr_t uiRawAddress, std::uintptr_t uiNewAddress, std::size_t szHookCode)
	{
		uint8_t code[0x5] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
		*(uint32_t*)(code + 1) = uiNewAddress - uiRawAddress - sizeof(code);
		SysMemWrite((LPVOID)uiRawAddress, &code, sizeof(code));

		size_t fil_size = szHookCode - 5;
		if (fil_size) { SysMemFill((void*)(uiRawAddress + 5), 0x90, fil_size); }
	}

	bool SetHook(uint32_t uiRawAddr, uint32_t uiTarAddr, uint32_t szRawSize)
	{
		DWORD old = 0;
		DWORD rva = 0;
		BYTE rawJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
		BYTE retJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
		BYTE tarCal[] = { 0xE8,0x00,0x00,0x00,0x00 };

		SysMemAccess((LPVOID)uiRawAddr, 0x1000, PAGE_EXECUTE_READWRITE);
		PBYTE alloc = (PBYTE)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (alloc)
		{
			//Copy the Code for the original address to alloc address
			memcpy(alloc, (PVOID)uiRawAddr, szRawSize);

			//Write Jmp Code
			rva = (DWORD)alloc - uiRawAddr - sizeof(rawJmp);
			memcpy(&rawJmp[1], &rva, sizeof(DWORD));
			memcpy((PBYTE)uiRawAddr, rawJmp, sizeof(rawJmp));

			//Write Call TarFunc Code
			rva = uiTarAddr - (DWORD)(&alloc[szRawSize]) - sizeof(tarCal);
			memcpy(&tarCal[1], &rva, sizeof(DWORD));
			memcpy(&alloc[szRawSize], tarCal, sizeof(tarCal));

			//Write Ret Code
			rva = (uiRawAddr + szRawSize) - (DWORD)(&alloc[szRawSize + sizeof(tarCal)]) - sizeof(retJmp);
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

	bool DetourAttachFunc(void* ppRawFunc, void* pNewFunc)
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		LONG erroAttach = DetourAttach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = DetourTransactionCommit();

		if (erroAttach == NO_ERROR && erroCommit == NO_ERROR) return false;

		MessageBoxW(NULL, L"DetourAttachFunc Failed!!", NULL, NULL);

		return true;
	}

	bool DetourDetachFunc(void* ppRawFunc, void* pNewFunc)
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		LONG erroDetach = DetourDetach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = DetourTransactionCommit();

		if (erroDetach == NO_ERROR && erroCommit == NO_ERROR) return false;

		MessageBoxW(NULL, L"DetourDetachFunc Failed!!", NULL, NULL);

		return true;
	}

	bool CreateProcessWithDlls(std::wstring_view wsAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
	{
		return DetourCreateProcessWithDllsW(wsAppName.data(), NULL, NULL, NULL, FALSE, uiFlags, NULL, NULL, lpStartupInfo, lpProcessInformation, nDlls, rlpDlls, NULL);
	}
}