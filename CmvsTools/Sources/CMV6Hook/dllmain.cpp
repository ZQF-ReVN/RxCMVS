#include <Windows.h>

#include "../../Modules/CmvsTools/CMV6Hook.h"

using namespace CMVSTools::CMV6Hook;


VOID StartHook()
{
	//UnPackCMVThread();
	CMV6DecodeWebP();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

EXTERN_C VOID _declspec(dllexport) DirA(){}

