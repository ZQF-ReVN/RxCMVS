#include "../../lib/CmvsTools/MGVEditor.h"

using namespace CMVSTools::MGVFile;


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

int wmain(int argc, wchar_t* argv[])
{
	try
	{
		auto show_info = []()
		{
			std::cout
				<< "MGV Editor\n"
				<< "Extract Video/Audio: MGVEditor.exe op.mgv\n"
				<< "Replace Video      : MGVEditor.exe op.mgv op.ogv\n"
				//<< "Create New MGV     : MGVEditor.exe op.mgv op.ogv op.ogg\n"
				<< std::endl;
		};

		auto extract = [](const wchar_t* wpMGV)
		{
			MGVEditor mgv(wpMGV);
			mgv.Extract();
		};

		auto replace = [](const wchar_t* wpMGV, const wchar_t* wpVideo)
		{
			MGVEditor mgv(wpMGV);
			mgv.Replace(wpVideo);
		};

		auto create = [](const wchar_t* wpMGV, const wchar_t* wpVideo, const wchar_t* wpAudio)
		{
			MGVEditor mgv(wpMGV);
			mgv.Create(wpVideo, wpAudio);
		};

		switch (argc)
		{
		case 1: { show_info(); } break;
		case 2: { extract(argv[1]); } break;
		case 3: { replace(argv[1], argv[2]); } break;
		case 4: { create(argv[1], argv[2], argv[3]); } break;
		}

	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}