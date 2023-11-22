#pragma once
#include <cstdint>


namespace Rut::RxHook
{
	void HookCreateFontA(const uint32_t uiCharSet, const char* cpFontName);
	void HookCreateFontW(const uint32_t uiCharSet, const wchar_t* cpFontName);
	void HookCreateFontIndirectA(const uint32_t uiCharSet, const char* cpFontName);
	void HookCreateFontIndirectW(const uint32_t uiCharSet, const wchar_t* cpFontName);
	void HookTitleExA(const char* cpRawTitle, const char* cpPatchTitle);
}