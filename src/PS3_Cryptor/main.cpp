#include <stdexcept>

#include "../../lib/CMVS/PS3.h"
#include "../../lib/Rut/RxMem.h"
#include "../../lib/Rut/RxConsole.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 4:
		{
			if (wcscmp(argv[1], L"decode") == 0)
			{
				Rut::RxMem::Auto out_mem;
				CMVS::PS3::Cryptor::Decode(argv[2], out_mem);
				out_mem.SaveData(argv[3]);
			}
			else if (wcscmp(argv[1], L"encode") == 0)
			{
				Rut::RxMem::Auto out_mem;
				CMVS::PS3::Cryptor::Encode(argv[2], out_mem);
				out_mem.SaveData(argv[3]);
			}
		}
		break;

		default:
		{
			Rut::RxConsole::Put(L"\n");
			Rut::RxConsole::Put(L"\tDecode: PS3_Cryptor.exe decode [ps3 path] [out path]\n");
			Rut::RxConsole::Put(L"\tEncode: PS3_Cryptor.exe encode [ps3 path] [out path]\n\n");
			Rut::RxConsole::Put(L"\tPS3_Cryptor.exe decode sn1010.ps3 sn1010.ps3.dec\n");
			Rut::RxConsole::Put(L"\tPS3_Cryptor.exe encode sn1010.ps3 sn1010.ps3.enc\n");
		}
		}
	}
	catch (const std::runtime_error& err)
	{
		Rut::RxConsole::PutFormat("\n\truntime_error:%s\n\n", err.what());
	}

}

static void DebugMain()
{
	
}


int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
	//::DebugMain();
}
