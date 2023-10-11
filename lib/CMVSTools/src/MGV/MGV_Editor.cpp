#include "MGV_Editor.h"
#include "../../../Rut/RxFS.h"
#include "../../../Rut/RxMem.h"

using namespace Rut;


namespace CMVS::MGV
{
	MGVEditor::MGVEditor(const std::wstring_view wsMGV) : m_Header({}), m_wsMGV(wsMGV)
	{
		RxFS::Binary{ m_wsMGV, Rut::RIO::RIO_IN } >> m_Header;
	}

	void MGVEditor::Extract()
	{
		RxMem::Auto tmp;
		uint32_t index_size = m_Header.uiFrameIndexCount * 4;

		// Restore File Pointer
		RxFS::Binary ifs_mgv{ m_wsMGV, Rut::RIO::RIO_IN };
		ifs_mgv.SetPos(sizeof(m_Header));

		// Unpack Audio
		uint8_t* buf_ptr = tmp.SetSize(m_Header.uiAudioSize);
		ifs_mgv.MovePos(index_size);
		ifs_mgv.Read(buf_ptr, m_Header.uiAudioSize);
		RxFS::SaveFileViaPath((m_wsMGV + L".ogg").c_str(), buf_ptr, m_Header.uiAudioSize);

		// Unpack Video
		buf_ptr = tmp.SetSize(m_Header.uiVideoSize);
		ifs_mgv.Read(buf_ptr, m_Header.uiVideoSize);
		RxFS::SaveFileViaPath((m_wsMGV + L".ogv").c_str(), buf_ptr, m_Header.uiVideoSize);
	}

	void MGVEditor::Replace(const std::wstring_view wsVideo)
	{
		RxMem::Auto tmp;
		RxFS::Binary ifs_mgv{ m_wsMGV, Rut::RIO::RIO_IN};
		RxFS::Binary ofs_mgv{ m_wsMGV + L".new", Rut::RIO::RIO_OUT};

		// Write Header
		m_Header.uiVideoSize = (uint32_t)RxPath::FileSize(wsVideo);
		ofs_mgv.Write(&m_Header, sizeof(m_Header));

		//Write Index
		uint32_t index_size = m_Header.uiFrameIndexCount * 4;
		uint8_t* index_ptr = tmp.SetSize(index_size);
		ifs_mgv.MovePos(sizeof(this->m_Header));
		ifs_mgv.Read(index_ptr, index_size);
		ofs_mgv.Write(index_ptr, index_size);

		//Write Audio
		uint32_t audio_size = m_Header.uiAudioSize;
		uint8_t* audio_ptr = tmp.SetSize(audio_size);
		ifs_mgv.Read(audio_ptr, audio_size);
		ofs_mgv.Write(audio_ptr, audio_size);

		//Write Video
		uint32_t video_size = m_Header.uiVideoSize;
		uint8_t* video_ptr = tmp.SetSize(video_size);
		RxFS::Binary{ wsVideo, Rut::RIO::RIO_IN }.Read(video_ptr, video_size);
		ofs_mgv.Write(video_ptr, video_size);
	}
}

