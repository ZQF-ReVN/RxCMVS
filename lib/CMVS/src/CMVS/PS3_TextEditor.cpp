#include "PS3_TextEditor.h"

#include <Rut/RxStr.h>
#include <Rut/RxJson.h>
#include <Rut/RxFile.h>

using namespace Rut;


namespace CMVS::PS3
{
	static std::wstring NumToStr(const wchar_t* wpFormat, size_t nValue)
	{
		wchar_t buf[0x10];
		size_t len = (size_t)swprintf_s(buf, 0x10, wpFormat, nValue);
		return { buf, len };
	}

	void TextEditor::Init(std::wstring_view wsPath)
	{
		m_wsPath = wsPath;
		m_amPS3.LoadFile(wsPath);
		this->ReadEntry();
	}

	bool TextEditor::Import(std::wstring_view wsJsonPath, std::wstring_view wsSavePath, size_t nCodePage)
	{
		// Check
		if (this->m_vecTextIndex.empty()) { return false; }

		// Read Json
		RxJson::JValue json = RxJson::Parser{}.Load(wsJsonPath);
		RxJson::JArray& jarr_scn = json[L"Scenario"];

		// Check
		if (jarr_scn.size() != m_vecTextIndex.size()) { return false; }


		// Read To TextIndex
		size_t append_text_size = 0;
		size_t text_seg_len = this->GeTextSegSize();
		uint8_t* code_seg_ptr = this->GetCodeSegPtr();
		for (size_t ite = 0; ite < this->m_vecTextIndex.size(); ite++)
		{
			// Read Text
			Text_Entry& entry = m_vecTextIndex[ite];
			entry.msText = RxStr::ToMBCS(jarr_scn[ite][L"tra"].ToStrView(), nCodePage);

			if (entry.msText.empty()) { throw std::runtime_error("CMVS::PS3::Editor::Insert Error! Text Empty"); }

			// Cal New Text Rva
			entry.uiRvaValRva = (uint32_t)(text_seg_len + append_text_size);
			append_text_size += entry.msText.size() + 1;

			// Write New Text Rva
			uint32_t* rva_ptr = (uint32_t*)(code_seg_ptr + entry.uiRvaPtrRva);
			*rva_ptr = entry.uiRvaValRva;
		}


		// Write Append Data
		RxMem::Auto append_text_data;
		append_text_data.SetSize(append_text_size);
		uint8_t* append_text_ptr = append_text_data.GetPtr();
		for (auto& entry : m_vecTextIndex)
		{
			size_t str_full_len = entry.msText.size() + 1;
			memcpy(append_text_ptr, entry.msText.data(), str_full_len);
			append_text_ptr += str_full_len;
		}
		this->m_amPS3.Append(append_text_data);

		// Set Header Data
		PS3_HDR* hdr_ptr = this->GetHdrPtr();
		hdr_ptr->uiDataDecompressSize += (uint32_t)append_text_size;
		hdr_ptr->uiTextBlockSize += (uint32_t)append_text_size;

		// Save Data
		this->m_amPS3.SaveData(wsSavePath);

		return true;
	}

	void TextEditor::Export(std::wstring_view wsPath, size_t nCodePage)
	{
		size_t hdr_len = this->GetHdrSize();
		size_t code_len = this->GetCodeSegSize();

		RxJson::JValue json_root;
		RxJson::JArray& jarr_scn = json_root[L"Scenario"].ToAry();
		for (auto& entry : m_vecTextIndex)
		{
			std::wstring text;
			RxStr::ToWCS(entry.msText, text, nCodePage);

			RxJson::JObject jobj_info;
			jobj_info[L"raw"] = text;
			jobj_info[L"tra"] = text;

			if(false)
			{
				jobj_info[L"FOA_Ptr"] = NumToStr(L"0x%08x", hdr_len + entry.uiRvaPtrRva);
				jobj_info[L"FOA_Rva"] = NumToStr(L"0x%08x", hdr_len + code_len + entry.uiRvaValRva);
			}

			jarr_scn.push_back(jobj_info);
		}

		RxJson::Parser::Save(json_root, wsPath, true);
	}


	void TextEditor::ReadEntry()
	{
		constexpr uint8_t push_str_opcode_beg[4] = { 0x01, 0x02, 0x20, 0x01 };
		constexpr uint8_t push_str_opcode_end[4] = { 0x0F, 0x02, 0x30, 0x04 };

		Text_Entry entry;
		uint8_t* code_seg_ptr = this->GetCodeSegPtr();
		uint32_t code_seg_size = this->GetCodeSegSize();
		for (size_t ite = 0; ite < code_seg_size; ite++)
		{
			if (memcmp(push_str_opcode_beg, (code_seg_ptr + ite), sizeof(push_str_opcode_beg))) { continue; }
			if (memcmp(push_str_opcode_end, (code_seg_ptr + ite + sizeof(push_str_opcode_beg) + 4), sizeof(push_str_opcode_end))) { continue; }

			entry.uiRvaPtrRva = (uint32_t)(ite + sizeof(push_str_opcode_beg));
			entry.uiRvaValRva = *(uint32_t*)(code_seg_ptr + entry.uiRvaPtrRva);

			// Filter String
			std::string_view ms_text = (char*)this->GetTextSegPtr() + entry.uiRvaValRva;
			if (this->Filter(ms_text)) { continue; }

			entry.msText = ms_text;
			m_vecTextIndex.emplace_back(entry);
		}
	}

	bool TextEditor::Filter(std::string_view msText)
	{
		if (msText.empty() ||
			msText.find(".ogg") != std::string::npos || msText.find(".wav") != std::string::npos ||
			msText.find(".mv2") != std::string::npos || msText.find(".pb3") != std::string::npos ||
			msText.find(".pb2") != std::string::npos || msText.find(".ps3") != std::string::npos ||
			msText.find(".ps2") != std::string::npos || msText.find(".cur") != std::string::npos ||
			msText.find(".cmv") != std::string::npos || msText.find(".mgv") != std::string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	uint8_t* TextEditor::GetPS3Ptr() const
	{
		return m_amPS3.GetPtr();
	}

	PS3_HDR* TextEditor::GetHdrPtr()
	{
		return (PS3_HDR*)this->GetPS3Ptr();
	}

	uint32_t TextEditor::GetHdrSize()
	{
		return this->GetHdrPtr()->uiHeaderSize;
	}

	uint32_t TextEditor::GetCodeSegSize()
	{
		return this->GetHdrPtr()->uiCodeBlockSize + (4 * this->GeTextCount()) + this->GetHdrPtr()->uiUnBlockSize;
	}

	uint32_t TextEditor::GeTextSegSize()
	{
		return this->GetHdrPtr()->uiTextBlockSize;
	}

	uint8_t* TextEditor::GetCodeSegPtr()
	{
		return this->GetPS3Ptr() + this->GetHdrSize();
	}

	uint8_t* TextEditor::GetTextSegPtr()
	{
		return this->GetPS3Ptr() + this->GetHdrSize() + this->GetCodeSegSize();
	}

	uint32_t TextEditor::GeTextCount()
	{
		return this->GetHdrPtr()->uiTextCount;
	}
}