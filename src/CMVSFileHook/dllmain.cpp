#include <Windows.h>
#include <stdexcept>

#include "../../lib/Rut/RxPath.h"
#include "../../lib/RxINI/RxINI.h"
#include "../../lib/CMVSTools/FileHook.h"

using namespace Rut;


VOID StartHook(HMODULE hDllBase)
{
	try
	{
		RxINI::Parser ini(RxPath::NotSuffix(RxPath::ModulePathW(hDllBase)) + L".ini");

		std::wstring seleted_game_name = ini.Get(L"CMVS_File_Hook", L"GameSelected");

		auto& seleted_game = ini.Get(seleted_game_name);
		std::wstring version = seleted_game[L"Engine_Ver"];
		uint32_t script_loa = seleted_game[L"Script_Lod"];
		uint32_t script_xor = seleted_game[L"Script_Xor"];
		uint32_t script_com = seleted_game[L"Script_Com"];
		uint32_t image_load = seleted_game[L"Image_Load"];

		std::string hook_folder = ini.Get(L"CMVS_File_Hook", L"HookFolder");
		CMVS::FileHook::SetHookFolder(hook_folder.c_str());

		if (version == L"CMVS_380_")
		{
			CMVS::FileHook::SetPS3Hook_380_(script_loa, script_xor, script_com);
			CMVS::FileHook::SetPB3Hook_380_(image_load);
		}
		else if (version == L"CMVS_342_")
		{
			CMVS::FileHook::SetPS3Hook_342_(script_loa, script_xor, script_com);
			CMVS::FileHook::SetPB3Hook_342_(image_load);
		}
		else if (version == L"CMVS_210_")
		{
			CMVS::FileHook::SetPS3Hook_210_(script_loa, script_xor, script_com);
			CMVS::FileHook::SetPB3Hook_210_(image_load);
		}
	}
	catch (const std::runtime_error& err)
	{
		MessageBoxA(NULL, err.what(), NULL, NULL);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook(hModule);
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