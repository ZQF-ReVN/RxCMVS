#include <iostream>
#include <stdexcept>

#include "../../lib/CMVS/PS3.h"
#include "../../lib/Rut/RxCmd.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-ps3", L"ps3 path");
		arg.AddCmd(L"-save", L"ps3 save path");
		arg.AddCmd(L"-json", L"json path");
		arg.AddCmd(L"-code", L"codepage");
		arg.AddCmd(L"-mode", L"[export]:export texts, [import]:import texts");
		arg.AddExample(L"-mode export -ps3 sn1001.ps3 -json sn1001.ps3.json -code 932");
		arg.AddExample(L"-mode import -ps3 sn1001.ps3 -json sn1001.ps3.json -save sn1001.ps3.new -code 932");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg.GetValue(L"-mode") == L"export")
		{
			CMVS::PS3::TextEditor editor;
			editor.Init(arg.GetValue(L"-ps3"));
			editor.Export(arg.GetValue(L"-json"), ::_wtoi(arg.GetValue(L"-code").c_str()));
			Rut::RxCmd::PutFormat(L"Success: %s", arg.GetValue(L"-ps3"));
		}
		else if (arg.GetValue(L"-mode") == L"import")
		{
			CMVS::PS3::TextEditor editor;
			editor.Init(arg.GetValue(L"-ps3"));
			if (editor.Import(arg.GetValue(L"-json"), arg.GetValue(L"-save"), ::_wtoi(arg.GetValue(L"-code").c_str())))
			{
				Rut::RxCmd::PutFormat(L"Success: %s", arg.GetValue(L"-ps3"));
			}
			else
			{
				Rut::RxCmd::PutFormat(L"\n\tFailure: %s\n", arg.GetValue(L"-ps3"));
			}
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
	{
		CMVS::PS3::TextEditor editor;
		editor.Init(L"sn1001.ps3.new");
		editor.Export(L"sn1001.ps3.new.json", 932);
	}
	{
		//CMVS::PS3::TextEditor editor;
		//editor.Init(L"sn1001.ps3.dec");
		//editor.Insert(L"sn1001.ps3.json", L"sn1001.ps3.new", 932);
	}
}


int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
	//::DebugMain();
}
