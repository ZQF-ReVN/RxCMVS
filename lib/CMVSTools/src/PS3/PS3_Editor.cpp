#include "PS3_Editor.h"
#include "../../../Rut/RxFile.h"
#include "../../../Rut/RxStr.h"
#include "../../../RxJson/RxJson.h"

using namespace Rut;


namespace CMVS::PS3
{
	void Editor::Init(std::wstring_view wsPath, bool isDecode)
	{
		m_wsPath = wsPath;
		isDecode ? ((void)Coder::Decode(wsPath, m_amPS3)) : ((void)m_amPS3.LoadFile(wsPath));
		ReadEntry();
	}

	bool Editor::Insert(std::wstring_view wsJsonPath, std::wstring_view wsSavePath, size_t nCodePage)
	{
		// Check
		if (this->m_vecTextIndex.empty()) { return false; }

		// Read Json
		RxJson::Value json;
		RxJson::Parser parser;
		parser.Open(wsJsonPath);
		parser.Read(json);
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
			entry.msText = RxStr::ToMBCS(jarr_scn[ite][L"Text_Tra"], nCodePage);

			if (entry.msText.empty()) { throw std::runtime_error("CMVS::PS3::Editor::Insert Error! Text Empty"); }

			// Cal New Text Rva
			entry.uiRvaValRva = text_seg_len + append_text_size;
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
		hdr_ptr->uiDataDecompressSize += append_text_size;
		hdr_ptr->uiTextBlockSize += append_text_size;

		// Save Data
		this->m_amPS3.SaveData(wsSavePath);

		return true;
	}

	void Editor::Extract(std::wstring_view wsPath, size_t nCodePage)
	{
		auto fn_ToHexStr = [](uint32_t uiValue) -> std::wstring
			{
				wchar_t buf[25] = { 0 };
				RxStr::Sprintf(buf, 25, L"0x%08X", uiValue);
				return buf;
			};

		size_t hdr_len = this->GetHdrSize();
		size_t code_len = this->GetCodeSegSize();

		RxJson::Value json_root;
		RxJson::JArray& jarr_scn = json_root[L"Scenario"];
		for (auto& entry : m_vecTextIndex)
		{
			std::wstring text;
			RxStr::ToWCS(entry.msText, text, nCodePage);

			RxJson::JObject jobj_info;
			jobj_info[L"Text_Raw"] = text;
			jobj_info[L"Text_Tra"] = text;
			jobj_info[L"FOA_Ptr"] = fn_ToHexStr(hdr_len + entry.uiRvaPtrRva);
			jobj_info[L"FOA_Rva"] = fn_ToHexStr(hdr_len + code_len + entry.uiRvaValRva);

			jarr_scn.push_back(jobj_info);
		}

		RxJson::Parser::Save(json_root, wsPath);
	}


	void Editor::ReadEntry()
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

			entry.uiRvaPtrRva = ite + sizeof(push_str_opcode_beg);
			entry.uiRvaValRva = *(uint32_t*)(code_seg_ptr + entry.uiRvaPtrRva);

			// Filter String
			std::string_view ms_text = (char*)this->GetTextSegPtr() + entry.uiRvaValRva;
			if (this->Filter(ms_text)) { continue; }

			entry.msText = ms_text;
			m_vecTextIndex.emplace_back(entry);
		}
	}

	bool Editor::Filter(std::string_view msText)
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

	uint8_t* Editor::GetPtr()
	{
		return m_amPS3.GetPtr();
	}

	PS3_HDR* Editor::GetHdrPtr()
	{
		return (PS3_HDR*)this->GetPtr();
	}

	uint32_t Editor::GetHdrSize()
	{
		return this->GetHdrPtr()->uiHeaderSize;
	}

	uint32_t Editor::GetCodeSegSize()
	{
		return this->GetHdrPtr()->uiCodeBlockSize + (4 * this->GeTextCount()) + this->GetHdrPtr()->uiUnBlockSize;
	}

	uint32_t Editor::GeTextSegSize()
	{
		return this->GetHdrPtr()->uiTextBlockSize;
	}

	uint8_t* Editor::GetCodeSegPtr()
	{
		return this->GetPtr() + this->GetHdrSize();
	}

	uint8_t* Editor::GetTextSegPtr()
	{
		return this->GetPtr() + this->GetHdrSize() + this->GetCodeSegSize();
	}

	uint32_t Editor::GeTextCount()
	{
		return this->GetHdrPtr()->uiTextCount;
	}
}