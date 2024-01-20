#include "RxFile.h"
#include "RxPath.h"
#include "RxStr.h"
#include "RxSys.h"

#include <stdexcept>



namespace Rut::RxFile::Stream
{
	BasicStream::BasicStream() : m_hFile(nullptr)
	{

	}

	BasicStream::BasicStream(BasicStream&& refStream) noexcept
	{
		this->m_hFile = refStream.m_hFile;
		refStream.m_hFile = nullptr;
	}

	BasicStream::~BasicStream()
	{
		this->Close();
	}

	void BasicStream::Create(const std::filesystem::path& phPath, size_t nMode)
	{
		m_hFile = RxSys::FileOpen(phPath, nMode);
		if (m_hFile == nullptr) { throw std::runtime_error("Create File Error!"); }
	}


	bool BasicStream::Close()
	{
		bool is_close = RxSys::FileClose(m_hFile);
		m_hFile = nullptr;
		return is_close;
	}

	bool BasicStream::Flush()
	{
		return RxSys::FileFlush(m_hFile);
	}

	bool BasicStream::IsEnd()
	{
		size_t file_size = GetSize();
		size_t current_pointer = GetPos();
		return current_pointer >= file_size;
	}

	size_t BasicStream::SetPos(size_t nOffset, size_t nWay)
	{
		return RxSys::FileSetPtr(m_hFile, nOffset, nWay);
	}

	size_t BasicStream::GetPos()
	{
		return RxSys::FileGetPtr(m_hFile);
	}

	size_t BasicStream::GetSize()
	{
		return RxSys::FileGetSize(m_hFile);
	}

	size_t BasicStream::Read(void* pBuffer, size_t nSize)
	{
		return RxSys::FileRead(m_hFile, pBuffer, nSize);
	}

	size_t BasicStream::Write(const void* pData, size_t nSize)
	{
		return RxSys::FileWrite(m_hFile, pData, nSize);
	}
}

namespace Rut::RxFile::Stream
{
	BinaryStream::BinaryStream()
	{

	}

	BinaryStream::BinaryStream(const std::filesystem::path& phPath, size_t nMode)
	{
		this->Create(phPath, nMode);
	}
}

namespace Rut::RxFile::Stream
{
	TextStream::TextStream() : m_rxFormat(RFM_ANSI)
	{

	}

	TextStream::TextStream(const std::filesystem::path& phPath, size_t nMode, size_t nFormat) : m_rxFormat(nFormat)
	{
		this->Create(phPath, nMode);
		this->EnsureBom(nMode);
	}

	void TextStream::WriteBOM()
	{
		switch (m_rxFormat)
		{
		case RFM_UTF8: Write((void*)"\xEF\xBB\xBF", 3); break;
		case RFM_UTF16: Write((void*)"\xFF\xFE", 2); break;
		}
	}

	void TextStream::CheckBOM()
	{
		uint32_t bom = 0, bom_size = 0;
		this->Read(&bom, sizeof(bom));

		switch (m_rxFormat)
		{
		case RFM_UTF8: { if ((bom & 0x00FFFFFFU) == 0x00BFBBEFU) { bom_size = 3; }}break;
		case RFM_UTF16: { if ((bom & 0x0000FFFFU) == 0x0000FEFF) { bom_size = 2; }}break;
		}

		this->SetPos(bom_size, RIO_BEGIN);
	}

	void TextStream::EnsureBom(size_t nMode)
	{
		if (this->GetSize())
		{
			if (nMode & RIO_DATA_IN) { this->CheckBOM(); }
		}
		else
		{
			if (nMode & RIO_DATA_OUT) { this->WriteBOM(); }
		}
	}

	size_t TextStream::WriteLine(std::wstring_view wsStr)
	{
		switch (m_rxFormat)
		{
		case RFM_ANSI:
		case RFM_UTF8:
		{
			std::string mbcs;
			uint32_t code_page = 0;
			if (m_rxFormat == RFM_UTF8) { code_page = 65001; }
			RxStr::ToMBCS(wsStr, mbcs, code_page);
			return Write(mbcs.data(), mbcs.size());
		}
		break;

		case RFM_UTF16:
		{
			return Write((wchar_t*)wsStr.data(), wsStr.size() * sizeof(wchar_t));
		}
		break;
		}

		return 0;
	}

	size_t TextStream::WriteLine(std::string_view msStr)
	{
		switch (m_rxFormat)
		{
		case RFM_ANSI:
		{
			return Write((char*)msStr.data(), msStr.size());
		}
		break;

		case RFM_UTF8:
		{
			std::string u8str;
			std::wstring u16str;
			RxStr::ToWCS(msStr, u16str, 0);
			RxStr::ToMBCS(u16str, u8str, 65001);
			return Write(u8str.data(), u8str.size());
		}
		break;

		case RFM_UTF16:
		{
			std::wstring wide;
			RxStr::ToWCS(msStr, wide, 0);
			return Write(wide.data(), (wide.size() * 2));
		}
		break;
		}

		return 0;
	}


	void TextStream::WriteAllLine(std::vector<std::string>& vecLine)
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

	void TextStream::WriteAllLine(std::vector<std::wstring>& vecLine)
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


	void TextStream::ReadAllLine(std::vector<std::string>& vecLine)
	{
		ReadAllLine([&vecLine](char* wpBeg, char* wpEnd) -> bool { vecLine.emplace_back(wpBeg, wpEnd); return true; });
	}

	void TextStream::ReadAllLine(std::vector<std::wstring>& vecLine)
	{
		ReadAllLine([&vecLine](wchar_t* wpBeg, wchar_t* wpEnd) -> bool { vecLine.emplace_back(wpBeg, wpEnd); return true; });
	}

	void TextStream::ReadAllLine(std::function<bool(char*, char*)> fnPerline)
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
		if (line_beg < str_end) { fnPerline(line_beg, str_end); }
	}

	void TextStream::ReadAllLine(std::function<bool(wchar_t*, wchar_t*)> fnPerline)
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
		if (line_beg < str_end) { fnPerline(line_beg, str_end); }
	}


	void TextStream::ReadRawText(std::string& msText)
	{
		size_t bom_size = this->GetPos();
		size_t raw_size = this->GetSize() - bom_size;

		switch (m_rxFormat)
		{
		case RFM_ANSI:
		{
			msText.resize(raw_size);
			this->Read(msText.data(), raw_size);
		}
		break;
		case RFM_UTF8:
		{
			char* buf = new char[raw_size + 1];
			this->Read(buf, raw_size);
			buf[raw_size] = '\0';

			std::wstring wstr;
			RxStr::ToWCS({ buf, raw_size }, wstr, 65001);
			RxStr::ToMBCS(wstr, msText, 65001);
			delete[] buf;
		}
		break;
		case RFM_UTF16:
		{
			size_t cch = raw_size / sizeof(wchar_t);
			wchar_t* buf = new wchar_t[cch + 1];
			this->Read(buf, raw_size);
			buf[cch] = '\0';

			RxStr::ToMBCS({ buf, cch }, msText, 0);
			delete[] buf;
		}
		break;
		}
	}

	void TextStream::ReadRawText(std::wstring& wsText)
	{
		size_t bom_size = this->GetPos();
		size_t raw_size = this->GetSize() - bom_size;

		switch (m_rxFormat)
		{
		case RFM_ANSI:
		case RFM_UTF8:
		{
			char* buf = new char[raw_size + 1];
			this->Read(buf, raw_size);
			buf[raw_size] = '\0';

			uint32_t code_page = 0;
			if (m_rxFormat == RFM_UTF8) { code_page = 65001; }
			RxStr::ToWCS({ buf, raw_size }, wsText, code_page);
			delete[] buf;
		}
		break;
		case RFM_UTF16:
		{
			wsText.resize(raw_size / sizeof(wchar_t));
			this->Read(wsText.data(), raw_size);
		}
		break;
		}
	}


	void TextStream::ReadToSStream(std::wstringstream& rfSStream)
	{
		std::wstring text;
		ReadRawText(text);
		rfSStream.str(std::move(text));
	}

	void TextStream::ReadToSStream(std::stringstream& rfSStream)
	{
		std::string text;
		ReadRawText(text);
		rfSStream.str(std::move(text));
	}


	void TextStream::Flush()
	{
		this->BinaryStream::Flush();
	}

	void TextStream::Close()
	{
		this->BinaryStream::Close();
	}

	void TextStream::Rewind()
	{
		this->SetPos(0, RIO_BEGIN);
	}
}

namespace Rut::RxFile
{
	void SaveFileViaPath(const std::filesystem::path& phPath, void* pData, size_t nBytes)
	{
		phPath.has_parent_path() ? (std::filesystem::create_directories(phPath.parent_path())) : (false);
		RxFile::Binary{ phPath, RIO_WRITE }.Write(pData, nBytes);
	}
}