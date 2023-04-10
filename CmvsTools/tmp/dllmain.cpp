#include <Windows.h>
#include "CMVSVersion.h"
#include "FileDump.h"
#include "Tools.h"

VOID EnableDebugOutLog()
{
	g_isDebug_FilePath = TRUE;
	g_isDebug_FileDump = TRUE;

	SetConsole();
}

VOID FilePathHook()
{
	CreateDirectoryA(g_sReadFolder, NULL);
#ifdef CMVS_210_
	SetHook(SCRIPTADDR, (DWORD)CMVS_210_PS2Hook, 0x6);
	SetHook(IMAGEADDR, (DWORD)CMVS_210_PB3Hook, 0x6);
#endif // CMVS_342_

#ifdef CMVS_342_
	SetHook(SCRIPTADDR, (DWORD)CMVS_342_PS3Hook, 0x9);
	SetHook(IMAGEADDR, (DWORD)CMVS_342_PB3Hook, 0x6);
#endif // CMVS_342_

#ifdef CMVS_380_
	SetHook(SCRIPTADDR, (DWORD)CMVS_380_PS3Hook, 0x9);
	SetHook(IMAGEADDR, (DWORD)CMVS_380_PB3Hook, 0x6);
#endif // CMVS_380_
}

VOID FileDumpHook()
{
	DumpFile();
}

VOID StartHook()
{
	//FileDumpHook();
	FilePathHook();

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
