#pragma once
#include <string>

#include "CMVS_Types.h"


namespace CMVS::MGV
{
	class Editor
	{
	public:
		MGV_HDR m_Header;
		std::wstring m_wsMGV;

	public:
		Editor(const std::wstring_view wsMGV);

		void Extract();
		void Replace(const std::wstring_view wsVideo);
	};
}

