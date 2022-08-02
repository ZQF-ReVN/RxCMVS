#include <Windows.h>

DWORD oldAddr = 0x00430685;
DWORD retAddr = oldAddr + 0x6;
char patchFolder[] = ".\\DirA\\";
char* fileName;

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

BOOL FileExists()
{
    char catPath[MAX_PATH] = {0};
    strcat_s(catPath, patchFolder);
    strcat_s(catPath, fileName);

    DWORD dwAttrib = GetFileAttributesA(catPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

VOID __declspec(naked) SetBasePath()
{
    __asm
    {
        add edi, 0x1068
        pushad
        pushfd
        mov fileName,ebx
    }

    if (FileExists())
    {
        __asm mov dword ptr[ esp+0x4 ], offset patchFolder;
    }

    __asm
    {
        popfd
        popad
        jmp retAddr
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
        WriteHookCode(oldAddr, (DWORD)SetBasePath);
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

