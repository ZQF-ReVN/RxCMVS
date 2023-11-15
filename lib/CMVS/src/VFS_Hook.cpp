#include "VFS_Hook.h"
#include "CMVS_Types.h"
#include "../../RxHook/RxHook.h"

#include <Windows.h>


namespace CMVS::VFS
{
	static char sg_aHookFolder[MAX_PATH] = { 0 };
	static Fn_ScriptReader sg_fnCMVS_ScriptReader = nullptr;
	static Fn_ReadFullData sg_fnCMVS_ReadFullData = nullptr;
	static Fn_ImageReader sg_fnCMVS_ImageReader = nullptr;
	static Fn_ImageDecoder sg_fnCMVS_ImageDecoder = nullptr;


	static HLOCAL LoadFile(char* cpScriptName, uint32_t* pSize_Ret)
	{
		char full_path[MAX_PATH];
		lstrcpyA(full_path, sg_aHookFolder); lstrcatA(full_path, cpScriptName);
		return sg_fnCMVS_ReadFullData(full_path, pSize_Ret);
	}

	static HLOCAL __fastcall CMVS_ScriptReader_Hook(VFS_Entry* pEntry, uint32_t uiEDX, char* cpScriptName, uint32_t* pSize_Ret)
	{
		HLOCAL buffer = LoadFile(cpScriptName, pSize_Ret);
		return buffer != nullptr ? (buffer) : (sg_fnCMVS_ScriptReader(pEntry, cpScriptName, pSize_Ret));
	}

	static BOOL __fastcall CMVS_ImageReader_Hook(uint32_t* This, uint32_t uiEDX, Image* pCImage, char* cpPrefixPath, char* cpImageName, uint32_t pUn0)
	{
		uint32_t* flag_ptr = (uint32_t*)(This + 0x1824);
		if (*flag_ptr == 0)
		{
			uint32_t data_size = 0;
			HLOCAL data_ptr = LoadFile(cpImageName, &data_size);
			if (data_ptr)
			{
				return sg_fnCMVS_ImageDecoder(This, pCImage, data_ptr, data_size, cpPrefixPath, pUn0);
			}
		}
		return sg_fnCMVS_ImageReader(This, pCImage, cpPrefixPath, cpImageName, pUn0);
	}


	void SetHookFolder(const char* cpFolder)
	{
		lstrcpyA(sg_aHookFolder, cpFolder);
	}

	void SetFnReadFullData(uint32_t fnReadFullData)
	{
		sg_fnCMVS_ReadFullData = (Fn_ReadFullData)fnReadFullData;
	}

	void SetFnImageDecoder(uint32_t fnImageDecoder)
	{
		sg_fnCMVS_ImageDecoder = (Fn_ImageDecoder)fnImageDecoder;
	}

	void HookPS3_V380(uint32_t fnScriptReader)
	{
		sg_fnCMVS_ScriptReader = (Fn_ScriptReader)fnScriptReader;
		Rut::RxHook::DetourAttachFunc(&sg_fnCMVS_ScriptReader, CMVS_ScriptReader_Hook);
	}

	void HookPB3_V380(uint32_t fnImageReader)
	{
		sg_fnCMVS_ImageReader = (Fn_ImageReader)fnImageReader;
		Rut::RxHook::DetourAttachFunc(&sg_fnCMVS_ImageReader, CMVS_ImageReader_Hook);
	}
}
