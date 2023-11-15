#include <cstdint>
#include <Windows.h>


namespace CMVS
{
	struct Pack_Coder;

	struct VFS_Entry
	{
		char aReadPath[2048];
		Pack_Coder* pCoder;
	};

	struct VFS_Comm_Index
	{
		VFS_Entry Entry[4];
	};

	struct VFS_Image_Index
	{
		uint8_t aUn0[4200];
		VFS_Entry aEntries[10];
		uint8_t aUn1[2052];
	};

	struct Rect_Size
	{
		uint32_t aUn[6];
		uint32_t iWidth;
		uint32_t iHeigh;
	};

	struct Engine
	{
		uint32_t* pVtable;
		HWND hWnd;
		uint32_t hInstance;
		uint32_t cpKeyConfigFilePath;
		uint32_t lpParam;
		uint32_t usIconID;
		uint32_t uiUn0_0;
		uint32_t uiBase_Val_0;
		uint32_t aUn1[50];
		WNDCLASSA WndClass;
		uint32_t uiBase_Val_1;
		uint32_t aUn2_1;
		uint32_t aUn2_2;
		uint32_t aUn2_3;
		char aTitle[128];
		char aWorkDir[1000];
		uint32_t aUn3[126];
		Rect_Size WndSize;
		uint32_t aBase_Val[343];
		VFS_Comm_Index* aVFSIndexPtr[8];
		uint32_t aUn4[1742];
		char aPath_0[176];
		uint32_t aMainFrame_Val_0[20];
		char aPath_1[128];
		uint32_t aMainFrame_Val_1[14];
		char aMainFrameWnd_WorkDir[1920];
		uint32_t aUn5[32];
		uint32_t uiTimer;
		uint32_t aCommand_Buffer[64];
		char aScriptName[128];
		uint32_t uiScriptSize;
		uint32_t aUn6_0;
		uint32_t aUn6_1;
		uint32_t aUn6_2;
		uint32_t uiReg0;
		uint32_t iCodeExecutLen;
		uint32_t uiReg1;
		uint32_t aCommand_Val_3[15];
		uint32_t aUn7[444];
		uint32_t aCommand_Val_5;
		uint32_t pScript;
		uint32_t aCommand_Val_0[3];
		uint32_t pCodeSeg;
		uint32_t uiUn8_0;
		uint32_t uiUn8_1;
		uint32_t uiUn8_2;
		uint32_t pStrSeg;
		uint32_t uiUn8_3;
		uint32_t uiUn8_4;
		uint32_t uiUn8_5;
		uint32_t pUnSeg;
		uint32_t uiUn8_6;
		uint32_t uiUn8_7;
		uint32_t uiUn8_8;
		uint32_t aCommand_Val_1[5];
		uint32_t aStackBuffer[16384];
		uint32_t aUn9[64];
		uint32_t* pStackPtr;
		uint32_t nStackTop;
		uint32_t aCommand_Val_e0;
		uint32_t aCommand_Buffer_1[63];
		uint32_t aCommand_Val_e1;
		uint32_t aCommand_Val_2[14];
		uint32_t aUnA[255];
		uint32_t uiMainFrame_Val_0;
		uint32_t uiMainFrame_Val_1;
	};

	struct Image
	{
		uint8_t* pPixelArray;
		uint32_t nSize;
		uint32_t nWidth;
		uint32_t nHight;
		uint32_t nBits;
		uint32_t nType;
	};

	//-MGVFile
	// -MGVHeader
	// -IndexCount * 4
	// -Audio
	// -Video
	struct MGV_HDR
	{
		uint8_t  aSignature[4];
		uint32_t uiBitCount;
		uint32_t uiVideoSize;
		uint32_t uiAudioSize;
		uint32_t uiFrameIndexCount;
		uint16_t usWidth;
		uint16_t usHight;
		uint32_t uiFrameRateTime; //1s = 1000ms
		uint32_t uiFrameRate; // FrameRate * 1000
	};

	struct PS3_HDR
	{
		uint32_t uiSignature;
		uint32_t uiHeaderSize;
		uint32_t uiUn0;
		uint32_t uiDataDecodeKey;
		uint32_t uiTextCount;
		uint32_t uiCodeBlockSize;
		uint32_t uiUnBlockSize;
		uint32_t uiTextBlockSize;
		uint32_t uiUn2;
		uint32_t uiDataCompressSize;
		uint32_t uiDataDecompressSize;
		uint32_t uiUn3;
	};


	typedef BOOL(__thiscall* Fn_RunVM)(Engine* This);

	typedef HLOCAL(__stdcall* Fn_ReadFullData)(char* cpFileName, uint32_t* pSize_Ret);

	typedef HLOCAL(__thiscall* Fn_ScriptReader)(VFS_Entry* pEntry, char* cpScriptName, uint32_t* pSize_Ret);

	typedef BOOL(__thiscall* Fn_ImageReader)(uint32_t* This, Image* pCImage, char* cpPrefixPath, char* cpImageName, uint32_t pUn0);

	typedef BOOL(__thiscall* Fn_ImageDecoder)(uint32_t* This, Image* pCImage, HLOCAL pData, uint32_t nDataSize, char* cpImageName, uint32_t pUn0);

	typedef HLOCAL(__thiscall* Fn_PackReadFile)(Pack_Coder* pCoder, VFS_Entry* pEntry, const char* cpFileName, uint32_t* pFileSize_Ret, uint32_t uiUn0, uint32_t uiUn1);

	typedef BOOL(__thiscall* Fn_RegCommPath)(VFS_Comm_Index* This, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9);

	typedef BOOL(__thiscall* Fn_RegVideoPath)(VFS_Comm_Index* This, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9);

	typedef BOOL(__thiscall* Fn_RegImagePath)(VFS_Image_Index* This, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9);

	typedef BOOL(__thiscall* Fn_RegUnPath)(VFS_Comm_Index* This, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9);


	struct Pack_Coder_VTable
	{
		uint32_t* fnUn0;
		uint32_t* fnUn1;
		uint32_t* fnUn2;
		uint32_t* fnUn3;
		Fn_PackReadFile fnReadFile;
		uint32_t* fnUn4;
		uint32_t* fnUn5;
	};

	struct Pack_Coder
	{
		Pack_Coder_VTable* pVtable;
	};
}