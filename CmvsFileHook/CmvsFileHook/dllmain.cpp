#include <Windows.h>
#include "CMVSVersion.h"


VOID StartHook()
{
#ifdef CMVS_210_
	SetHook(PS2ADDR, (DWORD)CMVS_210_PS2Hook, 0x6);
	SetHook(PB3ADDR_Ver210, (DWORD)CMVS_210_PB3Hook, 0x6);
#endif // CMVS_342_

#ifdef CMVS_342_
	SetHook(PS3ADDR, (DWORD)CMVS_342_PS3Hook, 0x9);
	SetHook(PB3ADDR, (DWORD)CMVS_342_PB3Hook, 0x6);
#endif // CMVS_342_

#ifdef CMVS_380_
	SetHook(PS3ADDR, (DWORD)CMVS_380_PS3Hook, 0x9);
	SetHook(PB3ADDR, (DWORD)CMVS_380_PB3Hook, 0x6);
#endif // CMVS_380_

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