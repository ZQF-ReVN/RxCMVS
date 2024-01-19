#pragma once
#include <string>
#include <cstdint>
#include <Windows.h>


namespace Rut::RxHook
{
	class Transfer
	{
	public:
		static void Set(uint8_t ucAsmCode, void* pRawAddress, void* pNewAddress, size_t szOrgCode);
		static void Set(void* pRawAddress, void* pNewAddress, size_t szOrgCode);
	};

	class Detours
	{
	public:
		static void Begin();
		static void Attach(void* ppRawFunc, void* pNewFunc);
		static void Detach(void* ppRawFunc, void* pNewFunc);
		static void Commit();
		static void AttrachDirectly(void* ppRawFunc, void* pNewFunc);
	};


	bool CreateProcessWithDlls(const wchar_t* wpAppName, uint32_t uiFlags, uint32_t nDlls, const char** rlpDlls, LPSTARTUPINFOW pStartupInfo, LPPROCESS_INFORMATION pProcessInformation);
}