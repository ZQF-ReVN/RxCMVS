#pragma once
#include <cstdint>


namespace Rut::RxHook
{
	bool SysMemRead(void* pAddress, void* pBuffer, size_t nSize);
	bool SysMemWrite(void* pAddress, void* pBuffer, size_t nSize);
	bool SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize);
	bool SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess);
	void* SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess);

	void SysMemRead(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit);
	void SysMemWrite(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit);
	void SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize, const wchar_t* wpErrorMsg, bool isExit);
	void SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess, const wchar_t* wpErrorMsg, bool isExit);
	void* SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess, const wchar_t* wpErrorMsg, bool isExit);

	void* MemSearch(const void* pStartAddress, size_t nMaxSearchSize, const void* pMatchData, const size_t nMatchDataBytes, const bool isBackward = false);
}


