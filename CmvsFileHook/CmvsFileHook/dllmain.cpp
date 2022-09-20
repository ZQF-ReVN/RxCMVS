#include <Windows.h>
#include "CMVSVersion.h"


VOID StartHook()
{

#ifdef CMVS_326_
	SetHook(PS3ADDR, (DWORD)CMVS_326_PS3Hook, 0x9);
	SetHook(PB3ADDR, (DWORD)CMVS_326_PB3Hook, 0x6);
#endif // CMVS_326_

#ifdef CMVS_380_
	SetHook(PS3ADDR, (DWORD)CMVS_380_PS3Hook, 0x9);
	SetHook(PB3ADDR, (DWORD)CMVS_380__PB3Hook, 0x6);
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