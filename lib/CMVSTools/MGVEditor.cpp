#include "MGVEditor.h"
#include "../../lib/Rxx/Mem.h"
#include "../../lib//Rxx/File.h"


using namespace Rut::MemX;
using namespace Rut::FileX;


namespace CMVSTools
{
	namespace MGVFile
	{
		MGVEditor::MGVEditor(const std::wstring& wsMGV) : m_hdHeader({}), m_wsMGV(wsMGV)
		{
			m_ifsMGV = OpenFileBinaryStream(m_wsMGV);
			m_ifsMGV.read((char*)&m_hdHeader, sizeof(m_hdHeader));
		}

		void MGVEditor::Extract()
		{
			AutoMem mem;
			size_t index_size = m_hdHeader.uiFrameIndexCount * 4;

			// Restore File Pointer
			m_ifsMGV.seekg(sizeof(m_hdHeader), std::ios::beg);

			// Unpack Audio
			uint8_t* buf_ptr = mem[m_hdHeader.uiAudioSize];
			m_ifsMGV.seekg(index_size, std::ios::cur);
			m_ifsMGV.read((char*)buf_ptr, m_hdHeader.uiAudioSize);
			SaveFileViaPath((m_wsMGV + L".ogg").c_str(), buf_ptr, m_hdHeader.uiAudioSize);

			// Unpack Video
			buf_ptr = mem[m_hdHeader.uiVideoSize];
			m_ifsMGV.read((char*)buf_ptr, m_hdHeader.uiVideoSize);
			SaveFileViaPath((m_wsMGV + L".ogv").c_str(), buf_ptr, m_hdHeader.uiVideoSize);
		}

		void MGVEditor::Replace(const std::wstring& wsVideo)
		{
			AutoMem mem;
			std::ofstream ofs_mgv = CreateFileBinaryStream(m_wsMGV + L".new");

			m_hdHeader.uiVideoSize = (uint32_t)GetFileSize(wsVideo);

			// Write Header
			ofs_mgv.write((char*)&m_hdHeader, sizeof(m_hdHeader));

			//Write Index
			size_t index_size = m_hdHeader.uiFrameIndexCount * 4;
			uint8_t* index_ptr = mem[index_size];
			m_ifsMGV.read((char*)index_ptr, index_size);
			ofs_mgv.write((char*)index_ptr, index_size);

			//Write Audio
			size_t audio_size = m_hdHeader.uiAudioSize;
			uint8_t* audio_ptr = mem[audio_size];
			m_ifsMGV.read((char*)audio_ptr, audio_size);
			ofs_mgv.write((char*)audio_ptr, audio_size);

			//Write Video
			std::ifstream ifs_vidoe = OpenFileBinaryStream(wsVideo);
			size_t video_size = m_hdHeader.uiVideoSize;
			uint8_t* video_ptr = mem[video_size];
			ifs_vidoe.read((char*)video_ptr, video_size);
			ofs_mgv.write((char*)video_ptr, video_size);
		}

		void MGVEditor::Create(const std::wstring& wsVideo, const std::wstring& wsAudio)
		{
			//uint8_t* p_index = mem[]
			//for (uint16_t iteIndex = 0; iteIndex < m_hdHeader.uiFrameIndexCount; iteIndex++)
			//{
			//	pIndex[0] = 0;
			//	pIndex[1] = iteIndex;

			//	pIndex += 2;
			//}
			//pIndex = nullptr;
			//m_fsMGV.write(m_pRes, static_cast<std::streamsize>(m_hdHeader.uiFrameIndexCount) * 4);
		}
	}
}

