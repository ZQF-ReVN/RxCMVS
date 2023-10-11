#include "../../lib/Rut/RxConsole.h"
#include "../../lib/CMVSTools/PS3.h"

using namespace Rut;


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 4:
		{
			size_t code_page = 932;
			wchar_t* end = nullptr;
			code_page = wcstol(argv[3], &end, 10);

			std::wstring_view ps3_path = argv[1];
			std::wstring_view out_path = argv[2];

			CMVS::PS3::Editor editor;
			editor.Init(ps3_path);
			editor.Extract(out_path, code_page);
			RxConsole::PutFormat(L"Success: %s", ps3_path);
		}
		break;

		case 5:
		{
			size_t code_page = 936;
			wchar_t* end = nullptr;
			code_page = wcstol(argv[3], &end, 10);

			std::wstring_view ps3_path = argv[1];
			std::wstring_view json_path = argv[2];
			std::wstring_view out_path = argv[3];

			CMVS::PS3::Editor editor;
			editor.Init(ps3_path);
			if (editor.Insert(json_path, out_path, code_page))
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
			RxConsole::Put(L"\tExtract: PS3TextEditor.exe [ps3 path] [json path] [extract codepage]\n");
			RxConsole::Put(L"\tInsert: PS3TextEditor.exe [ps3 path] [json path] [new ps3 path] [insert codepage]\n\n");
			RxConsole::Put(L"\tPS3TextEditor sn1010.ps3 sn1010.ps3.json 936\n");
			RxConsole::Put(L"\tPS3TextEditor sn1010.ps3 sn1010.ps3.json sn1010.ps3.new 936\n");
		}
		}
	}
	catch (const std::runtime_error& err)
	{
		RxConsole::Put("\nException:\n\t");
		RxConsole::Put(err.what());
		RxConsole::Put("\n");
	}
	
}

//struct Text_Entry
//{
//	std::wstring wsText;
//	uint32_t uiRvaValRva{}; // A Value   [Text Data Rva] In Block
//	uint32_t uiRvaPtrRva{}; // A Pointer [Text Data Rva’s Rva] In Code Block
//};
//
//void ReadEntry(std::wstring_view wsText, std::vector<Text_Entry>& vcEntry)
//{
//	Rut::RxStream::Text txt{ wsText, Rut::RIO::RIO_IN, Rut::RFM::RFM_UTF16 };
//	std::vector<std::wstring> texts;
//	txt.ReadAllLine(texts);
//
//	for (size_t ite = 0; ite < texts.size(); ite++)
//	{
//		std::wstring& line = texts[ite];
//		if (line.empty()) { continue; }
//
//		size_t foa_ptr = 0;
//		size_t foa_rva = 0;
//		(void)swscanf_s(line.c_str(), L"[Text:0x%08X Code:0x%08X]", &foa_rva, &foa_ptr);
//
//		ite += 2;
//		std::wstring tra = texts[ite];
//		ite++;
//
//
//		if (tra.find(L"Tra:") == std::wstring::npos)
//		{
//			throw std::runtime_error("Error!");
//		}
//
//		tra = tra.substr(4);
//
//		Text_Entry entry;
//		entry.uiRvaPtrRva = foa_ptr;
//		entry.uiRvaValRva = foa_rva;
//		entry.wsText = tra;
//		vcEntry.emplace_back(std::move(entry));
//	}
//}
//
//void ToJson(std::wstring wsFile)
//{
//	std::vector<Text_Entry> entry_list;
//	ReadEntry(L"text/" + wsFile + L".txt", entry_list);
//
//	Rut::RxJson::Value json;
//	Rut::RxJson::Parser parser;
//	parser.Open(L"json/" + wsFile + L".json");
//	parser.Read(json);
//
//	Rut::RxJson::JArray& jarr = json[L"Scenario"];
//	if (jarr.size() != entry_list.size())
//	{
//		throw std::runtime_error("Error!");
//	}
//
//	auto fn_ToStrHex = [](std::wstring_view wsStr) -> size_t
//		{
//			size_t value = 0;
//			(void)swscanf_s(wsStr.data(), L"0x%08X", &value);
//			return value;
//		};
//
//	for (size_t i = 0; i < jarr.size(); i++)
//	{
//		Text_Entry& entry = entry_list[i];
//		if (entry.uiRvaPtrRva != fn_ToStrHex(jarr[i][L"FOA_Ptr"]))
//		{
//			throw std::runtime_error("Error!");
//		}
//
//		if (entry.uiRvaValRva != fn_ToStrHex(jarr[i][L"FOA_Rva"]))
//		{
//			throw std::runtime_error("Error!");
//		}
//
//		jarr[i][L"Text_Tra"] = entry.wsText;
//	}
//
//	parser.Save(json, L"new/" + wsFile + L".json.new");
//}
//
//void OldFormatToJson()
//{
//	std::vector<std::wstring> file_list;
//	Rut::RxPath::CurFileNames(L"Script_jp_v1.02\\", file_list);
//	for (auto& file : file_list)
//	{
//		ToJson(file);
//	}
//}