#include "CMVSVersion.h"

#ifdef CMVS_210_
VOID __declspec(naked)CMVS_210_PS2Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, edi
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp + 0x18], offset g_sReadFolder
		*(BYTE*)PS2XOR = 0xEB;
		*(BYTE*)PS2Com = 0xEB;
	}
	else
	{
		*(BYTE*)PS2XOR = 0x76;
		*(BYTE*)PS2Com = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_210_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, ebp
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}
#endif // CMVS_210_

#ifdef CMVS_342_
VOID __declspec(naked)CMVS_342_PS3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x10]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp + 0x18], offset g_sReadFolder
		*(BYTE*)PS3XOR = 0xEB;
		*(BYTE*)PS3Com = 0xEB;
	}
	else
	{
		*(BYTE*)PS3XOR = 0x74;
		*(BYTE*)PS3Com = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_342_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov g_pFileName, ebx
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}
#endif // CMVS_342_

#ifdef CMVS_380_
VOID __declspec(naked)CMVS_380_PS3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x10]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp + 0x18], offset g_sReadFolder
		* (BYTE*)PS3XOR = 0xEB;
		*(BYTE*)PS3Com = 0xEB;
	}
	else
	{
		*(BYTE*)PS3XOR = 0x74;
		*(BYTE*)PS3Com = 0x74;
	}

	__asm
	{
		popad
		popfd
		ret
	}
}

VOID __declspec(naked)CMVS_380_PB3Hook()
{
	__asm
	{
		pushfd
		pushad
		mov eax, dword ptr[esp + 0x2C]
		mov g_pFileName, eax
	}

	if (FileExist())
	{
		__asm mov dword ptr[esp + 0x4], offset g_sReadFolder
	}

	__asm
	{
		popad
		popfd
		ret
	}
}
#endif // CMVS_380_