#include "JBPX.h"
#include "../TDA/ConsoleX.h"
#include "../TDA/DetoursX.h"
#include "../CMV6Editor/CMV6Editor.h"
#include "../ThirdParty/libwebp/encode.h"
#include "../ThirdParty/libwebp/decode.h"
#pragma comment(lib,"../ThirdParty/libwebp/libwebp.lib")


struct ImageSecData
{
	unsigned char* pSec0;
	unsigned char* pSec1;
	unsigned char* pSec2;
	unsigned char* pSec3;
	unsigned char* pSec4;
	unsigned char* pSec5;
	unsigned char* pSec6;
	unsigned char* pSec7;
	unsigned char* pSec8;
	unsigned char* pSec9;
	unsigned char* pSecA;
	unsigned char* pSecB;
	unsigned char* pSecC;
	unsigned char* pSecD;
	unsigned char* pSecE;
};

struct ImageSecWidthBites
{
	unsigned long szSec0;
	unsigned long szSec1;
	unsigned long szSec2;
	unsigned long szSec3;
	unsigned long szSec4;
	unsigned long szSec5;
	unsigned long szSec6;
	unsigned long szSec7;
	unsigned long szSec8;
	unsigned long szSec9;
	unsigned long szSecA;
	unsigned long szSecB;
	unsigned long szSecC;
	unsigned long szSecD;
	unsigned long szSecE;
};

BITMAPFILEHEADER g_bFile = { 0 };
BITMAPINFOHEADER g_bInfo = { 0 };

ImageSecData g_lpSecData = { 0 };
ImageSecWidthBites g_lpSecBites = { 0 };

BOOL g_isLossless = TRUE;
FLOAT g_dwWebPQuality = 75.0F;

WCHAR g_lpFilePathBuffer[0xFF] = { 0 };
PCHAR g_pBitMapBuffer = (PCHAR)malloc(0x384000);
PCHAR g_pJBPDBuffer = (PCHAR)malloc(0x384000);

typedef PBYTE(__fastcall* pGetJBPXBuffer)(PBYTE* pTHIS, DWORD dwEDX, DWORD dwSequence, PDWORD szJBPDBuffer);
pGetJBPXBuffer orgGetJBPXBuffer = (pGetJBPXBuffer)0x004333D0;

typedef DWORD(__thiscall* pTransFrame)(PDWORD pThis, DWORD a2, char* pJBPXBuffer, PDWORD szJBPXBuffer, DWORD a5);
pTransFrame orgTransFrame = (pTransFrame)0x004311E0;

typedef DWORD(__stdcall* pDecJBPDBuffer)(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, PBYTE pJBPDBuffer, PDWORD szJBPDBuffer, DWORD dwUnknow);
pDecJBPDBuffer orgDecJBPDBuffer = (pDecJBPDBuffer)0x00518490;

typedef DWORD(__stdcall* pDecJBP1Buffer)(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, PBYTE pJBP1Buffer, PDWORD szJBP1Buffer, DWORD dwUnknow);
pDecJBP1Buffer orgDecJBP1Buffer = (pDecJBP1Buffer)0x00431401;

//For 1280 x 720
//MergeImage Frome 15 Sectors
//PerLineBytes = 0x400
//ImageHight 1,2,3,4,5,6,7,8,9 = 0x100; A,B,C,D,E = 0xD0
VOID MergeImage720(ImageSecData* lpImageSecData, PCHAR pMergeBuffer)
{
	for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
	{
		memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSec0[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSec1[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSec2[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSec3[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSec4[0x400 * iteHight], 0x400);
		pMergeBuffer += 0x400 * 5;
	}

	for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
	{
		memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSec5[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSec6[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSec7[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSec8[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSec9[0x400 * iteHight], 0x400);
		pMergeBuffer += 0x400 * 5;
	}

	for (size_t iteHight = 0; iteHight < 0xD0; iteHight++)
	{
		memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSecA[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSecB[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSecC[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSecD[0x400 * iteHight], 0x400);
		memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSecE[0x400 * iteHight], 0x400);
		pMergeBuffer += 0x400 * 5;
	}
}

//For 1280 x 720
//SplitImage Frome 15 Sectors
//PerLineBytes = 0x400
//ImageHight 1,2,3,4,5,6,7,8,9 = 0x100; A,B,C,D,E = 0xD0
VOID SplitImage720(ImageSecData* lpImageSecData, PCHAR pSplitBuffer)
{
	for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
	{
		memcpy(&lpImageSecData->pSec0[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
		memcpy(&lpImageSecData->pSec0[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
		memcpy(&lpImageSecData->pSec1[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
		memcpy(&lpImageSecData->pSec2[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
		memcpy(&lpImageSecData->pSec3[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
		memcpy(&lpImageSecData->pSec4[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
		pSplitBuffer += 0x400 * 5;
	}

	for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
	{
		memcpy(&lpImageSecData->pSec5[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
		memcpy(&lpImageSecData->pSec6[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
		memcpy(&lpImageSecData->pSec7[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
		memcpy(&lpImageSecData->pSec8[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
		memcpy(&lpImageSecData->pSec9[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
		pSplitBuffer += 0x400 * 5;
	}

	for (size_t iteHight = 0; iteHight < 0xD0; iteHight++)
	{
		memcpy(&lpImageSecData->pSecA[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
		memcpy(&lpImageSecData->pSecB[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
		memcpy(&lpImageSecData->pSecC[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
		memcpy(&lpImageSecData->pSecD[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
		memcpy(&lpImageSecData->pSecE[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
		pSplitBuffer += 0x400 * 5;
	}
}

//Fast Use To Dynamic Dump Frame
BOOL SaveAsBmp(LPCWSTR lpFilePath, PCHAR pBitMapBuffer, DWORD szFile)
{
	lstrcatW((LPWSTR)lpFilePath, L".bmp");
	HANDLE hFile = CreateFileW(lpFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, &g_bFile, sizeof(BITMAPFILEHEADER), NULL, NULL);
		WriteFile(hFile, &g_bInfo, sizeof(BITMAPINFOHEADER), NULL, NULL);
		WriteFile(hFile, pBitMapBuffer, szFile, NULL, NULL);
		FlushFileBuffers(hFile);
		CloseHandle(hFile);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//Lossless WebP Format But Slow Encoding Speed. Use To Decode JBPX By Calling DecJBPX
BOOL SaveAsWebP(LPCWSTR lpFilePath, PUCHAR pBitMapBuffer, DWORD szFile)
{
	lstrcatW((LPWSTR)lpFilePath, L".webp");
	HANDLE hFile = CreateFileW(lpFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		//Destroy Alpha Channel
		for (size_t iteBitMap = 3; iteBitMap < szFile; iteBitMap++)
		{
			pBitMapBuffer[iteBitMap] = 0xFF;
			iteBitMap += 3;
		}

		//Encode To WebP
		uint8_t* pWebP = nullptr;
		size_t szWebP = 0;
		if (g_isLossless)
		{
			szWebP = WebPEncodeLosslessBGRA((uint8_t*)pBitMapBuffer, 1280, 720, 1280 * 4, &pWebP);
		}
		else
		{
			szWebP = WebPEncodeBGRA((uint8_t*)pBitMapBuffer, 1280, 720, 1280 * 4, g_dwWebPQuality, &pWebP);
		}

		//Write File To Disk
		WriteFile(hFile, pWebP, szWebP, NULL, NULL);
		FlushFileBuffers(hFile);
		CloseHandle(hFile);
		WebPFree(pWebP);

		std::wcout << L"Dump:" << lpFilePath << std::endl;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//Make Full Path Of Dump File
PWCHAR MakeFileName(DWORD dwSequence)
{
	swprintf_s(g_lpFilePathBuffer, L"Dump\\%08d", dwSequence);
	return g_lpFilePathBuffer;
}

//Set BMP Format File Info
VOID InitBMPInfo(BITMAPFILEHEADER* lpBitMapFile, BITMAPINFOHEADER* lpBitMapInfo)
{
	lpBitMapFile->bfType = 0x4D42;
	lpBitMapFile->bfSize = 0x384036;
	lpBitMapFile->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	lpBitMapInfo->biSize = 40;
	lpBitMapInfo->biWidth = 1280;
	lpBitMapInfo->biHeight = 720;
	lpBitMapInfo->biPlanes = 1;
	lpBitMapInfo->biBitCount = 32;
	lpBitMapInfo->biCompression = 0;
	lpBitMapInfo->biSizeImage = 0x384000;
}

//Set JBPX Fromat Processing Conditions
VOID InitJBPXInfo(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites)
{
	for (size_t iteSec = 0; iteSec < 0xF; iteSec++)
	{
		((PBYTE*)lpImageSecData)[iteSec] = (PBYTE)malloc(0x40000);
		((DWORD*)lpImageSecWidthBites)[iteSec] = 0x400;
	}
}

VOID InitDecodeInfo()
{
	InitJBPXInfo(&g_lpSecData, &g_lpSecBites);
	InitBMPInfo(&g_bFile, &g_bInfo);
}

BOOL LoadJBPX(LPCWSTR lpJBPXFilePath, PCHAR pJBPXBuffer, PDWORD szJBPXFile)
{
	*szJBPXFile = 0;
	errno_t err = 0;
	FILE* fpJBPX = nullptr;

	err = _wfopen_s(&fpJBPX, lpJBPXFilePath, L"rb");
	if (fpJBPX && !err)
	{
		fseek(fpJBPX, 0, SEEK_END);
		*szJBPXFile = ftell(fpJBPX);
		fseek(fpJBPX, 0, SEEK_SET);
		if (!(*szJBPXFile))
		{
			return FALSE;
		}

		fread(pJBPXBuffer, 1, *szJBPXFile, fpJBPX);

		fflush(fpJBPX);
		fclose(fpJBPX);

		return TRUE;
	}

	return FALSE;
}

BOOL JBPXDecodeToWebP(LPCWSTR lpJBPXFilePath, PCHAR pJBPXBuffer, DWORD szJBPXBuffer)
{
	orgDecJBPDBuffer(&g_lpSecData, &g_lpSecBites, (PBYTE)pJBPXBuffer, &szJBPXBuffer, 0);
	MergeImage720(&g_lpSecData, g_pBitMapBuffer);
	return SaveAsWebP(lpJBPXFilePath, (PUCHAR)g_pBitMapBuffer, g_bInfo.biSizeImage);
}

BOOL JBPXDecodeFromeFile(std::wstring strJBPXFilePath)
{
	BOOL isLoad = FALSE;
	BOOL isDecode = FALSE;
	static DWORD szJBPX = 0;

	isLoad = LoadJBPX(strJBPXFilePath.c_str(), g_pJBPDBuffer, &szJBPX);
	if (isLoad)
	{
		isDecode = JBPXDecodeToWebP(strJBPXFilePath.c_str(), g_pJBPDBuffer, szJBPX);
		if (isDecode)
		{
			return TRUE;
		}
		else
		{
			std::wcout << L"Decode File Failed!!\n" << std::endl;
			return FALSE;
		}
	}
	else
	{
		std::wcout << L"Load File Failed!!\n" << std::endl;
		return FALSE;
	}
}

VOID ReDoDecodeJBPXFromeFile()
{
	std::wstring jbpxFilePath;
	while (true)
	{
		std::wcout << L"Input JBPX File Name:" << std::endl;
		std::wcin >> jbpxFilePath;
		JBPXDecodeFromeFile(jbpxFilePath);
	}
}

VOID UnPackCMV()
{
	BOOL isSingle = FALSE;
	BOOL isDecode = FALSE;
	PCHAR pJBPXBuffer = NULL;
	PWCHAR resFilePath = NULL;
	std::wstring cmvFileName;

	CreateDirectoryW(L"Unpack", NULL);
	CreateDirectoryW(L"Dump", NULL);

	while (true)
	{
		std::wcout << L"** UnPackCMV / Decode JBPX Thread **\n" << std::endl;

		//Single JBPX Or CMV File
		std::wcout
			<< L"Code 1 : Decode Single JBPX \n"
			<< L"Code 0 : Process CMV File \n"
			<< L"Input Code:";
		std::wcin >> isSingle;
		if (isSingle)
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReDoDecodeJBPXFromeFile, NULL, NULL, NULL);
			return;
		}
		std::wcout << std::endl;

		//Create CMV File OBJ
		std::wcout << "Input File Name:";
		std::wcin >> cmvFileName;
		CMV6File::CMV6Unpack cmvFile(cmvFileName, L"Unpack\\");
		std::wcout << std::endl;

		//Is Decode JBPX
		std::wcout
			<< "Code 1 : Decode JBPX \n"
			<< "Code 0 : Not Decode Will Unpack All Resources(.JBPX / .Ogg) \n"
			<< L"Input Code:";
		std::wcin >> isDecode;
		if (isDecode)
		{
			//Save Image Lossless or Lossy
			std::wcout 
				<< L"Code 1 : Lossless\n"
				<< L"Code 0 : Lossy\n"
				<< L"Input Code:";
			std::wcin >> g_isLossless;
			if (!g_isLossless)
			{
				std::wcout 
					<< L"Set Quality Of WebP Lossy\n"
					<< L"Input Quality:";
				std::wcin >> g_dwWebPQuality;
			}

			std::wcout 
				<< L'\n'
				<< L"*********************************************\n"
				<< L"* Dump Start Will Save File In Dump Folder. *\n"
				<< L"*********************************************\n";

			//ReDo Decode JBPX Frome CMV File
			for (CMV6File::CMV6IndexDescriptor& descriptor : cmvFile.m_vecDescriptor)
			{
				//If Audio File
				if (descriptor.uiResType == 0)
				{
					cmvFile.UnPackSingleRes(descriptor.uiSequence);
					continue;
				}

				resFilePath = MakeFileName(descriptor.uiSequence);
				pJBPXBuffer = cmvFile.GetResToBuffer(descriptor.uiOffset + cmvFile.m_Header.uiResSecOffset, descriptor.uiCmpSize);
				JBPXDecodeToWebP(resFilePath, pJBPXBuffer, descriptor.uiCmpSize);
			}
		}
		else
		{
			std::wcout 
				<< L'\n'
				<< L"*************************************************\n"
				<< L"* Unpack Start Will Save File In Unpack Folder. *\n"
				<< L"*************************************************\n\n";

			cmvFile.UnPackAllRes();
		}

		std::wcout << std::endl;
	}
}

PBYTE __fastcall newGetJBPDBuffer(PBYTE* pTHIS, DWORD dwEDX, DWORD dwSequence, PDWORD szJBPDBuffer)
{
	MakeFileName(dwSequence);
	return orgGetJBPXBuffer(pTHIS, dwEDX, dwSequence, szJBPDBuffer);
}

DWORD __stdcall newDecJBPD(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, PBYTE pJBPDBuffer, PDWORD szJBPDBuffer, DWORD dwUnknow)
{
	MergeImage720(lpImageSecData, g_pBitMapBuffer);
	SaveAsBmp(g_lpFilePathBuffer, g_pBitMapBuffer, g_bInfo.biSizeImage);
	return orgDecJBPDBuffer(lpImageSecData, lpImageSecWidthBites, pJBPDBuffer, szJBPDBuffer, dwUnknow);
}

INT g_iWidth = 1280;
INT g_iHight = 720;
DWORD __stdcall DecWebP(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, PBYTE pWebPBuffer, PDWORD szWebPBuffer, DWORD dwUnknow)
{
	//If WebP Encode CMV
	if (*(PDWORD)pWebPBuffer == 0x46464952)
	{
		PCHAR pBitMap = (PCHAR)WebPDecodeBGRA((uint8_t*)pWebPBuffer, *szWebPBuffer, &g_iWidth, &g_iHight);
		if (pBitMap)
		{
			SplitImage720(lpImageSecData, pBitMap);
			WebPFree(pBitMap);
		}

		return 0;
	}
	else
	{
		return orgDecJBPDBuffer(lpImageSecData, lpImageSecWidthBites, pWebPBuffer, szWebPBuffer, dwUnknow);
	}
}

//*************************
//*      EXPORT FUNC      *
//*************************
VOID CMV6FrameDump()
{
	TDA::ConsoleX::SetConsole(L"CMV6FrameDump");

	InitBMPInfo(&g_bFile, &g_bInfo);

	TDA::DetoursX::DetourAttachFunc(&orgDecJBPDBuffer, newDecJBPD);
	TDA::DetoursX::DetourAttachFunc(&orgGetJBPXBuffer, newGetJBPDBuffer);
}

VOID CMV6DecodeWebP()
{
	//TDA::ConsoleX::SetConsole(L"CMV6DecodeWebP");

	TDA::DetoursX::DetourAttachFunc(&orgDecJBPDBuffer, DecWebP);
}

//Set Console Input .CMV File Path Unpack All Resources And Decode JBPX
VOID UnPackCMVThread()
{
	TDA::ConsoleX::SetConsole(L"UnPackCMV / Decode JBPX Thread");

	InitDecodeInfo();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UnPackCMV, NULL, NULL, NULL);
}