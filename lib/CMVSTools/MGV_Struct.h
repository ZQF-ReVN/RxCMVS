#pragma once
#include <cstdint>

namespace CMVSTools
{
	namespace MGVFile
	{
		//-MGVFile
		//--MGVHeader
		//--IndexCount * 4
		//--Audio
		//--Video
		struct MGVHeader
		{
			char aSignature[4];
			uint32_t uiBitCount;
			uint32_t uiVideoSize;
			uint32_t uiAudioSize;
			uint32_t uiFrameIndexCount;
			uint16_t usWidth;
			uint16_t usHight;
			uint32_t uiFrameRateTime; //1s = 1000ms
			uint32_t uiFrameRate; // FrameRate * 1000
		};
	}
}