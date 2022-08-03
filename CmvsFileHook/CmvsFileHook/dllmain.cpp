#include <Windows.h>

char* pFileName = 0;
char repFolder[] = ".\\DirA\\";

extern "C" VOID __declspec(dllexport) Dir_A() {}

VOID WriteHookCode(DWORD oldAddr, DWORD tarAddr)
{
    DWORD oldProtect = 0;
    VirtualProtect((LPVOID)oldAddr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

    DWORD rawAddr = tarAddr - oldAddr - 5;
    BYTE code[] = { 0xE9,0x00,0x00,0x00,0x00,0x90 };

    memcpy(&code[1], &rawAddr, 4);
    memcpy((void*)oldAddr, code, 6);
}

BOOL FileExist()
{
    char repPath[MAX_PATH] = { 0 };
    lstrcatA(repPath, repFolder);
    lstrcatA(repPath, pFileName);
    pFileName = 0;

    DWORD isFileExist = GetFileAttributesA(repPath);
    return (isFileExist != INVALID_FILE_ATTRIBUTES &&!(isFileExist & FILE_ATTRIBUTE_DIRECTORY));
}

DWORD oldAddr_PB3 = 0x0043FE8D;
DWORD retAddr_PB3 = oldAddr_PB3 + 0x6;
VOID __declspec(naked) PB3_SetBasePath()
{
    __asm
    {
        add esi, 0x1068
        pushad
        pushfd
        mov eax,dword ptr[ebp - 0x908]
        mov pFileName,eax
    }

    if (FileExist())
    {
        __asm mov dword ptr[ esp + 0x8 ], offset repFolder
    }

    __asm
    {
        popfd
        popad
        jmp retAddr_PB3
    }
}

DWORD oldAddr_PS3 = 0x0046CE10;
DWORD retAddr_PS3 = oldAddr_PS3 + 0x9;
VOID __declspec(naked) PS3_SetBasePath()
{
    __asm
    {
        push ebp
        mov ebp,esp
        sub esp, 0x810
        pushad
        pushfd
        mov pFileName,ebx
    }

    if (FileExist())
    {
        __asm mov dword ptr[esp + 0x1C], offset repFolder
        *(BYTE*)0x0046CEF1 = 0xEB;
        *(BYTE*)0x0046CF1D = 0xEB;
    }
    else
    {
        *(BYTE*)0x0046CEF1 = 0x74;
        *(BYTE*)0x0046CF1D = 0x74;
    }

    __asm
    {
        popfd
        popad
        jmp retAddr_PS3
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        WriteHookCode(oldAddr_PB3, (DWORD)PB3_SetBasePath);
        WriteHookCode(oldAddr_PS3, (DWORD)PS3_SetBasePath);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

