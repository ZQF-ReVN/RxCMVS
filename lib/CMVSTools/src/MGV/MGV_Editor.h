#pragma once
#include <string>

#include "MGV_Struct.h"


namespace CMVS::MGV
{
	class MGVEditor
	{
	public:
		MGVHeader m_Header;
		std::wstring m_wsMGV;

	public:
		MGVEditor(const std::wstring_view wsMGV);

		void Extract();
		void Replace(const std::wstring_view wsVideo);
	};
}

