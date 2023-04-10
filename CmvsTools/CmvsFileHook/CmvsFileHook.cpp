#include "CmvsFileHook.h"


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
		je isEbxNULL;
		mov g_pFileName, ebx;
		isEbxNULL :
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


BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize)
{
	DWORD old = 0;
	DWORD rva = 0;
	BYTE rawJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE retJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE tarCal[] = { 0xE8,0x00,0x00,0x00,0x00 };

	BOOL protect = VirtualProtect((LPVOID)dwRawAddr, 0x1000, PAGE_EXECUTE_READWRITE, &old);
	PBYTE alloc = (PBYTE)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (alloc && protect)
	{
		//Copy the Code for the original address to alloc address
		memcpy(alloc, (PVOID)dwRawAddr, szRawSize);

		//Write Jmp Code
		rva = (DWORD)alloc - dwRawAddr - sizeof(rawJmp);
		memcpy(&rawJmp[1], &rva, sizeof(DWORD));
		memcpy((PBYTE)dwRawAddr, rawJmp, sizeof(rawJmp));

		//Write Call TarFunc Code
		rva = dwTarAddr - (DWORD)(&alloc[szRawSize]) - sizeof(tarCal);
		memcpy(&tarCal[1], &rva, sizeof(DWORD));
		memcpy(&alloc[szRawSize], tarCal, sizeof(tarCal));

		//Write Ret Code
		rva = (dwRawAddr + szRawSize) - (DWORD)(&alloc[szRawSize + sizeof(tarCal)]) - sizeof(retJmp);
		memcpy(&retJmp[1], &rva, sizeof(DWORD));
		memcpy(&alloc[szRawSize + sizeof(tarCal)], retJmp, sizeof(retJmp));

		return TRUE;
	}
	else
	{
		MessageBoxW(NULL, L"SetHook Failed!!", NULL, NULL);
		return FALSE;
	}
}


VOID SetPS3Hook_210_(DWORD dwPS2Raw, DWORD dwPS2Dec, DWORD dwPS2Com)
{
	g_dwPB3Raw = dwPS2Raw;
	g_dwPS3Dec = (PBYTE)dwPS2Dec;
	g_dwPS3Com = (PBYTE)dwPS2Com;

	SetHook(dwPS2Raw, (DWORD)CMVS_210_PS2Hook, 0x6);
}

VOID SetPB3Hook_210_(DWORD dwPB3Raw)
{
	SetHook(dwPB3Raw, (DWORD)CMVS_210_PB3Hook, 0x6);
}

VOID SetPS3Hook_342_(DWORD dwPS3Raw, DWORD dwPS3Dec, DWORD dwPS3Com)
{
	g_dwPB3Raw = dwPS3Raw;
	g_dwPS3Dec = (PBYTE)dwPS3Dec;
	g_dwPS3Com = (PBYTE)dwPS3Com;

	SetHook(dwPS3Raw, (DWORD)CMVS_342_PS3Hook, 0x9);
}

VOID SetPB3Hook_342_(DWORD dwPB3Raw)
{
	SetHook(dwPB3Raw, (DWORD)CMVS_342_PB3Hook, 0x6);
}

VOID SetPS3Hook_380_(DWORD dwPS3Raw, DWORD dwPS3Dec, DWORD dwPS3Com)
{
	g_dwPB3Raw = dwPS3Raw;
	g_dwPS3Dec = (PBYTE)dwPS3Dec;
	g_dwPS3Com = (PBYTE)dwPS3Com;

	SetHook(dwPS3Raw, (DWORD)CMVS_380_PS3Hook, 0x9);
}

VOID SetPB3Hook_380_(DWORD dwPB3Raw)
{
	SetHook(dwPB3Raw, (DWORD)CMVS_380_PB3Hook, 0x6);
}