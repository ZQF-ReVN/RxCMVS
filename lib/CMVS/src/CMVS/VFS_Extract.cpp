#include "VFS_Extract.h"
#include "CMVS_Types.h"

#include "../lib/Rut/RxFile.h"
#include "../lib/Rut/RxCmd.h"
#include "../lib/RxHook/RxHook.h"

#include <set>
#include <iostream>


namespace CMVS::VFS
{
	static Engine* sg_pEngine = nullptr;
	static Fn_RegCommPath sg_fnRegCommPath = nullptr;
	static Fn_RegVideoPath sg_fnRegVideoPath = nullptr;
	static Fn_RegImagePath sg_fnRegImagePath = nullptr;

	static std::set<VFS_Entry*> sg_stEntryPtr;


	static BOOL __fastcall RegCommPath_Hook(VFS_Comm_Index* This,uint32_t uiEDX, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9)
	{
		for (size_t ite = 0; ite < 4; ite++)
		{
			sg_stEntryPtr.insert(&This->Entry[ite]);
		}
		return sg_fnRegCommPath(This, nIndex, cpPath, a4, a5, a6, a7, a8, a9);
	}

	static BOOL __fastcall RegImagePath_Hook(VFS_Image_Index* This, uint32_t uiEDX, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9)
	{
		for (size_t ite = 0; ite < 10; ite++)
		{
			sg_stEntryPtr.insert(&This->aEntries[ite]);
		}
		return sg_fnRegImagePath(This, nIndex, cpPath, a4, a5, a6, a7, a8, a9);
	}

	static BOOL __fastcall RegVideoPath_Hook(VFS_Comm_Index* This, uint32_t uiEDX, uint32_t nIndex, const char* cpPath, uint32_t a4, uint32_t a5, uint32_t a6, uint32_t a7, uint32_t a8, uint32_t a9)
	{
		for (size_t ite = 0; ite < 4; ite++)
		{
			sg_stEntryPtr.insert(&This->Entry[ite]);
		}
		return sg_fnRegVideoPath(This, nIndex, cpPath, a4, a5, a6, a7, a8, a9);
	}



	static VFS_Entry* FindEntry(std::string_view msPackName)
	{
		for (VFS_Entry* entry_ptr : sg_stEntryPtr)
		{
			if (entry_ptr->pCoder)
			{
				if (strstr(entry_ptr->aReadPath, msPackName.data()))
				{
					return entry_ptr;
				}
			}
		}
		return nullptr;
	}

	static void PrintEntries()
	{
		for (VFS_Entry* entry_ptr : sg_stEntryPtr)
		{
			if (entry_ptr->pCoder)
			{
				Rut::RxCmd::PutFormat("%s\n", entry_ptr->aReadPath);
			}
		}
	}

	static void ExtractFile(std::string_view msPackName, std::string_view msFileName)
	{
		VFS_Entry* entry_ptr = FindEntry(msPackName);

		if (entry_ptr)
		{
			uint32_t file_size = 0;
			HLOCAL h_heap = entry_ptr->pCoder->pVtable->fnReadFile(entry_ptr->pCoder, entry_ptr, msFileName.data(), &file_size, 0, 0);
			if (h_heap && file_size)
			{
				Rut::RxFile::SaveFileViaPath(msFileName, h_heap, file_size);
				::LocalFree(h_heap);
				std::cout << "File extract successfully!\n";
			}
		}
		else
		{
			std::cout << "Not Find Pack!\n";
		}
	}

	static void __stdcall ExtractThread()
	{
		Rut::RxCmd::Alloc(L"CMVS_VFS_Extract", true);

		std::string command;
		while (true)
		{
			std::cout << "$>";
			std::cin >> command;

			if (command == "ls")
			{
				PrintEntries();
			}
			else if (command == "ext")
			{
				std::string pack_name;
				std::string file_name;

				std::cout << "PackName:";
				std::cin >> pack_name;
				std::cout << "FileName:";
				std::cin >> file_name;

				ExtractFile(pack_name, file_name);
			}

			std::cout << std::endl;
		}
	}


	// *******************
    // *   EXPORT FUNC   *
    // *******************
	void Extract(uint32_t fnRegCommPath, uint32_t fnRegImagePath, uint32_t fnRegVideoPath)
	{
		sg_fnRegCommPath = (Fn_RegCommPath)fnRegCommPath;
		sg_fnRegImagePath = (Fn_RegImagePath)fnRegImagePath;
		sg_fnRegVideoPath = (Fn_RegVideoPath)fnRegVideoPath;

		Rut::RxHook::Detours::Begin();
		Rut::RxHook::Detours::Attach(&sg_fnRegCommPath, RegCommPath_Hook);
		Rut::RxHook::Detours::Attach(&sg_fnRegImagePath, RegImagePath_Hook);
		Rut::RxHook::Detours::Attach(&sg_fnRegVideoPath, RegVideoPath_Hook);
		Rut::RxHook::Detours::Commit();

		::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ExtractThread, NULL, NULL, NULL);
	}
}