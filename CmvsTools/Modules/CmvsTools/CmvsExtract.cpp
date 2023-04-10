#include <Windows.h>
#include <iostream>
#include <vector>

#include "CmvsExtract.h"
#include "../../ThirdParty/TDA/ConsoleX.h"
#include "../../ThirdParty/TDA/DetoursX.h"


namespace CmvsTools
{
	namespace CmvsExtract
	{
		static std::string g_msFileName;
		static std::vector<PDWORD> g_vecThisPointer;
		static std::vector<std::string> g_vecPackPath;
		static PDWORD g_pVTable = 0;
		static LPCSTR g_lpString = 0;
		static BOOL g_isInit = FALSE;

		typedef HLOCAL(__thiscall* pReadFile)(PDWORD pThis, LPCSTR lpPackPath, LPCSTR lpString, LPCSTR lpFileName, PDWORD lpSize);
		static pReadFile RawReadFile = NULL;

		typedef DWORD(__thiscall* pGetPackInfo)(PDWORD pTHIS, DWORD dwUnKnow0, DWORD dwUnKnow1);
		static pGetPackInfo RawGetPackInfo = (pGetPackInfo)0x004100F0;


		BOOL ExtractPack()
		{
			DWORD size = 0;
			size_t number = 0;
			size_t i = 0;

			if (g_vecPackPath.size() == g_vecThisPointer.size())
			{
				for (; i < g_vecPackPath.size(); i++)
				{
					std::cout << "M ->" << i << "<- " << "PackPath:" << g_vecPackPath[i] << std::endl;
				}
			}

			while (true)
			{
				std::cout << "PackNumber:";
				std::cin >> number;
				std::cout << std::endl;
				std::cout << "FileName::";
				std::cin >> g_msFileName;
				std::cout << std::endl;

				if ((number <= i) && (!g_msFileName.empty()))
				{
					PDWORD hLocal = (PDWORD)RawReadFile(g_vecThisPointer[number], g_vecPackPath[number].c_str(), g_lpString, g_msFileName.c_str(), &size);
					if (hLocal != NULL)
					{
						HANDLE hFile = CreateFileA(g_msFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if (hFile != INVALID_HANDLE_VALUE)
						{
							WriteFile(hFile, hLocal, size, NULL, NULL);
							FlushFileBuffers(hFile);
							CloseHandle(hFile);
							std::cout << "Dump:" << g_msFileName << std::endl;
						}
						else
						{
							std::cout << "CreateFile failed!!!" << std::endl;
						}
						LocalFree(hLocal);
					}
					else
					{
						std::cout << "ReadFile failed!!!" << std::endl;
					}
				}
			}
		}

		HLOCAL __fastcall NewReadFile(PDWORD pThis, DWORD dwReserved, LPCSTR lpPackPath, LPCSTR lpString, LPCSTR lpFileName, PDWORD pSize)
		{
			if (!g_isInit)
			{
				g_lpString = lpString;
				if (g_lpString != NULL)
				{
					g_isInit = TRUE;
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ExtractPack, NULL, NULL, NULL);
				}
			}

			return RawReadFile(pThis, lpPackPath, lpString, lpFileName, pSize);
		}

		DWORD __fastcall NewGetPackInfo(PDWORD pTHIS, DWORD dwReserved, DWORD dwUnKnow0, DWORD dwUnKnow1)
		{
			if (!g_pVTable)
			{
				g_pVTable = (PDWORD)*pTHIS;
				RawReadFile = (pReadFile)g_pVTable[3];

				TDA::DetoursX::DetourAttachFunc(&RawReadFile, NewReadFile);
			}

			if (!g_isInit)
			{
				std::string path = (LPCSTR)(&(&dwUnKnow1)[0x2])[0];
				if (path.find(".cpz") != std::string::npos)
				{
					g_vecPackPath.emplace_back(path);
					g_vecThisPointer.emplace_back(pTHIS);
				}
			}

			return RawGetPackInfo(pTHIS, dwUnKnow0, dwUnKnow1);
		}

		void SetCmvsExtract()
		{
			TDA::ConsoleX::SetConsole(L"CmvsExtract");

			TDA::DetoursX::DetourAttachFunc(&RawGetPackInfo, NewGetPackInfo);
		}
	}
}
