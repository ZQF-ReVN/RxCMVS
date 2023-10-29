#include "API.h"
#include "Hook.h"
#include "API_DEF.h"

#include <Windows.h>


namespace Rut::RxHook
{
	//*********Start Hook CreateFontA*******
	static DWORD sg_dwCharSetA = 0;
	static LPCSTR sg_lpFontNameA = nullptr;
	static Fn_CreateFontA sg_fnCreateFontA = CreateFontA;
	HFONT WINAPI CreateFontA_Hook(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
	{
		iCharSet = sg_dwCharSetA;
		pszFaceName = sg_lpFontNameA;
		return sg_fnCreateFontA(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
	}

	bool HookCreateFontA(const uint32_t uiCharSet, const char* cpFontName)
	{
		sg_dwCharSetA = uiCharSet;
		sg_lpFontNameA = cpFontName;
		return DetourAttachFunc(&sg_fnCreateFontA, CreateFontA_Hook);
	}
	//*********END Hook CreateFontA*********


	//*********Start Hook CreateFontW*******
	static DWORD sg_dwCharSetW = 0;
	static LPCWSTR sg_lpFontNameW = nullptr;
	static Fn_CreateFontW sg_fnCreateFontW = CreateFontW;
	HFONT WINAPI CreateFontW_Hook(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName)
	{
		iCharSet = sg_dwCharSetW;
		pszFaceName = sg_lpFontNameW;
		return sg_fnCreateFontW(cHeight, cWidth, cEscapement, cOrientation, cWeight, bItalic, bUnderline, bStrikeOut, iCharSet, iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily, pszFaceName);
	}

	bool HookCreateFontW(const uint32_t uiCharSet, const wchar_t* cpFontName)
	{
		sg_dwCharSetW = uiCharSet;
		sg_lpFontNameW = cpFontName;
		return DetourAttachFunc(&sg_fnCreateFontW, CreateFontW_Hook);
	}
	//*********END Hook CreateFontW*********


	//*********Start Hook CreateFontIndirectA*******
	static DWORD sg_dwCharSetIndirectA = 0;
	static LPCSTR sg_lpFontNameIndirectA = nullptr;
	static Fn_CreateFontIndirectA sg_fnCreateFontIndirectA = CreateFontIndirectA;
	HFONT WINAPI CreateFontIndirectA_Hook(LOGFONTA* lplf)
	{
		lplf->lfCharSet = (BYTE)sg_dwCharSetIndirectA;
		::lstrcpyA(lplf->lfFaceName, sg_lpFontNameIndirectA);
		return sg_fnCreateFontIndirectA(lplf);
	}

	bool HookCreateFontIndirectA(const uint32_t uiCharSet, const char* cpFontName)
	{
		sg_dwCharSetIndirectA = uiCharSet;
		sg_lpFontNameIndirectA = cpFontName;
		return DetourAttachFunc(&sg_fnCreateFontIndirectA, CreateFontIndirectA_Hook);
	}
	//*********END Hook CreateFontIndirectA*********


	//*********Start Hook CreateFontIndirectW*******
	static DWORD sg_dwCharSetIndirectW = 0;
	static LPCWSTR sg_lpFontNameIndirectW = nullptr;
	static Fn_CreateFontIndirectW sg_fnCreateFontIndirectW = CreateFontIndirectW;
	HFONT WINAPI CreateFontIndirectW_Hook(LOGFONTW* lplf)
	{
		lplf->lfCharSet = (BYTE)sg_dwCharSetIndirectW;
		::lstrcpyW(lplf->lfFaceName, sg_lpFontNameIndirectW);
		return sg_fnCreateFontIndirectW(lplf);
	}

	bool HookCreateFontIndirectW(const uint32_t uiCharSet, const wchar_t* cpFontName)
	{
		sg_dwCharSetIndirectW = uiCharSet;
		sg_lpFontNameIndirectW = cpFontName;
		return DetourAttachFunc(&sg_fnCreateFontIndirectW, CreateFontIndirectW_Hook);
	}
	//*********END Hook CreateFontIndirectW*********


	//*********Start Hook CreateWindowExA*******
	static LPCSTR sg_lpNewTitle = nullptr;
	static LPCSTR sg_lpRawTitle = nullptr;
	static Fn_CreateWindowExA sg_fnCreateWindowExA = CreateWindowExA;
	HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, INT X, INT Y, INT nWidth, INT nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
	{
		if (!::lstrcmpA(lpWindowName, sg_lpRawTitle))
		{
			lpWindowName = sg_lpNewTitle;
			DetourDetachFunc(&sg_fnCreateWindowExA, &CreateWindowExA_Hook);
		}

		return sg_fnCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}

	bool HookTitleExA(const char* cpRawTitle, const char* cpPatchTitle)
	{
		sg_lpNewTitle = cpPatchTitle;
		sg_lpRawTitle = cpRawTitle;
		return DetourAttachFunc(&sg_fnCreateWindowExA, CreateWindowExA_Hook);
	}
	//*********END Hook CreateWindowExA*********
}