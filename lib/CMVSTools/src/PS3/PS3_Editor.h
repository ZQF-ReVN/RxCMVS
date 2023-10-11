#pragma once
#include <vector>

#include "PS3_Coder.h"
#include "PS3_Struct.h"


namespace CMVS::PS3
{
	class Editor
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

		Editor() {};

		void Init(std::wstring_view wsPath, bool isDecode = false);
		bool Insert(std::wstring_view wsJsonPath, std::wstring_view wsSavePath, size_t nCodePage);
		void Extract(std::wstring_view wsPath, size_t nCodePage);

		void ReadEntry();
		bool Filter(std::string_view msText);

	private:
		uint8_t* GetPtr();
		PS3_HDR* GetHdrPtr();
		uint32_t GetHdrSize();
		uint32_t GetCodeSegSize();
		uint32_t GeTextSegSize();
		uint8_t* GetCodeSegPtr();
		uint8_t* GetTextSegPtr();
		uint32_t GeTextCount();
	};
}