#include "VFS_Hook.h"
#include "CMVS_Types.h"
#include "../../RxHook/RxHook.h"

#include <Windows.h>


namespace CMVS::VFS
{
	static char sg_aHookFolder[MAX_PATH] = { 0 };
	static Fn_ScriptReader sg_fnScriptReader = nullptr;
	static Fn_ReadFullData sg_fnReadFullData = nullptr;
	static Fn_ImageReader sg_fnImageReader = nullptr;
	static Fn_ImageDecoder sg_fnImageDecoder = nullptr;


	static HLOCAL LoadFile(char* cpScriptName, uint32_t* pSize_Ret)
	{
		char full_path[MAX_PATH];
		lstrcpyA(full_path, sg_aHookFolder); lstrcatA(full_path, cpScriptName);
		return sg_fnReadFullData(full_path, pSize_Ret);
	}

	static HLOCAL __fastcall ScriptReader_Hook(VFS_Entry* pEntry, uint32_t uiEDX, char* cpScriptName, uint32_t* pSize_Ret)
	{
		HLOCAL buffer = LoadFile(cpScriptName, pSize_Ret);
		return buffer != nullptr ? (buffer) : (sg_fnScriptReader(pEntry, cpScriptName, pSize_Ret));
	}

	static BOOL __fastcall ImageReader_Hook(uint32_t* This, uint32_t uiEDX, Image* pCImage, char* cpPrefixPath, char* cpImageName, uint32_t pUn0)
	{
		uint32_t* flag_ptr = (uint32_t*)(This + 0x1824);
		if (*flag_ptr == 0)
		{
			uint32_t data_size = 0;
			HLOCAL data_ptr = LoadFile(cpImageName, &data_size);
			if (data_ptr)
			{
				return sg_fnImageDecoder(This, pCImage, data_ptr, data_size, cpPrefixPath, pUn0);
			}
		}
		return sg_fnImageReader(This, pCImage, cpPrefixPath, cpImageName, pUn0);
	}


    // *******************
    // *   EXPORT FUNC   *
    // *******************
	void SetHookFolder(const char* cpFolder)
	{
		lstrcpyA(sg_aHookFolder, cpFolder);
	}

	void SetFnReadFullData(uint32_t fnReadFullData)
	{
		sg_fnReadFullData = (Fn_ReadFullData)fnReadFullData;
	}

	void SetFnImageDecoder(uint32_t fnImageDecoder)
	{
		sg_fnImageDecoder = (Fn_ImageDecoder)fnImageDecoder;
	}

	void HookPS3_V380(uint32_t fnScriptReader)
	{
		sg_fnScriptReader = (Fn_ScriptReader)fnScriptReader;
		Rut::RxHook::DetourAttachFunc(&sg_fnScriptReader, ScriptReader_Hook);
	}

	void HookPB3_V380(uint32_t fnImageReader)
	{
		sg_fnImageReader = (Fn_ImageReader)fnImageReader;
		Rut::RxHook::DetourAttachFunc(&sg_fnImageReader, ImageReader_Hook);
	}
}
