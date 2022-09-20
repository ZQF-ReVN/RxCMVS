#include "Tools.h"

PCHAR g_pFileName = 0;
CHAR g_sReadFolder[] = ".\\DirA\\";

BOOL FileExist()
{
	CHAR repPath[MAX_PATH] = { 0 };
	lstrcatA(repPath, g_sReadFolder);
	lstrcatA(repPath, g_pFileName);

	DWORD isFileExist = GetFileAttributesA(repPath);
	return (isFileExist != INVALID_FILE_ATTRIBUTES && !(isFileExist & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize)
{
	BOOL isProtect = 0;
	DWORD oldProtect = 0;
	DWORD allocateAddr = 0;
	DWORD relativeAddr = 0;
	BYTE rawJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE retJmp[] = { 0xE9,0x00,0x00,0x00,0x00 };
	BYTE tarCal[] = { 0xE8,0x00,0x00,0x00,0x00 };

	isProtect = VirtualProtect((LPVOID)dwRawAddr, sizeof(rawJmp), PAGE_EXECUTE_READWRITE, &oldProtect);
	allocateAddr = (DWORD)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocateAddr && isProtect)
	{
		//Copy the Code for the original address to alloc address
		memcpy((PVOID)allocateAddr, (PVOID)dwRawAddr, szRawSize);

		//Write Jmp Code
		relativeAddr = allocateAddr - dwRawAddr - sizeof(rawJmp);
		memcpy(&rawJmp[1], &relativeAddr, sizeof(DWORD));
		memcpy((void*)dwRawAddr, rawJmp, sizeof(rawJmp));

		//Write Call TarFunc Code
		relativeAddr = dwTarAddr - (allocateAddr + szRawSize) - sizeof(tarCal);
		memcpy(&tarCal[1], &relativeAddr, sizeof(DWORD));
		memcpy((PVOID)(allocateAddr + szRawSize), tarCal, sizeof(tarCal));

		//Write Ret Code
		relativeAddr = (dwRawAddr + szRawSize) - (allocateAddr + szRawSize + sizeof(tarCal)) - sizeof(retJmp);
		memcpy(&retJmp[1], &relativeAddr, sizeof(DWORD));
		memcpy((PVOID)(allocateAddr + szRawSize + sizeof(tarCal)), retJmp, sizeof(retJmp));

		return TRUE;
	}
	else
	{
		MessageBoxW(NULL, L"SetHook Failed!!", NULL, NULL);
		return FALSE;
	}

}
