#pragma once
#include <sstream>
#include <cstdint>
#include <functional>

#include "Basic.h"


namespace Rut
{
	enum RFM
	{
		RFM_ANSI,
		RFM_UTF8,
		RFM_UTF16
	};
}

namespace Rut::RxFS
{
	class Text : private Basic
	{
	private:
		RFM m_rxFormat;

	public:
		Text(): m_rxFormat(RFM::RFM_ANSI)
		{

		}

		template <typename T_STR> Text(T_STR PATH, RIO emAccess, RFM emFormat, RCO emCreate = RCO::RCO_AUTO) : m_rxFormat(emFormat)
		{
			this->Create(PATH, emAccess, emCreate);
			EnsureBOM(emAccess);
		}

		Text& operator <<(const char* cpStr) { WriteLine(cpStr); return *this; }
		Text& operator <<(const wchar_t* wpStr) { WriteLine(wpStr); return *this; }
		Text& operator <<(std::string_view msStr) { WriteLine(msStr.data(), msStr.size()); return *this; }
		Text& operator <<(std::wstring_view wsStr) { WriteLine(wsStr.data(), wsStr.size()); return *this; }

		void WriteBOM();
		void CheckBOM();
		void EnsureBOM(RIO emAccess);

		size_t WriteLine(const char* cpStr);
		size_t WriteLine(const wchar_t* cpStr);
		size_t WriteLine(std::string_view msStr);
		size_t WriteLine(std::wstring_view wsStr);
		size_t WriteLine(const char* cpStr, size_t nChar);
		size_t WriteLine(const wchar_t* cpStr, size_t nChar);

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

		void Rewind();
	};
}