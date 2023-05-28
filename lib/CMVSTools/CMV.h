#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "CMV_Struct.h"
#include "../Rxx/Mem.h"


namespace CMVSTools
{
	namespace CMV
	{
		class CMVEditor
		{
		private:
			CMV_Header m_Header;
			Rut::MemX::AutoMem m_AutoMem;

			std::wstring m_wsCMV;
			std::wstring m_wsFolder;
			std::ifstream m_ifsCMV;
			std::vector<CMV_Entry> m_vcIndex;

			void SaveFrame(CMV_Entry& ceEntry);
			void ReadIndex();

		public:
			CMVEditor(const std::wstring& wsCMV);

			void ExtractAll();
			void ExtractViaSeq(uint32_t uiSequence);

			static wchar_t* MakeFileName(uint32_t uiSequence, uint32_t uiType);
		};
	}
}

