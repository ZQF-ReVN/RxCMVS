#include "CMVSVersion.h"

VOID __declspec(naked)CMVS_326_PS3Hook()
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

VOID __declspec(naked)CMVS_326_PB3Hook()
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

VOID __declspec(naked)CMVS_380__PB3Hook()
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