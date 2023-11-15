#include "VFS_Extract.h"
#include "CMVS_Types.h"


namespace CMVS::VFS
{
	static Engine* sg_pCMVS = nullptr;
	static Fn_RunVM sg_fnCMVS_RunVM = nullptr;

	BOOL __fastcall RunVM_Hook(Engine* This, uint32_t uiEDX)
	{
		sg_pCMVS = This;
		return sg_fnCMVS_RunVM(This);
	}


	void SetFnRunVM(uint32_t fnRunVM)
	{
		sg_fnCMVS_RunVM = (Fn_RunVM)fnRunVM;
	}
}