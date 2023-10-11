#include "Text.h"
#include "../RxCvt/Cvt.h"


namespace Rut::RxFS
{
	void Text::WriteBOM()
	{
		switch (m_rxFormat)
		{
		case RFM::RFM_UTF8:
			Write((char*)"\xEF\xBB\xBF", 3);
			break;
		case RFM::RFM_UTF16:
			Write((void*)"\xFF\xFE", 2);
			break;
		}
	}

	void Text::CheckBOM()
	{
		uint32_t bom = 0; this->Read(&bom, sizeof(bom));

		switch (m_rxFormat)
		{
		case RFM::RFM_UTF8:
		{
			if ((bom & 0x00FFFFFFU) == 0x00BFBBEFU) { this->MovePos(-1); return; } // Skip BOM
		}
		break;

		case RFM::RFM_UTF16:
		{
			if ((bom & 0x0000FFFFU) == 0x0000FEFF) { this->MovePos(-2); return; } // Skip BOM
		}
		break;
		}

		this->MovePos(-4); // Not BOM Back Pointer
	}

	void Text::EnsureBOM(RIO emAccess)
	{
		switch (emAccess)
		{
		case RIO::RIO_IN:
			CheckBOM();
			break;
		case RIO::RIO_OUT:
			WriteBOM();
			break;
		case RIO::RIO_IN_OUT:
			CheckBOM();
			break;
		}
	}


	size_t Text::WriteLine(const char* cpStr)
	{
		size_t char_count = strlen(cpStr);
		return WriteLine((char*)cpStr, char_count);
	}

	size_t Text::WriteLine(const wchar_t* cpStr)
	{
		size_t wchar_len = wcslen(cpStr);
		return WriteLine(cpStr, wchar_len);
	}

	size_t Text::WriteLine(std::string_view msStr)
	{
		return WriteLine(msStr.data(), msStr.size());
	}

	size_t Text::WriteLine(std::wstring_view wsStr)
	{
		return WriteLine(wsStr.data(), wsStr.size());
	}

	size_t Text::WriteLine(const wchar_t* cpStr, size_t nChar)
	{
		switch (m_rxFormat)
		{
		case RFM::RFM_ANSI:
		case RFM::RFM_UTF8:
		{
			std::string mbcs;
			uint32_t code_page = CP_ACP;
			if (m_rxFormat == RFM_UTF8) { code_page = CP_UTF8; }
			RxCvt::ToMBCS(cpStr, mbcs, code_page);
			return Write(mbcs.data(), mbcs.size());
		}
		break;

		case RFM::RFM_UTF16:
		{
			return Write((wchar_t*)cpStr, nChar * 2);
		}
		break;
		}

		return 0;
	}

	size_t Text::WriteLine(const char* cpStr, size_t nChar)
	{
		switch (m_rxFormat)
		{
		case RFM::RFM_ANSI:
		{
			return Write((char*)cpStr, nChar);
		}
		break;

		case RFM::RFM_UTF8:
		{
			std::string u8str;
			std::wstring u16str;
			RxCvt::ToWCS(cpStr, u16str, CP_ACP);
			RxCvt::ToMBCS(u16str, u8str, CP_UTF8);
			return Write(u8str.data(), u8str.size());
		}
		break;

		case RFM::RFM_UTF16:
		{
			std::wstring wide;
			RxCvt::ToWCS(cpStr, wide, CP_ACP);
			return Write(wide.data(), (wide.size() * 2));
		}
		break;
		}

		return 0;
	}


	void Text::WriteAllLine(std::vector<std::string>& vecLine)
	{
		std::string text;
		for (auto& line : vecLine)
		{
			text.append(line);
			text.append(1, '\n');
		}
		text.pop_back();
		this->WriteLine(text);
		this->Flush();
	}

	void Text::WriteAllLine(std::vector<std::wstring>& vecLine)
	{
		std::wstring text;
		for (auto& line : vecLine)
		{
			text.append(line);
			text.append(1, L'\n');
		}
		text.pop_back();
		this->WriteLine(text);
		this->Flush();
	}


	void Text::ReadAllLine(std::vector<std::string>& vecLine)
	{
		ReadAllLine([&vecLine](char* wpBeg, char* wpEnd) -> bool { vecLine.emplace_back(wpBeg, wpEnd); return true; });
	}

	void Text::ReadAllLine(std::vector<std::wstring>& vecLine)
	{
		ReadAllLine([&vecLine](wchar_t* wpBeg, wchar_t* wpEnd) -> bool { vecLine.emplace_back(wpBeg, wpEnd); return true; });
	}

	void Text::ReadAllLine(std::function<bool(char*, char*)> fnPerline)
	{
		std::string text;
		ReadRawText(text);

		char* search_end = nullptr;
		char* line_beg = text.data();
		while (search_end = strchr(line_beg, '\n'))
		{
			char* line_end = search_end;
			if (search_end[-1] == '\r') { line_end -= 1; }
			if (!fnPerline(line_beg, line_end)) { return; }
			line_beg = search_end + 1;
		}

		char* str_end = (char*)(text.end()._Ptr);
		if (line_beg < str_end)
		{
			fnPerline(line_beg, str_end);
		}
	}

	void Text::ReadAllLine(std::function<bool(wchar_t*, wchar_t*)> fnPerline)
	{
		std::wstring text;
		ReadRawText(text);

		wchar_t* search_end = nullptr;
		wchar_t* line_beg = text.data();
		while (search_end = wcschr(line_beg, L'\n'))
		{
			wchar_t* line_end = search_end;
			if (search_end[-1] == L'\r') { line_end -= 1; }
			if (!fnPerline(line_beg, line_end)) { return; }
			line_beg = search_end + 1;
		}

		wchar_t* str_end = (wchar_t*)(text.end()._Ptr);
		if (line_beg < str_end)
		{
			fnPerline(line_beg, str_end);
		}
	}


	void Text::ReadRawText(std::string& msText)
	{
		size_t bom = this->GetPos();
		size_t size = this->GetSize() - bom;

		switch (m_rxFormat)
		{
		case RFM_ANSI:
		{
			msText.resize(size);
			this->Read(msText.data(), size);
		}
		break;
		case RFM_UTF8:
		{
			char* buf = new char[size + 1];
			this->Read(buf, size);
			buf[size] = '\0';

			std::wstring wstr;
			RxCvt::ToWCS({ buf, size }, wstr, CP_UTF8);
			RxCvt::ToMBCS(wstr, msText, CP_UTF8);
			delete[] buf;
		}
		break;
		case RFM_UTF16:
		{
			size_t cch = size / sizeof(wchar_t);
			wchar_t* buf = new wchar_t[cch + 1];
			this->Read(buf, size);
			buf[cch] = '\0';

			RxCvt::ToMBCS({ buf, cch }, msText, CP_ACP);
			delete[] buf;
		}
		break;
		}
	}

	void Text::ReadRawText(std::wstring& wsText)
	{
		size_t bom = this->GetPos();
		size_t size = this->GetSize() - bom;

		switch (m_rxFormat)
		{
		case RFM_ANSI:
		case RFM_UTF8:
		{
			char* buf = new char[size + 1];
			this->Read(buf, size);
			buf[size] = '\0';

			uint32_t code_page = CP_ACP;
			if (m_rxFormat == RFM_UTF8) { code_page = CP_UTF8; }
			RxCvt::ToWCS({ buf, size }, wsText, code_page);
			delete[] buf;
		}
		break;
		case RFM_UTF16:
		{
			wsText.resize(size / sizeof(wchar_t));
			this->Read(wsText.data(), size);
		}
		break;
		}
	}


	void Text::ReadToSStream(std::wstringstream& rfSStream)
	{
		std::wstring text;
		ReadRawText(text);
		rfSStream.str(std::move(text));
	}

	void Text::ReadToSStream(std::stringstream& rfSStream)
	{
		std::string text;
		ReadRawText(text);
		rfSStream.str(std::move(text));
	}


	void Text::Rewind()
	{
		this->SetPos(0);
	}

}