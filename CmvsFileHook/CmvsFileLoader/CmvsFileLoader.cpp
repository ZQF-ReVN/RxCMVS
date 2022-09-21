#include <Windows.h>
#include <detours.h>

STARTUPINFOW si;
PROCESS_INFORMATION pi;

INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	LPCWSTR applicationName = L"cmvs32.exe";
	LPCSTR dllName = "CmvsFileHook.dll";
	if (DetourCreateProcessWithDllW(applicationName, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi, dllName, NULL))
	{
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBoxW(NULL, L"CreateProcess Failed!", NULL, MB_OK);
	}

}