#include "MGVEditor.h"


namespace CMVSTools
{
	namespace MGVFile
	{
		MGVEditor::MGVEditor() :
			m_Header({ 0 }),
			m_pRes(nullptr),
			m_szAllocMax(0)
		{

		}

		MGVEditor::~MGVEditor()
		{
			if (m_pRes)
			{
				delete[] m_pRes;
			}

			if (m_fsMGV.is_open())
			{
				m_fsMGV.close();
			}

			if (m_fsRes.is_open())
			{
				m_fsRes.close();
			}
		}

		inline void MGVEditor::HeapReSize(size_t szRes)
		{
			if (!m_szAllocMax)
			{
				m_pRes = new char[szRes];
				m_szAllocMax = szRes;
			}

			if (szRes > m_szAllocMax)
			{
				delete[] m_pRes;
				m_pRes = new char[szRes];
				m_szAllocMax = szRes;
			}
		}

		unsigned int MGVEditor::GetFileSize(std::fstream& fsFile)
		{
			std::streamoff szFile = 0;
			std::streamoff oldOff = fsFile.tellg();

			fsFile.seekg(0, std::ios::end);
			szFile = fsFile.tellg();
			fsFile.seekg(oldOff, std::ios::beg);

			return (unsigned int)szFile;
		}


		MGVUnpack::MGVUnpack(std::wstring wsMGV)
		{
			m_wsMGV = wsMGV;

			InitMGVInfo();
		}

		inline void MGVUnpack::InitMGVInfo()
		{
			m_fsMGV.open(m_wsMGV, std::ios::in | std::ios::binary);
			if (m_fsMGV.is_open())
			{
				m_fsMGV.read((char*)&m_Header, sizeof(MGVHeader));
			}
		}

		void MGVUnpack::Unpack()
		{
			size_t szIndex = m_Header.uiFrameIndexCount * 4;

			//Unpack Audio
			m_fsRes.open(m_wsMGV + L".ogg", std::ios::out | std::ios::binary);
			if (!m_fsRes.is_open()) { return; }

			m_fsMGV.seekg(sizeof(MGVHeader));
			m_fsMGV.seekg(szIndex, std::ios::cur);

			HeapReSize(m_Header.uiAudioSize);

			m_fsMGV.read(m_pRes, m_Header.uiAudioSize);
			m_fsRes.write(m_pRes, m_Header.uiAudioSize);

			m_fsRes.flush();
			m_fsRes.close();

			//Unpack Video
			m_fsRes.open(m_wsMGV + L".ogv", std::ios::out | std::ios::binary);
			if (!m_fsRes.is_open()) { return; }

			m_fsMGV.seekg(sizeof(MGVHeader));
			m_fsMGV.seekg(szIndex, std::ios::cur);
			m_fsMGV.seekg(m_Header.uiAudioSize, std::ios::cur);

			HeapReSize(m_Header.uiVideoSize);

			m_fsMGV.read(m_pRes, m_Header.uiVideoSize);
			m_fsRes.write(m_pRes, m_Header.uiVideoSize);

			m_fsRes.flush();
			m_fsRes.close();
		}


		MGVPack::MGVPack(std::wstring wsMGV)
		{
			m_wsMGV = wsMGV;

			m_fsMGV.open(m_wsMGV, std::ios::out | std::ios::binary);
		}

		void MGVPack::InitMGVInfo(MGVHeader* lpHeader)
		{
			memcpy(&m_Header, lpHeader, sizeof(MGVHeader));
		}

		bool MGVPack::MakePack(std::wstring wsAudioPath, std::wstring wsVideoPath)
		{
			//Write Header
			if (!m_fsMGV.is_open()) { return false; }

			m_fsMGV.write((char*)&m_Header, sizeof(MGVHeader));
			m_fsMGV.flush();

			//Write Index
			HeapReSize(m_Header.uiFrameIndexCount * 4);
			uint16_t* pIndex = (uint16_t*)m_pRes;
			for (uint16_t iteIndex = 0; iteIndex < m_Header.uiFrameIndexCount; iteIndex++)
			{
				pIndex[0] = 0;
				pIndex[1] = iteIndex;

				pIndex += 2;
			}
			pIndex = nullptr;
			m_fsMGV.write(m_pRes, static_cast<std::streamsize>(m_Header.uiFrameIndexCount) * 4);

			//Write Audio
			m_fsRes.open(wsAudioPath, std::ios::in | std::ios::binary);
			if (!m_fsRes.is_open()) { return false; }

			HeapReSize(m_Header.uiAudioSize);

			m_fsRes.read(m_pRes, m_Header.uiAudioSize);
			m_fsMGV.write(m_pRes, m_Header.uiAudioSize);
			m_fsMGV.flush();

			m_fsRes.close();

			//Write Video
			m_fsRes.open(wsVideoPath, std::ios::in | std::ios::binary);
			if (!m_fsRes.is_open()) { return false; }

			HeapReSize(m_Header.uiVideoSize);

			m_fsRes.read(m_pRes, m_Header.uiVideoSize);
			m_fsMGV.write(m_pRes, m_Header.uiVideoSize);
			m_fsMGV.flush();

			m_fsRes.close();

			return true;
		}
	}
}

