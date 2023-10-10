#pragma once
#include <string>
#include <cstdint>
#include <Windows.h>


namespace Rut::RxHook
{
	void SetHookCode_Jmp(std::uintptr_t uiRawAddress, std::uintptr_t uiNewAddress, std::size_t szHookCode);
	void SetHookCode_Call(std::uintptr_t uiRawAddress, std::uintptr_t uiNewAddress, std::size_t szHookCode);
	bool SetHook(uint32_t uiRawAddr, uint32_t uiTarAddr, uint32_t szRawSize);
	bool DetourAttachFunc(void* ppRawFunc, void* pNewFunc);
	bool DetourDetachFunc(void* ppRawFunc, void* pNewFunc);

	bool CreateProcessWithDlls(std::wstring_view wsAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW pStartupInfo, LPPROCESS_INFORMATION pProcessInformation);
}