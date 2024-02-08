#include <iostream>
#include <stdexcept>

#include <CMVS/MGV_Editor.h>
#include <Rut/RxCmd.h>


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-mgv", L"mgv file");
		arg.AddCmd(L"-ogv", L"ogv path");
		arg.AddCmd(L"-mode", L"mode [extract]:extract ogv ogg file, [replace]:repalce ogv file");
		arg.AddExample(L"-mode extract -mgv op.mgv");
		arg.AddExample(L"-mode replace -mgv op.mgv -ogv op.ogv");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"extract")
		{
			CMVS::MGV::Editor::Extract(arg[L"-mgv"]);
		}
		else if (arg[L"-mode"] == L"replace")
		{
			CMVS::MGV::Editor::Replace(arg[L"-mgv"], arg[L"-ogv"]);
		}
		else
		{
			throw std::runtime_error("Error Command!");
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
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


int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
}
