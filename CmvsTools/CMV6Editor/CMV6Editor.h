#pragma once
#include <iostream>
#include <fstream>
#include <vector>


namespace CMV6File
{
	//CMV6File (Some As CMV4 CMV3)
	/*
	* CMV6FileHeader
	* CMV6IndexDescriptor[uiResCount + 1];
	* ResourcesData
	*/

	struct CMV6FileHeader
	{
		char aSignature[4]; //CMV6 CMV4 CMV3
		unsigned int uiResSecOffset;
		unsigned int uiCMVFileSize;
		unsigned int uiUnknow0; //Not Used
		unsigned int uiResMaxSequence; //Start With Zero
		unsigned int uiFrameRate;
		unsigned int uiFrameRateTime; // Per Second
		unsigned int uiImageWidth;
		unsigned int uiImageHight;
		unsigned int uiBitCount;
		unsigned int uiAudioFlag; // About Audio File Load If 0x0 Game Will Crash In MOG_INputMemory
	};

	struct CMV6IndexDescriptor
	{
		unsigned int uiSequence; // Max = uiResCount
		unsigned int uiCmpSize;
		unsigned int uiOrgSize; //BitMap Size ((uiBitCount / 8) * uiImageWidth * uiImageHight)
		unsigned int uiResType; // 0 = ogg, 02 = jbpx
		unsigned int uiOffset; // ResFileOffset = uiOffset + uiResOffset
	};

	enum ResType
	{
		OGG,
		WEBP, //Custom
		JBPX
	};

	class CMV6Editor
	{
	protected:
		char* m_pRes;
		size_t m_szAllocMax;
		std::fstream m_fsCMV;
		std::fstream m_fsRES;
		std::wstring m_wsCMV;
		std::wstring m_wsPath;

	public:
		size_t m_szData;
		CMV6FileHeader m_Header;

	public:
		CMV6Editor();
		~CMV6Editor();

		void HeapReSize(size_t szRes);
		static std::wstring MakeFileName(unsigned int uiSequence, unsigned int uiType);
		static unsigned int GetFileSize(std::ifstream& fsFile);
	};


	class CMV6Unpack : public CMV6Editor
	{
	public:
		std::vector<CMV6IndexDescriptor> m_vecDescriptor;

	private:
		void InitPackInfo();
		bool WriteRes(std::wstring& wsRes, size_t posRes, size_t szRes);

	public:
		CMV6Unpack(std::wstring wsCMV, std::wstring wsPath);

		bool UnPackAllRes();
		bool UnPackSingleRes(unsigned int uiSequence);
		char* GetResToBuffer(size_t posRes, size_t szRes);

	};

	class CMV6Pack : public CMV6Editor
	{
	private:
		struct AddResInfo
		{
			CMV6IndexDescriptor Descriptor;
			std::wstring wsResPath;
		};

	public:
		std::vector<AddResInfo> m_vecAddResInfo;

	public:
		CMV6Pack(std::wstring wsPath);

		void AddRes(CMV6IndexDescriptor& lpDescriptor, std::wstring& wsResPath);
		void MakeNewPack();
	};
}
