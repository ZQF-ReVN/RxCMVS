#pragma once
#include <iostream>
#include <fstream>


//-MGVFile
//--MGVHeader
//--IndexCount * 4
//--Audio
//--Video

struct MGVHeader
{
	char aSignature[4];
	unsigned int uiBitCount;
	unsigned int uiVideoSize;
	unsigned int uiAudioSize;
	unsigned int uiFrameIndexCount;
	unsigned short usWidth;
	unsigned short usHight;
	unsigned int uiFrameRateTime; //1s = 1000ms
	unsigned int uiFrameRate; // FrameRate * 1000
};

namespace MGVFile
{
	class MGVEditor
	{
	public:
		MGVHeader m_Header;

	protected:
		char* m_pRes;
		size_t m_szAllocMax;
		std::fstream m_fsMGV;
		std::fstream m_fsRes;
		std::wstring m_wsMGV;

	protected:
		MGVEditor();
		~MGVEditor();
		void HeapReSize(size_t szRes);
		static unsigned int GetFileSize(std::fstream& fsFile);

	private:

	};
	

	class MGVUnpack : public MGVEditor
	{
	public:
		MGVUnpack(std::wstring wsMGV);
		void Unpack();

	private:
		void InitMGVInfo();

	};


	class MGVPack : public MGVEditor
	{
	public:
		MGVPack(std::wstring wsMGV);

		void InitMGVInfo(MGVHeader* lpHeader);
		bool MakePack(std::wstring wsAudioPath, std::wstring wsVideoPath);

	private:

	};

}