#pragma once
#include <cstdint>


namespace Rut::RxHook
{
	bool HookCreateFontA(const uint32_t uiCharSet, const char* cpFontName);
	bool HookCreateFontIndirectA(const uint32_t uiCharSet, const char* cpFontName);
	bool HookTitleExA(const char* cpRawTitle, const char* cpPatchTitle);
}