#pragma once
#include <corecrt_wstdio.h>


namespace Rut::RxConsole
{
	FILE* Alloc(const wchar_t* lpTitle, bool isEdit = false, bool isTop = true);
}


