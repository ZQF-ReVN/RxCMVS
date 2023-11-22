#pragma once
#include <string>
#include <cstdint>
#include <Windows.h>


namespace Rut::RxHook
{
	void SetHookCode_Jmp(void* pRawAddress, void* pNewAddress, size_t szHookCode);
	void SetHookCode_Call(void* pRawAddress, void* pNewAddress, size_t szHookCode);
	void SetHook(void* uiRawAddr, void* uiTarAddr, size_t szRawSize);

	void DetourAttachFunc(void* ppRawFunc, void* pNewFunc);
	void DetourDetachFunc(void* ppRawFunc, void* pNewFunc);

	bool CreateProcessWithDlls(const wchar_t* wpAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW pStartupInfo, LPPROCESS_INFORMATION pProcessInformation);
}