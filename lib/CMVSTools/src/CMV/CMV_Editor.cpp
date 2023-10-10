#include "CMV_Editor.h"
#include "../../lib/Rut/RxPath.h"
#include "../../lib/Rut/RxStream.h"

#include <string>
#include <algorithm>

using namespace Rut;


namespace CMVS::CMV
{
	CMVEditor::CMVEditor(const std::wstring& wsCMV) : m_Header({}), m_wsCMV(wsCMV)
	{
		m_ifsCMV.Create(wsCMV, RIO::RIO_IN, RCO::RCO_OPEN);
		m_ifsCMV.Read(&m_Header, sizeof(m_Header));
		m_wsFolder = RxPath::NotSuffix(m_wsCMV) + L"\\";
		ReadIndex();
	}

	wchar_t* CMVEditor::MakeFileName(uint32_t uiSequence, uint32_t uiType)
	{
		static wchar_t name_tmp[20] = { 0 };

		switch (uiType)
		{
		case CMV_ResType::OGG: { swprintf_s(name_tmp, L"%08d.ogg", uiSequence); } break;
		case CMV_ResType::JBPX: { swprintf_s(name_tmp, L"%08d.jbpx", uiSequence); } break;
		case CMV_ResType::WEBP: { swprintf_s(name_tmp, L"%08d.webp", uiSequence); } break;
		}

		return name_tmp;
	}

	void CMVEditor::ReadIndex()
	{
		//Read Index
		size_t index_size = (m_Header.uiResMaxSequence + 1) * sizeof(CMV_Entry);
		CMV_Entry* index_ptr = m_AutoMem[index_size];
		m_ifsCMV.Read(index_ptr, index_size);

		//Read Entry
		for (size_t ite_entry = 0; ite_entry <= m_Header.uiResMaxSequence; ite_entry++)
		{
			m_vcIndex.push_back(index_ptr[ite_entry]);
		}
	}

	void CMVEditor::SaveFrame(CMV_Entry& ceEntry)
	{
		size_t size = ceEntry.uiCmpSize;
		size_t offset = m_Header.uiResSecOffset + ceEntry.uiOffset;
		wchar_t* name = MakeFileName(ceEntry.uiSequence, ceEntry.uiResType);

		m_ifsCMV.SetPos(offset);
		m_ifsCMV.Read(m_AutoMem[size], size);
		RxStream::SaveFileViaPath((m_wsFolder + name).c_str(), m_AutoMem, size);
	}

	void CMVEditor::ExtractAll()
	{
		std::for_each(m_vcIndex.begin(), m_vcIndex.end(), [this](auto& entry) { SaveFrame(entry); });
	}

	void CMVEditor::ExtractViaSeq(uint32_t uiSequence)
	{
		SaveFrame(m_vcIndex[uiSequence]);
	}

	//void CMVEditor::MakeNewPack()
	//{
	//	if (m_fsCMV.is_open())
	//	{
	//		//Set Header
	//		m_hdHeader.uiResMaxSequence = m_vecAddResInfo.size() - 1;
	//		m_hdHeader.uiResSecOffset = (m_hdHeader.uiResMaxSequence + 1) * 0x14 + sizeof(CMV_Header);
	//		m_hdHeader.uiCMVFileSize = m_hdHeader.uiResSecOffset + m_szData;

	//		//Write Header
	//		m_fsCMV.write((char*)&m_hdHeader, sizeof(CMV_Header));

	//		//Write Index
	//		size_t posResSecOffset = 0;
	//		for (AddResInfo& iteInfo : m_vecAddResInfo)
	//		{
	//			iteInfo.Descriptor.uiOffset = posResSecOffset;
	//			posResSecOffset += iteInfo.Descriptor.uiCmpSize;

	//			m_fsCMV.write((char*)&iteInfo.Descriptor, sizeof(CMV_Index));
	//		}

	//		//Write Resources
	//		size_t szRes = 0;
	//		for (AddResInfo& iteInfo : m_vecAddResInfo)
	//		{
	//			szRes = iteInfo.Descriptor.uiCmpSize;
	//			m_fsRES.open(iteInfo.wsResPath, std::ios::in | std::ios::binary);
	//			if (m_fsRES.is_open())
	//			{
	//				HeapReSize(szRes);

	//				if (m_pRes)
	//				{
	//					m_fsRES.read(m_pRes, iteInfo.Descriptor.uiCmpSize);
	//					m_fsCMV.write(m_pRes, iteInfo.Descriptor.uiCmpSize);
	//				}

	//				m_fsRES.close();
	//			}
	//		}

	//		m_fsCMV.flush();
	//	}
	//}
}

