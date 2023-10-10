#pragma once
#include <cstdint>


namespace CMVS::CMV
{
	struct ImageSecData
	{
		uint8_t* pSec0;
		uint8_t* pSec1;
		uint8_t* pSec2;
		uint8_t* pSec3;
		uint8_t* pSec4;
		uint8_t* pSec5;
		uint8_t* pSec6;
		uint8_t* pSec7;
		uint8_t* pSec8;
		uint8_t* pSec9;
		uint8_t* pSecA;
		uint8_t* pSecB;
		uint8_t* pSecC;
		uint8_t* pSecD;
		uint8_t* pSecE;
	};

	struct ImageSecWidthBites
	{
		uint32_t szSec0;
		uint32_t szSec1;
		uint32_t szSec2;
		uint32_t szSec3;
		uint32_t szSec4;
		uint32_t szSec5;
		uint32_t szSec6;
		uint32_t szSec7;
		uint32_t szSec8;
		uint32_t szSec9;
		uint32_t szSecA;
		uint32_t szSecB;
		uint32_t szSecC;
		uint32_t szSecD;
		uint32_t szSecE;
	};


	typedef uint32_t(__stdcall* fnDecJBP1Buffer)(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, uint8_t* pJBP1Buffer, uint32_t* szJBP1Buffer, uint32_t dwUnknow);
	typedef uint32_t(__stdcall* fnDecJBPDBuffer)(ImageSecData* lpImageSecData, ImageSecWidthBites* lpImageSecWidthBites, uint8_t* pJBPDBuffer, uint32_t* szJBPDBuffer, uint32_t dwUnknow);
	typedef uint32_t(__thiscall* fnTransFrame)(uint32_t* pThis, uint32_t a2, uint8_t* pJBPXBuffer, uint32_t* szJBPXBuffer, uint32_t a5);
	typedef uint8_t* (__fastcall* fnGetJBPXBuffer)(uint32_t* pTHIS, uint32_t dwEDX, uint32_t dwSequence, uint32_t* szJBPDBuffer);
}