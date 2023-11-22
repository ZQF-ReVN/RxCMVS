#pragma once


namespace Rut::RxHook
{
	void SysErrorMsgBox(const char* cpMsg, bool isExit);
	void SysErrorMsgBox(const wchar_t* wpMsg, bool isExit);
}