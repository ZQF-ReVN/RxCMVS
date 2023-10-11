#pragma once
#include "../../../Rut/RxMem.h"


namespace CMVS::PS3
{
    class Coder
    {
    public:
        static void Decode(std::wstring_view wsPath, Rut::RxMem::Auto& rfDecode);
        static void Encode(std::wstring_view wsPath, Rut::RxMem::Auto& rfEncode);

    private:
        static void KeyDecData(uint8_t* pData, size_t uiDataSize, uint32_t uiKey);
        static void KeyEncData(uint8_t* pData, size_t uiDataSize, uint32_t uiKey);

        static void LZSS_Decode(uint8_t* pEnc, size_t nEncSize, uint8_t* pDec);
        static size_t LZSS_Encode_Fake(uint8_t* pRaw, size_t nRawSize, uint8_t* pEnc);
    };
}
