#include <stdexcept>

#include "../../lib/CMVS/MGV.h"
#include "../../lib/Rut/RxConsole.h"


static void UserMain(int argc, char* argv[])
{
	try
	{
		switch (argc)
		{
		case 2:
		{
			CMVS::MGV::Editor::Extract(argv[1]);
		}
		break;

		case 3:
		{
			CMVS::MGV::Editor::Replace(argv[1], argv[2]);
		}
		break;

		default:
		{
			Rut::RxConsole::Put("MGV Editor\n");
			Rut::RxConsole::Put("Extract Video/Audio: MGV_Editor.exe op.mgv\n");
			Rut::RxConsole::Put("Replace Video: MGV_Editor.exe op.mgv op.ogv\n\n");
		}
		break;
		}

	}
	catch (const std::runtime_error& err)
	{
		Rut::RxConsole::PutFormat("\n\truntime_error:%s\n\n", err.what());
	}
}

static void DebugMain()
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


int main(int argc, char* argv[])
{
	::UserMain(argc, argv);
}
