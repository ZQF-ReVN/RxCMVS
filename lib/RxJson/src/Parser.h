#pragma once
#include "Value.h"


namespace Rut::RxJson
{
	using namespace Rut;

	class Parser
	{
	private:
		std::wstring m_wsJson;
		std::wstring::iterator m_iteChar;

	public:
		Parser();

		wchar_t SkipWhite();
		wchar_t GetToken();

		void ParseArray(Value& rfJValue);
		void ParseObject(Value& rfJValue);
		void ParseNumber(Value& rfJValue);
		void ParseString(Value& rfJValue);
		void ParseValue(Value& rfJValue);
		void ParseKey(std::wstring& wsKey);

		void Open(std::wstring_view wsJson);
		bool Read(Value& rfJValue);
		static void Save(Value& rfJVaue, std::wstring_view wsFileName);
	};
}