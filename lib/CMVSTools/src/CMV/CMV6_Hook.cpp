#include "CMV6_Hook.h"
#include "CMV6_Hook_Def.h"
#include "CMV6_PixelArray.h"
#include "CMV6_Coder.h"
#include "CMV_Editor.h"
#include "../../../RxHook/RxHook.h"
#include "../../../Rut/RxStream.h"



#include <Windows.h>
#include <iostream>

using namespace Rut;


namespace CMVS::CMV
{
	static wchar_t sg_aFilePathBuffer[0xFF] = { 0 };
	static uint8_t* sg_pJBPDBuffer = (uint8_t*)malloc(0x384000);
	static uint8_t* sg_pBitMapBuffer = (uint8_t*)malloc(0x384000);

	static fnTransFrame sg_fnTransFrame = (fnTransFrame)0x004311E0;
	static fnGetJBPXBuffer sg_fnGetJBPXBuffer = (fnGetJBPXBuffer)0x004333D0;
	static fnDecJBPDBuffer sg_fnDecJBPDBuffer = (fnDecJBPDBuffer)0x00518490;
	static fnDecJBP1Buffer sg_fnDecJBP1Buffer = (fnDecJBP1Buffer)0x00431401;


	//Make Full Path Of Dump File
	wchar_t* MakeFileName(uint32_t nSequence)
	{
		swprintf_s(sg_aFilePathBuffer, L"Dump\\%08d", nSequence);
		return sg_aFilePathBuffer;
	}


	bool LoadJBPX(const wchar_t* wpJBPXFilePath, uint8_t* pJBPXBuffer, uint32_t* szJBPXFile)
	{
		*szJBPXFile = 0;
		FILE* fpJBPX = nullptr;

		errno_t err = _wfopen_s(&fpJBPX, wpJBPXFilePath, L"rb");
		if ((fpJBPX == nullptr) || (err != 0)) { return false; }

		// Get Size
		fseek(fpJBPX, 0, SEEK_END);
		*szJBPXFile = ftell(fpJBPX);
		if (!(*szJBPXFile)) { return false; }
		fseek(fpJBPX, 0, SEEK_SET);

		// Write To Buffer
		fread(pJBPXBuffer, 1, *szJBPXFile, fpJBPX);

		fflush(fpJBPX);
		fclose(fpJBPX);

		return true;
	}

	bool JBPXDecodeToWebP(const wchar_t* lpJBPXFilePath, uint8_t* pJBPXBuffer, uint32_t szJBPXBuffer)
	{
		sg_fnDecJBPDBuffer(&sg_SecData, &sg_SecBites, (PBYTE)pJBPXBuffer, &szJBPXBuffer, 0);
		MergeImage720(&sg_SecData, sg_pBitMapBuffer);
		return SaveAsWebP((wchar_t*)lpJBPXFilePath, (PUCHAR)sg_pBitMapBuffer, sg_BitMapInfo.biSizeImage);
	}

	bool JBPXDecodeFromeFile(std::wstring wsJBPXPath)
	{
		static uint32_t jbpx_size = 0;

		bool is_load = LoadJBPX(wsJBPXPath.c_str(), sg_pJBPDBuffer, &jbpx_size);
		if (is_load == false) { std::wcout << L"Load File Failed!!\n" << std::endl; return false; }

		bool is_decode = JBPXDecodeToWebP(wsJBPXPath.c_str(), sg_pJBPDBuffer, jbpx_size);
		if (is_decode) { std::wcout << L"Decode File Failed!!\n" << std::endl; return false; }

		return true;
	}

	void ReDoDecodeJBPXFromeFile()
	{
		std::wstring jbpx_path;
		while (true)
		{
			std::wcout << L"Input JBPX File Name:" << std::endl;
			std::wcin >> jbpx_path;
			JBPXDecodeFromeFile(jbpx_path);
		}
	}

	void UnPackCMV()
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
				for (CMV6File::CMV_Index& descriptor : cmvFile.m_vecDescriptor)
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

	uint8_t* __fastcall GetJBPDBuffer_Hook(uint32_t* pTHIS, uint32_t dwEDX, uint32_t dwSequence, uint32_t* szJBPDBuffer)
	{
		MakeFileName(dwSequence);
		return sg_fnGetJBPXBuffer(pTHIS, dwEDX, dwSequence, szJBPDBuffer);
	}

	uint32_t __stdcall DecJBPD_Hook(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, uint8_t* pJBPDBuffer, uint32_t* szJBPDBuffer, uint32_t dwUnknow)
	{
		MergeImage720(lpImageSecData, sg_pBitMapBuffer);
		SaveAsBmp(sg_aFilePathBuffer, sg_pBitMapBuffer, sg_BitMapInfo.biSizeImage);
		return sg_fnDecJBPDBuffer(lpImageSecData, lpImageSecWidthBites, pJBPDBuffer, szJBPDBuffer, dwUnknow);
	}


	//*************************
	//*      EXPORT FUNC      *
	//*************************
	void CMV6FrameDump()
	{
		RxStream::SetConsole(L"CMV6FrameDump");

		InitBMPInfo(&sg_BitMapFile, &sg_BitMapInfo);

		RxHook::DetourAttachFunc(&sg_fnDecJBPDBuffer, DecJBPD_Hook);
		RxHook::DetourAttachFunc(&sg_fnGetJBPXBuffer, GetJBPDBuffer_Hook);
	}

	void CMV6DecodeWebP()
	{
		//TDA::ConsoleX::SetConsole(L"CMV6DecodeWebP");

		RxHook::DetourAttachFunc(&sg_fnDecJBPDBuffer, DecWebP);
	}

	//Set Console Input .CMV File Path Unpack All Resources And Decode JBPX
	void UnPackCMVThread()
	{
		RxStream::SetConsole(L"UnPackCMV / Decode JBPX Thread");

		InitDecodeInfo();
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UnPackCMV, NULL, NULL, NULL);
	}
}
