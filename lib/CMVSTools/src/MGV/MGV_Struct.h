#pragma once
#include <cstdint>


namespace CMVS::MGV
{
	//-MGVFile
	//--MGVHeader
	//--IndexCount * 4
	//--Audio
	//--Video
	struct MGVHeader
	{
		uint8_t  aSignature[4];
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