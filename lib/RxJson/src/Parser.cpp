#include "Parser.h"
#include "../../Rut/RxFS.h"


namespace Rut::RxJson
{
	Parser::Parser()
	{

	}

	wchar_t Parser::SkipWhite()
	{
		while (true)
		{
			wchar_t tmp_char = m_iteChar[0];
			switch (tmp_char)
			{
			case L' ':
			case L'\t':
			case L'\n':
			case L'\r':
				m_iteChar++;
				continue;
			}
			return tmp_char;
		}
	}

	wchar_t Parser::GetToken()
	{
		wchar_t tmp_char = SkipWhite();

		switch (tmp_char)
		{
		case L'{':case L'}':
		case L'[':case L']':
		case L'"':case L':':case L',':
		case L'n':case L't':case L'f':
		case L'0':case L'1':case L'2':case L'3':
		case L'4':case L'5':case L'6':case L'7':
		case L'8':case L'9':case L'-':return tmp_char;
		}

		throw std::runtime_error("Not Find Token");
	}


	void Parser::ParseArray(Value& rfJValue)
	{
		m_iteChar++;

		while (true)
		{
			switch (GetToken())
			{
			case L',': // end of element
			{
				m_iteChar++;
			}
			break;

			case L']': // end of array
			{
				m_iteChar++;
				rfJValue.SureArray();
				return;
			}
			break;

			default: // parse value
			{
				Value value;
				ParseValue(value);
				rfJValue.Append(std::move(value));
			}
			}
		}
	}

	void Parser::ParseObject(Value& rfJValue)
	{
		m_iteChar++;

		std::wstring key;
		while (true)
		{
			switch (GetToken())
			{
			case L'"': // check if key
			{
				m_iteChar++;
				ParseKey(key);
			}
			break;

			case L':': // check key : value
			{
				m_iteChar++;
				SkipWhite();
				Value value;
				ParseValue(value);
				rfJValue.AddKey(key, std::move(value));
			}
			break;

			case L',': // check end of key / value
			{
				m_iteChar++;
			}
			break;

			case L'}': // end of object
			{
				m_iteChar++;
				rfJValue.SureObject();
				return;
			}
			break;

			default: { throw std::runtime_error("ParseObject Error!"); }
			}
		}
	}

	void Parser::ParseNumber(Value& rfJValue)
	{
		wchar_t* end = nullptr;
		double num_org = wcstod(m_iteChar._Ptr, &end);
		m_iteChar._Ptr = end;

		int num_int = (int)(num_org);
		double num_loss = (double)(num_int);

		num_org == num_loss ? (rfJValue = (int)num_int) : (rfJValue = num_org);
	}

	static uint8_t Char2Num(wchar_t cChar)
	{
		if ((uint16_t)cChar >= 0x41) // A-Z
		{
			return (uint8_t)(cChar - 0x37);
		}
		else // 0-9
		{
			return (uint8_t)(cChar - 0x30);
		}
	}

	static uint16_t MakeUnicode(const wchar_t* wpStr)
	{
		uint8_t hex_0 = Char2Num(wpStr[0]);
		uint8_t hex_1 = Char2Num(wpStr[1]);
		uint8_t hex_2 = Char2Num(wpStr[2]);
		uint8_t hex_3 = Char2Num(wpStr[3]);

		return (hex_0 << 12) | (hex_1 << 8) | (hex_2 << 4) | (hex_3 << 0);
	}

	void Parser::ParseString(Value& rfJValue)
	{
		m_iteChar++;

		wchar_t ch = 0;
		std::wstring text;
		while (true)
		{
			ch = m_iteChar[0];

			if (ch == L'\\') // control characters
			{
				m_iteChar++;
				wchar_t ctrl_char = m_iteChar[0];

				switch (ctrl_char)
				{
				case L'\\': ctrl_char = L'\\'; break;
				case L'"': ctrl_char = L'\"'; break;
				case L'b': ctrl_char = L'\b'; break;
				case L'f': ctrl_char = L'\f'; break;
				case L'n': ctrl_char = L'\n'; break;
				case L'r': ctrl_char = L'\r'; break;
				case L't': ctrl_char = L'\t'; break;
				case L'u':
				{
					m_iteChar++;
					ctrl_char = (wchar_t)MakeUnicode(m_iteChar._Ptr);
					m_iteChar += 3;
				}
				break;

				default: throw std::runtime_error("Unknow Control Characters");
				}

				ch = ctrl_char;
			}
			else if (ch == L'"') // end
			{
				m_iteChar++; // skip " char
				break;
			}

			text.append(1, ch);
			m_iteChar++; // netx char
		}

		Value value(std::move(text));
		rfJValue = std::move(value);
	}

	void Parser::ParseValue(Value& rfJValue)
	{
		switch (m_iteChar[0])
		{
		case L'{': ParseObject(rfJValue); break; // object
		case L'[': ParseArray(rfJValue); break; // array
		case L't': { m_iteChar += 4; rfJValue = true; } break; // true
		case L'f': { m_iteChar += 5; rfJValue = false; }break; // false
		case L'n': { m_iteChar += 4; rfJValue = Value(); }break; // null
		case L'"': ParseString(rfJValue); break; // string
		case L'0':case L'1':case L'2':case L'3':
		case L'4':case L'5':case L'6':case L'7':
		case L'8':case L'9':
		case L'-': ParseNumber(rfJValue); break; // Number
		default: throw std::runtime_error("ParseValue Error!"); // Error!
		}
	}

	void Parser::ParseKey(std::wstring& wsKey)
	{
		std::wstring::iterator beg = m_iteChar;
		std::wstring::iterator end = std::find(beg, m_wsJson.end(), L'"');
		wsKey = { beg, end };
		m_iteChar = end + 1;
	}


	void Parser::Open(std::wstring_view wsJson)
	{
		RxFS::Text{ wsJson,RIO::RIO_IN, RFM::RFM_UTF8 }.ReadRawText(m_wsJson);
		m_iteChar = m_wsJson.begin();
	}

	bool Parser::Read(Value& rfJValue)
	{
		(GetToken() == L'{') ? (this->ParseObject(rfJValue)) : (throw std::runtime_error("Not Find Object"));
		return (m_iteChar == m_wsJson.end()) ? (true) : (false);
	}

	void Parser::Save(Value& rfJVaue, std::wstring_view wsFileName)
	{
		std::wstring text;
		rfJVaue.Dump(text);
		RxFS::Text{ wsFileName, RIO::RIO_OUT, RFM::RFM_UTF8 }.WriteLine(text);
	}
}