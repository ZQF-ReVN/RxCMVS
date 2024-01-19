#include <stdexcept>

#include "../../lib/CMVS/PS3.h"
#include "../../lib/Rut/RxConsole.h"

using namespace Rut;


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 5:
		{
			std::wstring_view ps3_path = argv[2];
			std::wstring_view out_path = argv[3];

			size_t code_page = 932;
			wchar_t* end = nullptr;
			code_page = ::wcstol(argv[4], &end, 10);

			CMVS::PS3::TextEditor editor;
			editor.Init(ps3_path);
			editor.Export(out_path, code_page);
			RxConsole::PutFormat(L"Success: %s", ps3_path);
		}
		break;

		case 6:
		{
			std::wstring_view ps3_path = argv[2];
			std::wstring_view json_path = argv[3];
			std::wstring_view out_path = argv[4];

			size_t code_page = 936;
			wchar_t* end = nullptr;
			code_page = ::wcstol(argv[5], &end, 10);

			CMVS::PS3::TextEditor editor;
			editor.Init(ps3_path);
			if (editor.Import(json_path, out_path, code_page))
			{
				RxConsole::PutFormat(L"Success: %s", ps3_path);
			}
			else
			{
				RxConsole::PutFormat(L"\n\tFailure: %s\n", ps3_path);
			}
		}
		break;

		default:
		{
			RxConsole::Put(L"\n");
			RxConsole::Put(L"\tExport: PS3_TextEditor.exe export [ps3 path] [json path] [extract codepage]\n");
			RxConsole::Put(L"\tImport: PS3_TextEditor.exe import [ps3 path] [json path] [new ps3 path] [insert codepage]\n\n");
			RxConsole::Put(L"\tPS3_TextEditor.exe export sn1010.ps3 sn1010.ps3.json 936\n");
			RxConsole::Put(L"\tPS3_TextEditor.exe import sn1010.ps3 sn1010.ps3.json sn1010.ps3.new 936\n");
		}
		}
	}
	catch (const std::runtime_error& err)
	{
		RxConsole::PutFormat("\n\truntime_error:%s\n\n", err.what());
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