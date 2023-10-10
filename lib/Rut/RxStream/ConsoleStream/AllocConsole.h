#pragma once
#include <corecrt_wstdio.h>


#ifdef WIN32

namespace Rut::RxStream
{
	FILE* SetConsole(const wchar_t* lpTitle);
}

#endif // WIN32

