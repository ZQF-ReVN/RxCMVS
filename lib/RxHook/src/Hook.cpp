#include "Hook.h"
#include "Mem.h"
#include "ErrorMsg.h"

#include <Windows.h>
#include <detours.h>


namespace Rut::RxHook
{
	void SetHookCode_Jmp(void* pRawAddress, void* pNewAddress, size_t szHookCode)
	{
		SysMemAccess(pRawAddress, szHookCode, PAGE_EXECUTE_READWRITE, nullptr, L"SetHookCode_Jmp Failed!", true);

		uint8_t asm_code[0x5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
		size_t* addr_ptr = (size_t*)(asm_code + 1);
		*addr_ptr = (size_t)pNewAddress - (size_t)pRawAddress - sizeof(asm_code);
		memcpy(pRawAddress, asm_code, sizeof(asm_code));

		(szHookCode > 0x5) ? (void)memset((uint8_t*)pRawAddress + 0x5, 0x90, szHookCode - 0x5) : (void)nullptr;
	}

	void SetHookCode_Call(void* pRawAddress, void* pNewAddress, size_t szHookCode)
	{
		SysMemAccess(pRawAddress, szHookCode, PAGE_EXECUTE_READWRITE, nullptr, L"SetHookCode_Call Failed!", true);

		uint8_t asm_code[0x5] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
		size_t* addr_ptr = (size_t*)(asm_code + 1);
		*addr_ptr = (size_t)pNewAddress - (size_t)pRawAddress - sizeof(asm_code);
		memcpy(pRawAddress, asm_code, sizeof(asm_code));

		(szHookCode > 0x5) ? (void)memset((uint8_t*)pRawAddress + 0x5, 0x90, szHookCode - 0x5) : void();
	}

	void SetHook(void* uiRawAddr, void* uiTarAddr, size_t szRawSize)
	{
		size_t rva = 0;
		uint8_t raw_jmp_asm[] = { 0xE9,0x00,0x00,0x00,0x00 };
		uint8_t ret_jmp_asm[] = { 0xE9,0x00,0x00,0x00,0x00 };
		uint8_t tar_cal_asm[] = { 0xE8,0x00,0x00,0x00,0x00 };

		SysMemAccess(uiRawAddr, 0x1000, PAGE_EXECUTE_READWRITE, nullptr, L"SetHook Failed!", true);

		uint8_t* code_buffer = (uint8_t*)SysMemAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE, L"SetHook Failed!!", true);

		//Copy the Code for the original address to alloc address
		memcpy(code_buffer, uiRawAddr, szRawSize);

		//Write Jmp Code
		rva = (size_t)code_buffer - (size_t)uiRawAddr - sizeof(raw_jmp_asm);
		memcpy(raw_jmp_asm + 1, &rva, sizeof(size_t));
		memcpy(uiRawAddr, raw_jmp_asm, sizeof(raw_jmp_asm));

		//Write Call TarFunc Code
		rva = (size_t)uiTarAddr - (size_t)(code_buffer + szRawSize) - sizeof(tar_cal_asm);
		memcpy(tar_cal_asm + 1, &rva, sizeof(size_t));
		memcpy(code_buffer + szRawSize, tar_cal_asm, sizeof(tar_cal_asm));

		//Write Ret Code
		rva = ((size_t)uiRawAddr + szRawSize) - (size_t)(code_buffer + szRawSize + sizeof(tar_cal_asm)) - sizeof(ret_jmp_asm);
		memcpy(ret_jmp_asm + 1, &rva, sizeof(size_t));
		memcpy(code_buffer + szRawSize + sizeof(tar_cal_asm), ret_jmp_asm, sizeof(ret_jmp_asm));
	}

	void DetourAttachFunc(void* ppRawFunc, void* pNewFunc)
	{
		::DetourRestoreAfterWith();
		::DetourTransactionBegin();
		::DetourUpdateThread(GetCurrentThread());

		LONG erroAttach = ::DetourAttach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = ::DetourTransactionCommit();

		if (erroAttach == NO_ERROR && erroCommit == NO_ERROR) return;

		SysErrorMsgBox(L"DetourAttachFunc Failed!!", true);
	}

	void DetourDetachFunc(void* ppRawFunc, void* pNewFunc)
	{
		::DetourRestoreAfterWith();
		::DetourTransactionBegin();
		::DetourUpdateThread(GetCurrentThread());

		LONG erroDetach = ::DetourDetach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = ::DetourTransactionCommit();

		if (erroDetach == NO_ERROR && erroCommit == NO_ERROR) return;

		SysErrorMsgBox(L"DetourDetachFunc Failed!!", true);
	}

	bool CreateProcessWithDlls(const wchar_t* wpAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
	{
		return ::DetourCreateProcessWithDllsW(wpAppName, NULL, NULL, NULL, FALSE, uiFlags, NULL, NULL, lpStartupInfo, lpProcessInformation, nDlls, rlpDlls, NULL);
	}
}