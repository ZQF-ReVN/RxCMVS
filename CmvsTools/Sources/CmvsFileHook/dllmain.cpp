#include <Windows.h>

#include "../../Modules/CmvsTools/CmvsFileHook.h"

using namespace CmvsTools::CmvsFileHook;


static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);


VOID StartHook()
{
	SetPS3Hook_380_(g_dwExeBase + 0x6CE10, g_dwExeBase + 0x6CEF1, g_dwExeBase + 0x6CF1D);
	SetPB3Hook_380_(g_dwExeBase + 0x3FE8D);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" VOID __declspec(dllexport) Dir_A() {}