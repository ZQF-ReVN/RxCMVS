#include "Hook.h"
#include "Mem.h"
#include "ErrorMsg.h"

#include <Windows.h>
#include <detours.h>


namespace Rut::RxHook
{
	void Transfer::Set(uint8_t ucAsmCode, void* pRawAddress, void* pNewAddress, size_t szOrgCode)
	{
		SysMemAccess(pRawAddress, szOrgCode, PAGE_EXECUTE_READWRITE, nullptr, L"Transfer::Set Failed!", true);

		uint8_t asm_code[0x5] = { ucAsmCode, 0x00, 0x00, 0x00, 0x00 };
		size_t* addr_ptr = (size_t*)(asm_code + 1);
		*addr_ptr = (size_t)pNewAddress - (size_t)pRawAddress - sizeof(asm_code);
		memcpy(pRawAddress, asm_code, sizeof(asm_code));

		(szOrgCode > 0x5) ? (void)memset((uint8_t*)pRawAddress + 0x5, 0x90, szOrgCode - 0x5) : (void)nullptr;
	}

	void Transfer::Set(void* pRawAddress, void* pNewAddress, size_t szOrgCode)
	{
		size_t rva = 0;
		uint8_t raw_jmp_asm[] = { 0xE9,0x00,0x00,0x00,0x00 };
		uint8_t ret_jmp_asm[] = { 0xE9,0x00,0x00,0x00,0x00 };
		uint8_t tar_cal_asm[] = { 0xE8,0x00,0x00,0x00,0x00 };

		SysMemAccess(pRawAddress, 0x1000, PAGE_EXECUTE_READWRITE, nullptr, L"Transfer::Set SysMemAccess Failed!", true);

		uint8_t* code_buffer = (uint8_t*)SysMemAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE, L"Transfer::Set SysMemAlloc Failed!", true);

		//Copy the Code for the original address to alloc address
		memcpy(code_buffer, pRawAddress, szOrgCode);

		//Write Jmp Code
		rva = (size_t)code_buffer - (size_t)pRawAddress - sizeof(raw_jmp_asm);
		memcpy(raw_jmp_asm + 1, &rva, sizeof(size_t));
		memcpy(pRawAddress, raw_jmp_asm, sizeof(raw_jmp_asm));

		//Write Call TarFunc Code
		rva = (size_t)pNewAddress - (size_t)(code_buffer + szOrgCode) - sizeof(tar_cal_asm);
		memcpy(tar_cal_asm + 1, &rva, sizeof(size_t));
		memcpy(code_buffer + szOrgCode, tar_cal_asm, sizeof(tar_cal_asm));

		//Write Ret Code
		rva = ((size_t)pRawAddress + szOrgCode) - (size_t)(code_buffer + szOrgCode + sizeof(tar_cal_asm)) - sizeof(ret_jmp_asm);
		memcpy(ret_jmp_asm + 1, &rva, sizeof(size_t));
		memcpy(code_buffer + szOrgCode + sizeof(tar_cal_asm), ret_jmp_asm, sizeof(ret_jmp_asm));
	}


	void Detours::Begin()
	{
		::DetourRestoreAfterWith();
		::DetourTransactionBegin();
		::DetourUpdateThread(GetCurrentThread());
	}

	void Detours::Attach(void* ppRawFunc, void* pNewFunc)
	{
		::DetourAttach((PVOID*)ppRawFunc, pNewFunc) != NO_ERROR ? SysErrorMsgBox(L"DetourAttach Failed!!", true) : (void)(0);
	}

	void Detours::Detach(void* ppRawFunc, void* pNewFunc)
	{
		::DetourDetach((PVOID*)ppRawFunc, pNewFunc) != NO_ERROR ? SysErrorMsgBox(L"DetourDetach Failed!!", true) : (void)(0);
	}

	void Detours::Commit()
	{
		::DetourTransactionCommit() != NO_ERROR ? SysErrorMsgBox(L"DetourTransactionCommit Failed!!", true) : (void)(0);
	}

	void Detours::AttrachDirectly(void* ppRawFunc, void* pNewFunc)
	{
		Detours::Begin();
		Detours::Attach(ppRawFunc, pNewFunc);
		Detours::Commit();
	}

	bool CreateProcessWithDlls(const wchar_t* wpAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
	{
		return ::DetourCreateProcessWithDllsW(wpAppName, NULL, NULL, NULL, FALSE, uiFlags, NULL, NULL, lpStartupInfo, lpProcessInformation, nDlls, rlpDlls, NULL);
	}

}