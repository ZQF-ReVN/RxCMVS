#pragma once
#include "Value.h"


namespace Rut::RxJson
{
	using namespace Rut;

	class Parser
	{
	private:
		wchar_t* m_wpJson;
		size_t m_nJsonCCH;
		size_t m_nReadCCH;

	public:
		Parser();
		~Parser();

	private:
		wchar_t SkipWhite();
		wchar_t GetToken();

		void AddReadCCH(size_t nCount = 1);
		wchar_t GetCurChar();
		wchar_t* GetCurPtr();
		size_t GeReadCCH();
		size_t GetJsonCCH();

		void ParseArray(Value& rfJValue);
		void ParseObject(Value& rfJValue);
		void ParseNumber(Value& rfJValue);
		void ParseString(Value& rfJValue);
		void ParseTrue(Value& rfJValue);
		void ParseFalse(Value& rfJValue);
		void ParseNull(Value& rfJValue);

		void ParseValue(Value& rfJValue);

	public:
		void Open(std::wstring_view wsJson);
		bool Read(Value& rfJValue);
		static void Save(Value& rfJVaue, std::wstring_view wsFileName);
	};
}