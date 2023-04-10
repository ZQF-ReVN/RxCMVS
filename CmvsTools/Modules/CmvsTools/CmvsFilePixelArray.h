#pragma once
#include <iostream>
#include <Windows.h>

namespace CmvsTools
{
	namespace CmvsFileDump
	{
		BOOL DumpBMPFile(PBYTE lpBuffer, LPCSTR lpOutFileName, DWORD dwSizeDiB);
	}
}
