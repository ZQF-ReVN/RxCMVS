#pragma once
#include <cstdint>


namespace Rut::RxHook
{
	void SysMemRead(void* pAddress, void* pBuffer, size_t nSize);
	void SysMemWrite(void* pAddress, void* pBuffer, size_t nSize);
	void SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize);
	void SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess);

	uint32_t MemSearch(uint32_t pFind, size_t szFind, uint8_t* pToFind, size_t szToFind, bool backward = false);
}


