#pragma once
#include <Windows.h>

extern PCHAR g_pFileName;
extern CHAR g_sReadFolder[];

BOOL FileExist();
BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize);