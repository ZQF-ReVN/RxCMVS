#include <Windows.h>

#include "../../lib/Rxx/INI.h"
#include "../../lib/Rxx/File.h"
#include "../../lib/CmvsTools/CmvsFileHook.h"

using namespace Rcf::INI;
using namespace Rut::FileX;
using namespace CMVSTools::CMVSFileHook;


static DWORD g_dwExeBase = NULL;
static DWORD g_dwDllBase = NULL;


VOID StartHook()
{
	try
	{
		std::wstring dll_name_noext = PathRemoveExtension(GetModuleNameViaBaseW((uint32_t)g_dwDllBase));

		Rcf::INI::INI_File* p_ini = new Rcf::INI::INI_File(dll_name_noext + L".ini");

		std::string hook_folder = p_ini->Get(L"CMVS_File_Hook", L"HookFolder");
		std::wstring seleted_game = p_ini->Get(L"CMVS_File_Hook", L"GameSelected");

		auto& sel_game = p_ini->Get(seleted_game);

		std::wstring version = sel_game[L"Engine_Ver"];
		uint32_t script_loa = sel_game[L"Script_Lod"];
		uint32_t script_xor = sel_game[L"Script_Xor"];
		uint32_t script_com = sel_game[L"Script_Com"];
		uint32_t image_load = sel_game[L"Image_Load"];

		SetHookFolder(hook_folder.c_str());

		if (version == L"CMVS_380_")
		{
			SetPS3Hook_380_(script_loa, script_xor, script_com);
			SetPB3Hook_380_(image_load);
		}
		else if (version == L"CMVS_342_")
		{
			SetPS3Hook_342_(script_loa, script_xor, script_com);
			SetPB3Hook_342_(image_load);
		}
		else if (version == L"CMVS_210_")
		{
			SetPS3Hook_210_(script_loa, script_xor, script_com);
			SetPB3Hook_210_(image_load);
		}

		delete p_ini;
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
		g_dwExeBase = (DWORD)GetModuleHandleW(NULL);
		g_dwDllBase = (DWORD)hModule;
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