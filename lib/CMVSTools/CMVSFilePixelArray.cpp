#include "CmvsFilePixelArray.h"

namespace CMVSTools
{
	namespace CMVSFileDump
	{
		struct BITMAPDEC
		{
			PBYTE lpBMPBuffer;
			PBYTE lpBits;
			LONG szBytesPerLine;
			DWORD szDiB;
			DWORD szPixel;
		};

		LPCSTR g_lpFileNmae = 0;
		BITMAPDEC g_bDec = { 0 };
		BITMAPFILEHEADER g_bFile = { 0 };
		BITMAPINFOHEADER g_bInfo = { 0 };

		VOID WriteBMPFile(BITMAPFILEHEADER* lpFile, BITMAPINFOHEADER* lpInfo, BITMAPDEC* lpDec)
		{
			HANDLE hFile = 0;

			hFile = CreateFileA(g_lpFileNmae, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				WriteFile(hFile, lpFile, sizeof(BITMAPFILEHEADER), NULL, NULL);
				WriteFile(hFile, lpInfo, sizeof(BITMAPINFOHEADER), NULL, NULL);
				WriteFile(hFile, lpDec->lpBits, lpDec->szDiB, NULL, NULL);
				FlushFileBuffers(hFile);
				CloseHandle(hFile);
			}
		}

		VOID InitBMPStruct()
		{
			//Init FileHeader
			memcpy(&g_bFile, g_bDec.lpBMPBuffer, sizeof(g_bFile));

			//Init InfoHeader
			memcpy(&g_bInfo, &g_bDec.lpBMPBuffer[sizeof(g_bFile)], sizeof(g_bInfo));

			//Set Poniter To Bits Data
			g_bDec.lpBits = &g_bDec.lpBMPBuffer[sizeof(g_bFile) + sizeof(g_bInfo)];

			//Calculate Perline Size
			g_bDec.szBytesPerLine = ((g_bInfo.biWidth * g_bInfo.biBitCount + 31) / 32) * 4;

			//Calculate The Size Of DiB
			g_bDec.szDiB = g_bDec.szBytesPerLine * g_bInfo.biHeight;

			//Calculate One Pixel Size
			g_bDec.szPixel = g_bDec.szBytesPerLine / g_bInfo.biWidth;

		}

		BOOL ReadPixel(BITMAPINFOHEADER* lpInfo, BITMAPDEC* lpMapDec, RGBQUAD* lpPixel, LONG x, LONG y)
		{
			if (x < lpInfo->biWidth && y < lpInfo->biHeight)
			{
				UINT px = x * lpMapDec->szPixel;
				UINT py = y * lpMapDec->szBytesPerLine;
				memcpy(lpPixel, &lpMapDec->lpBits[py + px], lpMapDec->szPixel);

				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		BOOL WritePixel(BITMAPINFOHEADER* lpInfo, BITMAPDEC* lpMapDec, RGBQUAD* lpPixel, LONG x, LONG y)
		{
			if (x < lpInfo->biWidth && y < lpInfo->biHeight)
			{
				UINT px = x * lpMapDec->szPixel;
				UINT py = y * lpMapDec->szBytesPerLine;
				memcpy(&lpMapDec->lpBits[py + px], lpPixel, lpMapDec->szPixel);

				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		BOOL VerFlipBitMap()
		{
			RGBQUAD pixel = { 0 };
			BITMAPDEC bDec = g_bDec;

			bDec.lpBits = (PBYTE)VirtualAlloc(NULL, g_bDec.szDiB, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			if (bDec.lpBits != NULL)
			{
				for (LONG y = 0; y < g_bInfo.biHeight; y++)
				{
					for (LONG x = 0; x < g_bInfo.biWidth; x++)
					{
						BOOL isRead = ReadPixel(&g_bInfo, &g_bDec, &pixel, x, y);
						BOOL isWritten = WritePixel(&g_bInfo, &bDec, &pixel, x, g_bInfo.biHeight - y - 1);
						if (!(isRead && isWritten))
						{
							VirtualFree(bDec.lpBits, NULL, MEM_RELEASE);
							return FALSE;
						}
					}
				}

				WriteBMPFile(&g_bFile, &g_bInfo, &bDec);
				VirtualFree(bDec.lpBits, NULL, MEM_RELEASE);
				return TRUE;;
			}

			return FALSE;
		}

		bool DumpBMPFile(PBYTE lpBuffer, LPCSTR lpFileName, DWORD dwSizeDiB)
		{
			g_bFile = { 0 };
			g_bInfo = { 0 };
			g_bDec = { 0 };
			g_lpFileNmae = lpFileName;

			g_bDec.lpBMPBuffer = lpBuffer;
			if (g_bDec.lpBMPBuffer != NULL)
			{
				InitBMPStruct();

				if (dwSizeDiB < g_bDec.szDiB)
				{
					return false;
				}

				if (g_bInfo.biBitCount == 32)
				{
					VerFlipBitMap();
					return true;
				}

				return false;
			}

			return false;
		}
	}
}

