#pragma once
#include <vector>

#include "CMVS_Types.h"
#include <Rut/RxMem.h>


namespace CMVS::PS3
{
	class TextEditor
	{
	private:
		struct Text_Entry
		{
			std::string msText;
			uint32_t uiRvaValRva{}; // A Value   [Text Data Rva] In Block
			uint32_t uiRvaPtrRva{}; // A Pointer [Text Data Rva’s Rva] In Code Block
		};

	public:
		Rut::RxMem::Auto m_amPS3;
		std::wstring m_wsPath;
		std::vector<Text_Entry> m_vecTextIndex;

		TextEditor() {};

		void Init(std::wstring_view wsPath);
		bool Import(std::wstring_view wsJsonPath, std::wstring_view wsSavePath, size_t nCodePage);
		void Export(std::wstring_view wsPath, size_t nCodePage);

		void ReadEntry();
		bool Filter(std::string_view msText);

	private:
		uint8_t* GetPS3Ptr() const;
		PS3_HDR* GetHdrPtr();
		uint32_t GetHdrSize();
		uint32_t GetCodeSegSize();
		uint32_t GeTextSegSize();
		uint8_t* GetCodeSegPtr();
		uint8_t* GetTextSegPtr();
		uint32_t GeTextCount();
	};
}