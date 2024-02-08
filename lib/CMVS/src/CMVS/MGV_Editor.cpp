#include "MGV_Editor.h"
#include "CMVS_Types.h"
#include <Rut/RxMem.h>
#include <Rut/RxFile.h>

using namespace Rut;


namespace CMVS::MGV
{
	void Editor::Extract(const std::filesystem::path& phMGV)
	{
		RxFile::Binary ifs_mgv{ phMGV, RIO_READ };

		MGV_HDR hdr = ifs_mgv.Get<MGV_HDR>();

		uint32_t index_size = hdr.uiFrameIndexCount * 4;
		ifs_mgv.SetPos(sizeof(hdr) + index_size);

		RxMem::Auto buffer;

		buffer.ReadData(ifs_mgv, hdr.uiAudioSize);
		buffer.SaveData(phMGV.stem().replace_extension(L".ogg"));

		buffer.ReadData(ifs_mgv, hdr.uiVideoSize);
		buffer.SaveData(phMGV.stem().replace_extension(L".ogv"));
	}

	void Editor::Replace(const std::filesystem::path& phMGV, const std::filesystem::path& phVideo)
	{
		RxFile::Binary ifs_mgv{ phMGV, RIO_READ };
		RxFile::Binary ofs_mgv{ phMGV.wstring() + L".new", RIO_WRITE };

		MGV_HDR hdr = ifs_mgv.Get<MGV_HDR>();

		// Write Header
		hdr.uiVideoSize = (uint32_t)std::filesystem::file_size(phVideo);
		ofs_mgv.Write(&hdr, sizeof(hdr));

		RxMem::Auto buffer;

		//Write Index
		uint32_t index_size = hdr.uiFrameIndexCount * 4;
		buffer.ReadData(ifs_mgv, index_size);
		ofs_mgv.Write(buffer.GetPtr(), buffer.GetSize());

		//Write Audio
		buffer.ReadData(ifs_mgv, hdr.uiAudioSize);
		ofs_mgv.Write(buffer.GetPtr(), buffer.GetSize());

		//Write Video
		buffer.LoadFile(phVideo);
		ofs_mgv.Write(buffer.GetPtr(), buffer.GetSize());
	}
}

