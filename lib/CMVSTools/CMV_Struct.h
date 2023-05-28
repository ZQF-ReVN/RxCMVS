#pragma once
#include <cstdint>

namespace CMVSTools
{
	namespace CMV
	{
		//CMVFile (Some As CMV4 CMV3)
		/*
		* CMVFileHeader
		* CMVIndexDescriptor[uiResCount + 1];
		* ResourcesData
		*/
		struct CMV_Header
		{
			uint8_t aSignature[4];      // CMV6 CMV4 CMV3
			uint32_t uiResSecOffset;
			uint32_t uiCMVFileSize;
			uint32_t uiUnknow0;         // Not Used
			uint32_t uiResMaxSequence;  // Start With Zero
			uint32_t uiFrameRate;
			uint32_t uiFrameRateTime;   // Per Second
			uint32_t uiImageWidth;
			uint32_t uiImageHight;
			uint32_t uiBitCount;
			uint32_t uiAudioFlag;       // About Audio File Load If 0x0 Game Will Crash In MOG_INputMemory
		};

		struct CMV_Entry
		{
			uint32_t uiSequence;        // Max = uiResCount
			uint32_t uiCmpSize;
			uint32_t uiOrgSize;         // Pixel Array Size ((uiBitCount / 8) * uiImageWidth * uiImageHight)
			uint32_t uiResType;         // 0 = ogg, 02 = jbpx
			uint32_t uiOffset;          // ResFileOffset = uiOffset + uiResOffset
		};

		enum CMV_ResType
		{
			OGG,
			WEBP, //Custom
			JBPX
		};
	}
}