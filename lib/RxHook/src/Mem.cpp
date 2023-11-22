#include "Mem.h"
#include "ErrorMsg.h"

#include <Windows.h>


namespace Rut::RxHook
{
	bool SysMemRead(void* pAddress, void* pBuffer, size_t nSize)
	{
		return SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr) ? memcpy(pBuffer, pAddress, nSize) : false;
	}

	bool SysMemWrite(void* pAddress, void* pBuffer, size_t nSize)
	{
		return SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr) ? memcpy(pAddress, pBuffer, nSize) : false;
	}

	bool SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize)
	{
		return SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE, nullptr) ? memset(pAddress, ucValue, nSize) : false;
	}

	bool SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess)
	{
		DWORD old = 0;
		return ::VirtualProtect(pAddress, nSize, uiAccess, &old) ? (bool)(pOldAccess ? (*pOldAccess = old) : (true)) : false;
	}

	void* SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess)
	{
		return ::VirtualAlloc(pAddress, nSize, uiType, uiAccess);
	}



	void SysMemRead(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit)
	{
		SysMemRead(pAddress, pBuffer, nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
	}

	void SysMemWrite(void* pAddress, void* pBuffer, size_t nSize, const wchar_t* wpErrorMsg, bool isExit)
	{
		SysMemWrite(pAddress, pBuffer, nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
	}

	void SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize, const wchar_t* wpErrorMsg, bool isExit)
	{
		SysMemFill(pAddress,ucValue,nSize) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
	}

	void SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess, uint32_t* pOldAccess, const wchar_t* wpErrorMsg, bool isExit)
	{
		SysMemAccess(pAddress,nSize,uiAccess,pOldAccess) ? (void)nullptr : SysErrorMsgBox(wpErrorMsg, isExit);
	}
	
	void* SysMemAlloc(void* pAddress, size_t nSize, uint32_t uiType, uint32_t uiAccess, const wchar_t* wpErrorMsg, bool isExit)
	{
		void* buffer_ptr = SysMemAlloc(pAddress, nSize, uiType, uiAccess);
		if (buffer_ptr == nullptr) { SysErrorMsgBox(wpErrorMsg, isExit); }
		return buffer_ptr;
	}


	void* MemSearch(const void* pStartAddress, size_t nMaxSearchSize, const void* pMatchData, const size_t nMatchDataBytes, const bool isBackward)
	{
		uint8_t* start_ptr = (uint8_t*)pStartAddress;
		if (start_ptr && nMatchDataBytes)
		{
			if (isBackward)
			{
				for (size_t ite = 0; ite < nMaxSearchSize; ite++)
				{
					if (!memcmp(pMatchData, start_ptr--, nMatchDataBytes)) 
					{ 
						return (start_ptr + 1);
					}
				}
			}
			else
			{
				for (size_t ite = 0; ite < nMaxSearchSize; ite++)
				{
					if (!memcmp(pMatchData, start_ptr++, nMatchDataBytes)) 
					{ 
						return (start_ptr - 1);
					}
				}
			}
		}

		return nullptr;
	}

}