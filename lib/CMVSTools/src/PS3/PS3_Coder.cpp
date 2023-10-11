#include "PS3_Coder.h"
#include "PS3_Struct.h"


namespace CMVS::PS3
{
    void Coder::Decode(std::wstring_view wsPath, Rut::RxMem::Auto& rfBuffer)
    {
        Rut::RxMem::Auto tmp_ps3(wsPath);
        uint8_t* enc_file_ptr = tmp_ps3.GetPtr();
        size_t enc_file_size = tmp_ps3.GetSize();
        PS3_HDR* enc_hdr_ptr = (PS3_HDR*)enc_file_ptr;

        uint8_t* enc_data_comp_ptr = enc_file_ptr + enc_hdr_ptr->uiHeaderSize;
        size_t enc_data_comp_size = enc_file_size - enc_hdr_ptr->uiHeaderSize;
        KeyDecData(enc_data_comp_ptr, enc_data_comp_size, enc_hdr_ptr->uiDataDecodeKey);

        rfBuffer.SetSize(enc_hdr_ptr->uiDataDecompressSize + enc_hdr_ptr->uiHeaderSize);

        uint8_t* dec_file_ptr = rfBuffer.GetPtr();
        uint8_t* dec_data_ptr = dec_file_ptr + enc_hdr_ptr->uiHeaderSize;
        LZSS_Decode(enc_data_comp_ptr, enc_hdr_ptr->uiDataCompressSize, dec_data_ptr);

        memcpy(dec_file_ptr, enc_file_ptr, enc_hdr_ptr->uiHeaderSize);
    }

    void Coder::Encode(std::wstring_view wsPath, Rut::RxMem::Auto& rfBuffer)
    {
        Rut::RxMem::Auto tmp_ps3(wsPath);
        uint8_t* dec_file_ptr = tmp_ps3.GetPtr();
        size_t dec_file_size = tmp_ps3.GetSize();

        PS3_HDR* dec_hdr_ptr = (PS3_HDR*)dec_file_ptr;
        uint8_t* dec_data_ptr = dec_file_ptr + dec_hdr_ptr->uiHeaderSize;


        rfBuffer.SetSize(dec_file_size * 2);
        uint8_t* enc_file_ptr = rfBuffer.GetPtr();
        memcpy(enc_file_ptr, dec_file_ptr, dec_hdr_ptr->uiHeaderSize);

        PS3_HDR* enc_hdr_ptr = (PS3_HDR*)enc_file_ptr;
        uint8_t* enc_data_comp_ptr = enc_file_ptr + enc_hdr_ptr->uiHeaderSize;
        enc_hdr_ptr->uiDataCompressSize = (uint32_t)LZSS_Encode_Fake(dec_data_ptr, dec_hdr_ptr->uiDataDecompressSize, enc_data_comp_ptr);
        KeyEncData(enc_data_comp_ptr, enc_hdr_ptr->uiDataCompressSize, enc_hdr_ptr->uiDataDecodeKey);

        rfBuffer.SetSize(enc_hdr_ptr->uiDataCompressSize + enc_hdr_ptr->uiHeaderSize);
    }


    static uint8_t Ror8(uint8_t ucValue, uint8_t ucShift)
    {
        return (ucValue >> ucShift) | (ucValue << (8 - ucShift));
    }

    static uint8_t Rol8(uint8_t ucValue, uint8_t ucShift)
    {
        return (ucValue << ucShift) | (ucValue >> (8 - ucShift));
    }

    void Coder::KeyDecData(uint8_t* pData, size_t uiDataSize, uint32_t uiKey)
    {
        if (uiDataSize == 0) { return; }

        uint8_t dec_key = ((uint8_t)(uiKey >> 3)) + ((uint8_t)(uiKey >> 24));
        uint8_t dec_shift = (uint8_t)((((uint32_t)(uiKey >> 20)) % 5) + 1);

        for (uint32_t ite = 0; ite < uiDataSize; ite++)
        {
            uint8_t tmp_byte = pData[ite];
            tmp_byte -= 0x7C;
            tmp_byte = dec_key ^ tmp_byte;
            tmp_byte = Ror8(tmp_byte, dec_shift);
            pData[ite] = tmp_byte;
        }
    }

    void Coder::KeyEncData(uint8_t* pData, size_t uiDataSize, uint32_t uiKey)
    {
        if (uiDataSize == 0) { return; }

        uint8_t dec_key = ((uint8_t)(uiKey >> 3)) + ((uint8_t)(uiKey >> 24));
        uint8_t dec_shift = (uint8_t)((((uint32_t)(uiKey >> 20)) % 5) + 1);

        for (uint32_t ite = 0; ite < uiDataSize; ite++)
        {
            uint8_t tmp_byte = pData[ite];
            tmp_byte = Rol8(tmp_byte, dec_shift);
            tmp_byte = dec_key ^ tmp_byte;
            tmp_byte += 0x7C;
            pData[ite] = tmp_byte;
        }
    }

    void Coder::LZSS_Decode(uint8_t* pEnc, size_t nEncSize, uint8_t* pDec)
    {
        constexpr size_t slider_size = 0x800;
        constexpr size_t slider_max_index = 0x7FF;
        constexpr size_t slider_beg_index = 0x7DF;

        uint8_t* dec_ptr = pDec;
        uint8_t* enc_ptr = pEnc;
        uint8_t* enc_end = pEnc + nEncSize;

        uint32_t token = 0;
        uint8_t slider_buf[slider_size] = { 0 };
        size_t ite_slider = slider_beg_index;

        while (true)
        {
            token >>= 1;

            if ((token & 0x100) == 0) // read token
            {
                if (enc_ptr >= enc_end) return;
                token = enc_ptr++[0] | 0xFF00;
            }

            if (enc_ptr >= enc_end) return;

            if (token & 1) // not compress
            {
                uint8_t tmp = enc_ptr[0];

                dec_ptr++[0] = tmp;
                slider_buf[ite_slider] = tmp;
                ite_slider = (ite_slider + 1) & slider_max_index;

                enc_ptr += 1;
            }
            else // compress
            {
                if ((enc_ptr + 1) >= enc_end) return;

                size_t copy_len = (enc_ptr[1] & 0x1F) + 1;
                size_t copy_off = ((enc_ptr[1] & 0xE0) << 3) | enc_ptr[0];

                for (size_t ite = 0; ite <= copy_len; ite++)
                {
                    size_t off = (ite + copy_off) & slider_max_index;
                    uint8_t tmp = slider_buf[off];

                    dec_ptr++[0] = tmp;
                    slider_buf[ite_slider] = tmp;
                    ite_slider = (ite_slider + 1) & slider_max_index;
                }

                enc_ptr += 2;
            }
        }
    }

    size_t Coder::LZSS_Encode_Fake(uint8_t* pRaw, size_t nRawSize, uint8_t* pEnc)
    {
        uint8_t* pEnc_Back = pEnc;
        size_t align = nRawSize % 8;
        for (size_t iteRaw = 0; iteRaw < ((nRawSize - align) / 8); iteRaw++)
        {
            pEnc[0] = 0xFF;
            memcpy(pEnc + 1, pRaw, 8);

            pEnc += 9;
            pRaw += 8;
        }

        if (align)
        {
            pEnc[0] = 0xFF;
            memcpy(pEnc + 1, pRaw, align);
            pEnc += align + 1;
        }

        return (size_t)(pEnc - pEnc_Back);
    }
}