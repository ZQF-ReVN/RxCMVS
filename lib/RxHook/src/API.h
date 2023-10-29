#pragma once
#include <cstdint>


namespace Rut::RxHook
{
	bool HookCreateFontA(const uint32_t uiCharSet, const char* cpFontName);
	bool HookCreateFontW(const uint32_t uiCharSet, const wchar_t* cpFontName);
	bool HookCreateFontIndirectA(const uint32_t uiCharSet, const char* cpFontName);
	bool HookCreateFontIndirectW(const uint32_t uiCharSet, const wchar_t* cpFontName);
	bool HookTitleExA(const char* cpRawTitle, const char* cpPatchTitle);
}