#pragma once
#include "CMV6_Hook_Def.h"


namespace CMVS::CMV
{
	//For 1280 x 720
	//MergeImage Frome 15 Sectors
	//PerLineBytes = 0x400
	//ImageHight 1,2,3,4,5,6,7,8,9 = 0x100; A,B,C,D,E = 0xD0
	void MergeImage720(ImageSecData* lpImageSecData, uint8_t* pMergeBuffer)
	{
		for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
		{
			memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSec0[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSec1[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSec2[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSec3[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSec4[0x400 * iteHight], 0x400);
			pMergeBuffer += 0x400 * 5;
		}

		for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
		{
			memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSec5[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSec6[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSec7[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSec8[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSec9[0x400 * iteHight], 0x400);
			pMergeBuffer += 0x400 * 5;
		}

		for (size_t iteHight = 0; iteHight < 0xD0; iteHight++)
		{
			memcpy(&pMergeBuffer[0x400 * 0], &lpImageSecData->pSecA[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 1], &lpImageSecData->pSecB[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 2], &lpImageSecData->pSecC[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 3], &lpImageSecData->pSecD[0x400 * iteHight], 0x400);
			memcpy(&pMergeBuffer[0x400 * 4], &lpImageSecData->pSecE[0x400 * iteHight], 0x400);
			pMergeBuffer += 0x400 * 5;
		}
	}

	//For 1280 x 720
	//SplitImage Frome 15 Sectors
	//PerLineBytes = 0x400
	//ImageHight 1,2,3,4,5,6,7,8,9 = 0x100; A,B,C,D,E = 0xD0
	void SplitImage720(ImageSecData* lpImageSecData, uint8_t* pSplitBuffer)
	{
		for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
		{
			memcpy(&lpImageSecData->pSec0[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
			memcpy(&lpImageSecData->pSec0[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
			memcpy(&lpImageSecData->pSec1[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
			memcpy(&lpImageSecData->pSec2[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
			memcpy(&lpImageSecData->pSec3[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
			memcpy(&lpImageSecData->pSec4[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
			pSplitBuffer += 0x400 * 5;
		}

		for (size_t iteHight = 0; iteHight < 0x100; iteHight++)
		{
			memcpy(&lpImageSecData->pSec5[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
			memcpy(&lpImageSecData->pSec6[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
			memcpy(&lpImageSecData->pSec7[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
			memcpy(&lpImageSecData->pSec8[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
			memcpy(&lpImageSecData->pSec9[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
			pSplitBuffer += 0x400 * 5;
		}

		for (size_t iteHight = 0; iteHight < 0xD0; iteHight++)
		{
			memcpy(&lpImageSecData->pSecA[0x400 * iteHight], &pSplitBuffer[0x400 * 0], 0x400);
			memcpy(&lpImageSecData->pSecB[0x400 * iteHight], &pSplitBuffer[0x400 * 1], 0x400);
			memcpy(&lpImageSecData->pSecC[0x400 * iteHight], &pSplitBuffer[0x400 * 2], 0x400);
			memcpy(&lpImageSecData->pSecD[0x400 * iteHight], &pSplitBuffer[0x400 * 3], 0x400);
			memcpy(&lpImageSecData->pSecE[0x400 * iteHight], &pSplitBuffer[0x400 * 4], 0x400);
			pSplitBuffer += 0x400 * 5;
		}
	}
}
