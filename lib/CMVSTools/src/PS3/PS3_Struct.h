#pragma once
#include <cstdint>

struct PS3_HDR
{
    uint32_t uiSignature;
    uint32_t uiHeaderSize;
    uint32_t uiUn0;
    uint32_t uiDataDecodeKey;
    uint32_t uiTextCount;
    uint32_t uiCodeBlockSize;
    uint32_t uiUnBlockSize;
    uint32_t uiTextBlockSize;
    uint32_t uiUn2;
    uint32_t uiDataCompressSize;
    uint32_t uiDataDecompressSize;
    uint32_t uiUn3;
};