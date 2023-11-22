#include "ErrorMsg.h"

#include <Windows.h>


namespace Rut::RxHook
{
	void SysErrorMsgBox(const wchar_t* wpMsg, bool isExit)
	{
		::MessageBoxW(NULL, wpMsg, NULL, NULL);
		isExit ? (::ExitProcess(-1)) : ((void)nullptr);
	}

	void SysErrorMsgBox(const char* cpMsg, bool isExit)
	{
		::MessageBoxA(NULL, cpMsg, NULL, NULL);
		isExit ? (::ExitProcess(-1)) : ((void)nullptr);
	}
}