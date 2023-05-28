#pragma once
#include <iostream>
#include <fstream>

#include "MGV_Struct.h"


namespace CMVSTools
{
	namespace MGVFile
	{
		class MGVEditor
		{
		public:
			MGVHeader m_hdHeader;
			std::wstring m_wsMGV;
			std::ifstream m_ifsMGV;

		public:
			MGVEditor(const std::wstring& wsMGV);

			void Extract();
			void Replace(const std::wstring& wsVideo);
			void Create(const std::wstring& wsVideo, const std::wstring& wsAudio);
		};
	}
}

