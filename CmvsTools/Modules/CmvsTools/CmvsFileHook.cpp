#include "CmvsFileHook.h"
#include "../../ThirdParty/TDA/MemX.h"

#include <Windows.h>


namespace CmvsTools
{
	namespace CmvsFileHook
	{
		static DWORD g_dwPS3Raw = NULL;
		static DWORD g_dwPB3Raw = NULL;
		static PBYTE g_dwPS3Dec = NULL;
		static PBYTE g_dwPS3Com = NULL;

		static PCHAR g_pFileName = 0;
		static CHAR g_aHookPath[MAX_PATH] = { 0 };
		static CHAR g_aHookFolder[] = ".\\FileHook\\";


		BOOL FileExist()
		{
			lstrcpyA(g_aHookPath, g_aHookFolder); lstrcatA(g_aHookPath, g_pFileName);

			DWORD isFileExist = GetFileAttributesA(g_aHookPath);
			if ((isFileExist != INVALID_FILE_ATTRIBUTES) && (isFileExist != FILE_ATTRIBUTE_DIRECTORY)) return TRUE;

			return FALSE;
		}

		VOID __declspec(naked)CMVS_210_PS2Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov g_pFileName, edi;
				test ebx, ebx;
				je is_EBX_NULL;
				mov g_pFileName, ebx;
			is_EBX_NULL:
			}

			if (FileExist())
			{
				__asm
				{
					mov dword ptr[esp + 0x18], offset g_aHookFolder;
				}
				*g_dwPS3Dec = 0xEB;
				*g_dwPS3Com = 0xEB;
			}
			else
			{
				*g_dwPS3Dec = 0x76;
				*g_dwPS3Com = 0x74;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}

		VOID __declspec(naked)CMVS_210_PB3Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov g_pFileName, ebp;
			}

			if (FileExist())
			{
				__asm mov dword ptr[esp], offset g_aHookFolder;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}


		VOID __declspec(naked)CMVS_342_PS3Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov eax, dword ptr[esp + 0x10];
				mov g_pFileName, eax;
			}

			if (FileExist())
			{
				__asm mov dword ptr[esp + 0x18], offset g_aHookFolder;
				*g_dwPS3Dec = 0xEB;
				*g_dwPS3Com = 0xEB;
			}
			else
			{
				*g_dwPS3Dec = 0x74;
				*g_dwPS3Com = 0x74;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}

		VOID __declspec(naked)CMVS_342_PB3Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov g_pFileName, ebx;
			}

			if (FileExist())
			{
				__asm mov dword ptr[esp], offset g_aHookFolder;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}


		VOID __declspec(naked)CMVS_380_PS3Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov eax, dword ptr[esp + 0x10];
				mov g_pFileName, eax;
			}

			if (FileExist())
			{
				__asm mov dword ptr[esp + 0x18], offset g_aHookFolder;
				*g_dwPS3Dec = 0xEB;
				*g_dwPS3Com = 0xEB;
			}
			else
			{
				*g_dwPS3Dec = 0x74;
				*g_dwPS3Com = 0x74;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}

		VOID __declspec(naked)CMVS_380_PB3Hook()
		{
			__asm
			{
				pushfd;
				pushad;
				mov eax, dword ptr[esp + 0x2C];
				mov g_pFileName, eax;
			}

			if (FileExist())
			{
				__asm mov dword ptr[esp + 0x4], offset g_aHookFolder;
			}

			__asm
			{
				popad;
				popfd;
				ret;
			}
		}


		void SetPS3Hook_210_(unsigned int uiPS2Raw, unsigned int uiPS2Dec, unsigned int uiPS2Com)
		{
			g_dwPB3Raw = uiPS2Raw;
			g_dwPS3Dec = (PBYTE)uiPS2Dec;
			g_dwPS3Com = (PBYTE)uiPS2Com;

			TDA::MemX::SetHook((DWORD)uiPS2Raw, (DWORD)CMVS_210_PS2Hook, 0x6);
		}

		void SetPB3Hook_210_(unsigned int uiPB3Raw)
		{
			TDA::MemX::SetHook((DWORD)uiPB3Raw, (DWORD)CMVS_210_PB3Hook, 0x6);
		}

		void SetPS3Hook_342_(unsigned int uiPS3Raw, unsigned int uiPS3Dec, unsigned int uiPS3Com)
		{
			g_dwPB3Raw = uiPS3Raw;
			g_dwPS3Dec = (PBYTE)uiPS3Dec;
			g_dwPS3Com = (PBYTE)uiPS3Com;

			TDA::MemX::SetHook((DWORD)uiPS3Raw, (DWORD)CMVS_342_PS3Hook, 0x9);
		}

		void SetPB3Hook_342_(unsigned int uiPB3Raw)
		{
			TDA::MemX::SetHook((DWORD)uiPB3Raw, (DWORD)CMVS_342_PB3Hook, 0x6);
		}

		void SetPS3Hook_380_(unsigned int uiPS3Raw, unsigned int uiPS3Dec, unsigned int uiPS3Com)
		{
			g_dwPB3Raw = uiPS3Raw;
			g_dwPS3Dec = (PBYTE)uiPS3Dec;
			g_dwPS3Com = (PBYTE)uiPS3Com;

			TDA::MemX::SetHook((DWORD)uiPS3Raw, (DWORD)CMVS_380_PS3Hook, 0x9);
		}

		void SetPB3Hook_380_(unsigned int uiPB3Raw)
		{
			TDA::MemX::SetHook((DWORD)uiPB3Raw, (DWORD)CMVS_380_PB3Hook, 0x6);
		}
	}
}
