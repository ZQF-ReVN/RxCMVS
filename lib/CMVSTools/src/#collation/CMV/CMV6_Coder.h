#pragma once
#include <iostream>
#include <Windows.h>
#include <libwebp/include/encode.h>
#include <libwebp/include/decode.h>

#include "CMV6_Hook_Def.h"


namespace CMVS::CMV
{
	static BITMAPFILEHEADER sg_BitMapFile = { 0 };
	static BITMAPINFOHEADER sg_BitMapInfo = { 0 };

	static ImageSecData sg_SecData = { 0 };
	static ImageSecWidthBites sg_SecBites = { 0 };

	static bool sg_isLossless = true;
	static float sg_dwWebPQuality = 75.0F;


	//Fast Use To Dynamic Dump Frame
	bool SaveAsBmp(wchar_t* wpFilePath, uint8_t* pBitMapBuffer, uint32_t szFile)
	{
		lstrcatW(wpFilePath, L".bmp");
		const HANDLE hfile = CreateFileW(wpFilePath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		WriteFile(hfile, &sg_BitMapFile, sizeof(BITMAPFILEHEADER), nullptr, nullptr);
		WriteFile(hfile, &sg_BitMapInfo, sizeof(BITMAPINFOHEADER), nullptr, nullptr);
		WriteFile(hfile, pBitMapBuffer, szFile, nullptr, nullptr);
		FlushFileBuffers(hfile);
		CloseHandle(hfile);

		return true;
	}

	//Lossless WebP Format But Slow Encoding Speed. Use To Decode JBPX By Calling DecJBPX
	bool SaveAsWebP(wchar_t* wpFilePath, uint8_t* pBitMapBuffer, uint32_t szFile)
	{
		lstrcatW(wpFilePath, L".webp");
		const HANDLE hfile = CreateFileW(wpFilePath, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		//Destroy Alpha Channel
		for (size_t ite_byte = 3; ite_byte < szFile; ite_byte++)
		{
			pBitMapBuffer[ite_byte] = 0xFF;
			ite_byte += 3;
		}

		//Encode To WebP
		uint8_t* webp_ptr = nullptr;
		size_t webp_size = 0;
		if (sg_isLossless)
		{
			webp_size = WebPEncodeLosslessBGRA(pBitMapBuffer, 1280, 720, 1280 * 4, &webp_ptr);
		}
		else
		{
			webp_size = WebPEncodeBGRA(pBitMapBuffer, 1280, 720, 1280 * 4, sg_dwWebPQuality, &webp_ptr);
		}

		//Write File To Disk
		WriteFile(hfile, webp_ptr, webp_size, nullptr, nullptr);
		FlushFileBuffers(hfile);
		CloseHandle(hfile);
		WebPFree(webp_ptr);

		std::wcout << L"Dump:" << wpFilePath << std::endl;

		return true;
	}

	static int sg_iWidth = 1280;
	static int sg_iHight = 720;
	uint32_t __stdcall DecWebP(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, uint8_t* pWebPBuffer, uint32_t* szWebPBuffer, uint32_t dwUnknow)
	{
		//If WebP Encode CMV
		if (*(uint32_t*)pWebPBuffer == 0x46464952)
		{
			uint8_t* pBitMap = WebPDecodeBGRA((uint8_t*)pWebPBuffer, *szWebPBuffer, &sg_iWidth, &sg_iHight);
			if (pBitMap)
			{
				SplitImage720(lpImageSecData, pBitMap);
				WebPFree(pBitMap);
			}

			return 0;
		}
		else
		{
			return sg_fnDecJBPDBuffer(lpImageSecData, lpImageSecWidthBites, pWebPBuffer, szWebPBuffer, dwUnknow);
		}
	}

	void InitDecodeInfo()
	{
		InitJBPXInfo(&sg_SecData, &sg_SecBites);
		InitBMPInfo(&sg_BitMapFile, &sg_BitMapInfo);
	}

	//Set BMP Format File Info
	void InitBMPInfo(BITMAPFILEHEADER* lpBitMapFile, BITMAPINFOHEADER* lpBitMapInfo)
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
	void InitJBPXInfo(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites)
	{
		for (size_t iteSec = 0; iteSec < 0xF; iteSec++)
		{
			((uint8_t**)lpImageSecData)[iteSec] = (uint8_t*)malloc(0x40000);
			((uint32_t*)lpImageSecWidthBites)[iteSec] = 0x400;
		}
	}
}