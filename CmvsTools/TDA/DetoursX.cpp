#include "DetoursX.h"

#include "../ThirdParty/detours/include/detours.h"
#pragma comment(lib,"../ThirdParty/detours/lib.X86/detours.lib")


namespace TDA
{
	BOOL DetoursX::DetourAttachFunc(PVOID ppRawFunc, PVOID pNewFunc)
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		LONG erroAttach = DetourAttach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = DetourTransactionCommit();

		if (erroAttach == NO_ERROR && erroCommit == NO_ERROR) return false;

		MessageBoxW(NULL, L"DetourAttachFunc Failed!!", NULL, NULL);

		return true;
	}

	BOOL DetoursX::DetourDetachFunc(PVOID ppRawFunc, PVOID pNewFunc)
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		LONG erroDetach = DetourDetach((PVOID*)ppRawFunc, pNewFunc);
		LONG erroCommit = DetourTransactionCommit();

		if (erroDetach == NO_ERROR && erroCommit == NO_ERROR) return false;

		MessageBoxW(NULL, L"DetourDetachFunc Failed!!", NULL, NULL);

		return true;
	}

	//********************
	//* Hook CreateFontA *
	//********************
	static DWORD g_dwCharSet = 0x86;
	static LPCSTR g_lpFontName = "ºÚÌå";

	typedef HFONT(WINAPI* pCreateFontA)(INT, INT, INT, INT, INT, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR);
	pCreateFontA rawCreateFontA = CreateFontA;

	HFONT WINAPI newCreateFontA
	(
		INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight,
		DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
		DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName
	)
	{
		return rawCreateFontA
		(
			cHeight, cWidth, cEscapement, cOrientation, cWeight,
			bItalic, bUnderline, bStrikeOut, g_dwCharSet, iOutPrecision,
			iClipPrecision, iQuality, iPitchAndFamily, g_lpFontName
		);
	}

	BOOL DetoursX::HookCreateFontA(DWORD dwCharSet, LPCSTR lpFontName)
	{
		g_dwCharSet = dwCharSet;
		g_lpFontName = lpFontName;
		return DetourAttachFunc(&rawCreateFontA, newCreateFontA);
	}


	//****************************
	//* Hook CreateFontIndirectA *
	//****************************
	typedef HFONT(WINAPI* pCreateFontIndirectA)(const LOGFONTA* lplf);
	pCreateFontIndirectA rawCreateFontIndirectA = CreateFontIndirectA;

	HFONT WINAPI newCreateFontIndirectA(LOGFONTA* lplf)
	{
		lplf->lfCharSet = (BYTE)g_dwCharSet;
		strcpy_s(lplf->lfFaceName, g_lpFontName);
		return rawCreateFontIndirectA(lplf);
	}

	BOOL DetoursX::HookCreateFontIndirectA(DWORD dwCharSet, LPCSTR lpFontName)
	{
		g_dwCharSet = dwCharSet;
		g_lpFontName = lpFontName;
		return DetourAttachFunc(&rawCreateFontIndirectA, newCreateFontIndirectA);
	}


	//************************
	//* Hook CreateWindowExA *
	//************************
	static LPCSTR g_lpPatchTitle = NULL;
	static LPCSTR g_lpRawTitle = NULL;

	typedef HWND(WINAPI* pCreateWindowExA)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
	pCreateWindowExA RawCreateWindowExA = CreateWindowExA;

	HWND WINAPI NewCreateWindowExA
	(
		DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam
	)
	{
		if (!lstrcmpA(lpWindowName, g_lpRawTitle))
		{
			lpWindowName = g_lpPatchTitle;
			TDA::DetoursX::DetourDetachFunc(&RawCreateWindowExA, &NewCreateWindowExA);
		}

		return RawCreateWindowExA
		(
			dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam
		);
	}

	BOOL DetoursX::HookTitleExA(LPCSTR lpRawTitle, LPCSTR lpPatchTitle)
	{
		g_lpPatchTitle = lpPatchTitle;
		g_lpRawTitle = lpRawTitle;
		return DetourAttachFunc(&RawCreateWindowExA, NewCreateWindowExA);
	}
}