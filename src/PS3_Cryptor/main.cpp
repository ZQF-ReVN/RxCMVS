#include <iostream>
#include <stdexcept>

#include "../../lib/CMVS/PS3.h"
#include "../../lib/Rut/RxMem.h"
#include "../../lib/Rut/RxCmd.h"


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-ps3", L"ps3 path");
		arg.AddCmd(L"-save", L"save path");
		arg.AddCmd(L"-mode", L"mode [decode]:decode ps3 file, [encode]:encode ps3 file");
		arg.AddExample(L"-mode decode -ps3 sn1001.ps3 -save sn1001.ps3.dec");
		arg.AddExample(L"-mode encode -ps3 sn1001.ps3 -save sn1001.ps3.enc");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg.GetValue(L"-mode") == L"decode")
		{
			Rut::RxMem::Auto out_mem;
			CMVS::PS3::Cryptor::Decode(arg.GetValue(L"-ps3"), out_mem);
			out_mem.SaveData(arg.GetValue(L"-save"));
		}
		else if (arg.GetValue(L"-mode") == L"encode")
		{
			Rut::RxMem::Auto out_mem;
			CMVS::PS3::Cryptor::Encode(arg.GetValue(L"-ps3"), out_mem);
			out_mem.SaveData(arg.GetValue(L"-save"));
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
	
}


int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
	//::DebugMain();
}
