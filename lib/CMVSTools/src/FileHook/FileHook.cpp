#include "FileHook.h"
#include "../../../RxHook/RxHook.h"

#include <Windows.h>


namespace CMVS::FileHook
{
	static uint32_t sg_fnLoadImage = 0;
	static uint32_t sg_fnLoadScript = 0;
	static uint8_t* sg_asDecodeScript = nullptr;
	static uint8_t* sg_asDecompScript = nullptr;

	static char* sg_cpFileName = nullptr;
	static char sg_aHookPath[MAX_PATH] = { 0 };
	static char sg_aHookFolder[MAX_PATH] = "FileHook\\";


	bool FileExist()
	{
		lstrcpyA(sg_aHookPath, sg_aHookFolder); lstrcatA(sg_aHookPath, sg_cpFileName);
		return (GetFileAttributesA(sg_aHookPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}

	void __declspec(naked)CMVS_210_PS2Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov sg_cpFileName, edi;
			test ebx, ebx;
			je IS_EBX_NULL;
			mov sg_cpFileName, ebx;
		IS_EBX_NULL:
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp + 0x18], offset sg_aHookFolder;
			*sg_asDecodeScript = 0xEB;
			*sg_asDecompScript = 0xEB;
		}
		else
		{
			*sg_asDecodeScript = 0x76;
			*sg_asDecompScript = 0x74;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}

	void __declspec(naked)CMVS_210_PB3Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov sg_cpFileName, ebp;
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp], offset sg_aHookFolder;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}


	void __declspec(naked)CMVS_342_PS3Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov eax, dword ptr[esp + 0x10];
			mov sg_cpFileName, eax;
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp + 0x18], offset sg_aHookFolder;
			*sg_asDecodeScript = 0xEB;
			*sg_asDecompScript = 0xEB;
		}
		else
		{
			*sg_asDecodeScript = 0x74;
			*sg_asDecompScript = 0x74;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}

	void __declspec(naked)CMVS_342_PB3Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov sg_cpFileName, ebx;
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp], offset sg_aHookFolder;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}


	void __declspec(naked)CMVS_380_PS3Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov eax, dword ptr[esp + 0x10];
			mov sg_cpFileName, eax;
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp + 0x18], offset sg_aHookFolder;
			*sg_asDecodeScript = 0xEB;
			*sg_asDecompScript = 0xEB;
		}
		else
		{
			*sg_asDecodeScript = 0x74;
			*sg_asDecompScript = 0x74;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}

	void __declspec(naked)CMVS_380_PB3Hook()
	{
		__asm
		{
			pushfd;
			pushad;
			mov eax, dword ptr[esp + 0x2C];
			mov sg_cpFileName, eax;
		}

		if (FileExist())
		{
			__asm mov dword ptr[esp + 0x4], offset sg_aHookFolder;
		}

		__asm
		{
			popad;
			popfd;
			ret;
		}
	}


	void SetHookFolder(const char* cpFolder)
	{
		strcpy_s(sg_aHookFolder, MAX_PATH, cpFolder);
	}

	void SetPS3Hook_210_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1)
	{
		sg_fnLoadImage = fnLoadScript;
		sg_asDecodeScript = (uint8_t*)asDecScript_0;
		sg_asDecompScript = (uint8_t*)asDecScript_1;

		Rut::RxHook::SetHook(fnLoadScript, (uint32_t)CMVS_210_PS2Hook, 0x6);
	}

	void SetPB3Hook_210_(uint32_t fnLoadImage)
	{
		Rut::RxHook::SetHook(fnLoadImage, (uint32_t)CMVS_210_PB3Hook, 0x6);
	}

	void SetPS3Hook_342_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1)
	{
		sg_fnLoadImage = fnLoadScript;
		sg_asDecodeScript = (uint8_t*)asDecScript_0;
		sg_asDecompScript = (uint8_t*)asDecScript_1;

		Rut::RxHook::SetHook(fnLoadScript, (uint32_t)CMVS_342_PS3Hook, 0x9);
	}

	void SetPB3Hook_342_(uint32_t fnLoadImage)
	{
		Rut::RxHook::SetHook(fnLoadImage, (uint32_t)CMVS_342_PB3Hook, 0x6);
	}

	void SetPS3Hook_380_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1)
	{
		sg_fnLoadImage = fnLoadScript;
		sg_asDecodeScript = (uint8_t*)asDecScript_0;
		sg_asDecompScript = (uint8_t*)asDecScript_1;

		Rut::RxHook::SetHook(fnLoadScript, (uint32_t)CMVS_380_PS3Hook, 0x9);
	}

	void SetPB3Hook_380_(uint32_t fnLoadImage)
	{
		Rut::RxHook::SetHook(fnLoadImage, (uint32_t)CMVS_380_PB3Hook, 0x6);
	}
}
