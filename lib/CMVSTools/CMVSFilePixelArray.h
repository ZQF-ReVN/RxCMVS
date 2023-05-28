#pragma once
#include <iostream>
#include <Windows.h>

namespace CMVSTools
{
	namespace CMVSFileDump
	{
		bool DumpBMPFile(PBYTE lpBuffer, LPCSTR lpOutFileName, DWORD dwSizeDiB);
	}
}
