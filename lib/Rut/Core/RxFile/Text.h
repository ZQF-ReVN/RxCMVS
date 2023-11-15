#pragma once
#include "Binary.h"

#include <sstream>
#include <cstdint>
#include <functional>


constexpr size_t RFM_ANSI = 0x0;
constexpr size_t RFM_UTF8 = 0x1;
constexpr size_t RFM_UTF16 = 0x2;


namespace Rut::RxFile
{
	class Text : private Binary
	{
	private:
		size_t m_rxFormat;

	public:
		Text();
		Text(std::string_view msPath, size_t nMode, size_t nFormat);
		Text(std::wstring_view wsPath, size_t nMode, size_t nFormat);

		Text& operator <<(std::string_view msStr) { WriteLine(msStr); return *this; }
		Text& operator <<(std::wstring_view wsStr) { WriteLine(wsStr); return *this; }

		void WriteBOM();
		void CheckBOM();
		void EnsureBom(size_t nMode);

		size_t WriteLine(std::string_view msStr);
		size_t WriteLine(std::wstring_view wsStr);

		void WriteAllLine(std::vector<std::wstring>& vecLine);
		void WriteAllLine(std::vector<std::string>& vecLine);

		void ReadAllLine(std::vector<std::string>& vecLine);
		void ReadAllLine(std::vector<std::wstring>& vecLine);
		void ReadAllLine(std::function<bool(char*, char*)> fnPerline);
		void ReadAllLine(std::function<bool(wchar_t*, wchar_t*)> fnPerline);

		void ReadRawText(std::string& msText);
		void ReadRawText(std::wstring& wsText);

		void ReadToSStream(std::wstringstream& rfSStream);
		void ReadToSStream(std::stringstream& rfSStream);

		void Flush();
		void Close();

		void Rewind();
	};
}