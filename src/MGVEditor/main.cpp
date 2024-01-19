#include <stdexcept>

#include "../../lib/CMVS/MGV.h"
#include "../../lib/Rut/RxConsole.h"

using namespace CMVS::MGV;


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 2: 
		{
			std::wstring_view mgv_path = argv[1];
			Editor::Extract(mgv_path);
		}
		break;

		case 3: 
		{
			std::wstring_view mgv_path = argv[1];
			std::wstring_view video_path = argv[2];
			Editor::Replace(mgv_path, video_path);
		}
		break;

		default:
		{
			Rut::RxConsole::Put("MGV Editor\n");
			Rut::RxConsole::Put("Extract Video/Audio: MGVEditor.exe op.mgv\n""Extract Video/Audio: MGVEditor.exe op.mgv\n");
			Rut::RxConsole::Put("Replace Video: MGVEditor.exe op.mgv op.ogv\n\n");
		}
		break;
		}

	}
	catch (const std::runtime_error& err)
	{
		Rut::RxConsole::PutFormat("\n\truntime_error:%s\n\n", err.what());
	}
}

void PackMGV()
{
	//MGVPack mgv(L"1.mgv");
	//MGVHeader header = { 0 };
	//header.aSignature[0] = 'M';
	//header.aSignature[1] = 'G';
	//header.aSignature[2] = 'V';
	//header.aSignature[3] = '1';

	//header.uiVideoSize = 0x59F4762;
	//header.uiAudioSize = 0x192566;
	//header.uiBitCount = 32;
	//header.usWidth = 1280;
	//header.usHight = 720;
	//header.uiFrameIndexCount = 0x0DF0; // FrameRate * Time. If Value Too Large Game Will Get Stuck After Playback
	//header.uiFrameRateTime = 1000; //1s
	//header.uiFrameRate = 30 * 1000; //30 * 1000

	//mgv.InitMGVInfo(&header);
	//mgv.MakePack(L"op.mgv.ogg", L"op.mgv.ogv");
}