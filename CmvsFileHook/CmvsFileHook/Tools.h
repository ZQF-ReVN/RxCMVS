#pragma once
#include <Windows.h>
#include <iostream>

extern BOOL g_bDebugFlag;
extern PCHAR g_pFileName;
extern CHAR g_sReadFolder[];

VOID WINAPI SetConsole();
BOOL FileExist();
BOOL SetHook(DWORD dwRawAddr, DWORD dwTarAddr, SIZE_T szRawSize);