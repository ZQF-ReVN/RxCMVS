#pragma once
#include <iostream>
#include <Windows.h>

namespace CMVSTools
{
	namespace CMVSFileDump
	{
		BOOL DumpBMPFile(PBYTE lpBuffer, LPCSTR lpOutFileName, DWORD dwSizeDiB);
	}
}
