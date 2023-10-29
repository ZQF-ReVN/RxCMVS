#include "Parser.h"
#include "../../Rut/RxFile.h"

#include <assert.h>


namespace Rut::RxJson
{
	Parser::Parser()
	{
		m_wpJson = nullptr;
		m_nJsonCCH = 0;
		m_nReadCCH = 0;
	}

	Parser::~Parser()
	{
		if (m_wpJson)
		{
			delete[] m_wpJson;
		}
	}


	void Parser::AddReadCCH(size_t nCount)
	{
		this->m_nReadCCH += nCount;
	}

	wchar_t Parser::GetCurChar()
	{
		return m_wpJson[m_nReadCCH];
	}

	wchar_t* Parser::GetCurPtr()
	{
		return m_wpJson + m_nReadCCH;
	}

	size_t Parser::GeReadCCH()
	{
		return m_nReadCCH;
	}

	size_t Parser::GetJsonCCH()
	{
		return this->m_nJsonCCH;
	}


	wchar_t Parser::SkipWhite()
	{
		while (true)
		{
			wchar_t tmp_char = this->GetCurChar();
			switch (tmp_char)
			{
			case L'\t': case L'\n':case L'\r':
			case L' ': this->AddReadCCH(); continue;
			}
			return tmp_char;
		}
	}

	wchar_t Parser::GetToken()
	{
		wchar_t tmp_char = this->SkipWhite();

		switch (tmp_char)
		{
		case L'{':case L'}':
		case L'[':case L']':
		case L'"':case L':':
		case L'n':case L't':case L'f':
		case L'0':case L'1':case L'2':
		case L'3':case L'4':case L'5':
		case L'6':case L'7':case L'8':
		case L'9':case L'-':return tmp_char;
		case L',':this->AddReadCCH(); return this->GetToken();
		default: throw std::runtime_error("Not Find Token");
		}
	}


	void Parser::ParseArray(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'[');

		this->AddReadCCH();

		while (this->GetToken() != L']')
		{
			Value value;
			this->ParseValue(value);
			rfJValue.Append(std::move(value));
		}

		this->AddReadCCH();

		rfJValue.SureArray();
		return;
	}

	void Parser::ParseObject(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'{');

		this->AddReadCCH();

		std::wstring_view key;
		while (true)
		{
			switch (this->GetToken())
			{
			case L'"': // parse key
			{
				this->AddReadCCH();

				wchar_t* beg = this->GetCurPtr();
				wchar_t* end = ::wcschr(beg, L'"');
				key = { beg, end };

				this->AddReadCCH((end - beg) + 1);
			}
			break;

			case L':': // check key : value
			{
				this->AddReadCCH();
				this->SkipWhite();

				Value value;
				this->ParseValue(value);
				rfJValue.AddKey(key, std::move(value));
			}
			break;

			case L'}': // end of object
			{
				this->AddReadCCH();
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
		wchar_t* beg = this->GetCurPtr();
		double num_org = ::wcstod(beg, &end);

		this->AddReadCCH(end - beg);

		int num_int = (int)(num_org);
		double num_loss = (double)(num_int);

		num_org == num_loss ? (rfJValue = (int)num_int) : (rfJValue = num_org);
	}

	void Parser::ParseString(Value& rfJValue)
	{
		auto fn_make_unicode = [](const wchar_t* wpStr) -> uint32_t
			{
				auto fn_char2num = [](wchar_t cChar) -> uint8_t
					{
						if ((uint16_t)cChar >= 0x41) // A-Z
						{
							return (uint8_t)(cChar - 0x37);
						}
						else // 0-9
						{
							return (uint8_t)(cChar - 0x30);
						}
					};

				uint8_t hex_0 = fn_char2num(wpStr[0]);
				uint8_t hex_1 = fn_char2num(wpStr[1]);
				uint8_t hex_2 = fn_char2num(wpStr[2]);
				uint8_t hex_3 = fn_char2num(wpStr[3]);

				return (hex_0 << 12) | (hex_1 << 8) | (hex_2 << 4) | (hex_3 << 0);
			};

		assert(this->GetCurChar() == '"');

		this->AddReadCCH();

		std::wstring text;
		while (true)
		{
			wchar_t ch = this->GetCurChar();

			if (ch == L'\\') // control characters
			{
				this->AddReadCCH();
				wchar_t ctrl_char = this->GetCurChar();

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
					this->AddReadCCH();
					ctrl_char = (wchar_t)fn_make_unicode(this->GetCurPtr());
					this->AddReadCCH(3);
				}
				break;

				default: throw std::runtime_error("Unknow Control Characters");
				}

				ch = ctrl_char;
			}
			else if (ch == L'"') // end
			{
				this->AddReadCCH(); // skip " char
				break;
			}

			text.append(1, ch);
			this->AddReadCCH(); // netx char
		}

		Value value(std::move(text));
		rfJValue = std::move(value);
	}

	void Parser::ParseTrue(Value& rfJValue)
	{
		assert(this->GetCurChar() == L't');

		this->AddReadCCH(4);
		rfJValue = true;
	}

	void Parser::ParseFalse(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'f');

		this->AddReadCCH(5);
		rfJValue = false;
	}

	void Parser::ParseNull(Value& rfJValue)
	{
		assert(this->GetCurChar() == L'n');

		this->AddReadCCH(4);
		rfJValue = Value();
	}

	void Parser::ParseValue(Value& rfJValue)
	{
		switch (this->GetCurChar())
		{
		case L'{': ParseObject(rfJValue); break; // object
		case L'[': ParseArray(rfJValue);  break; // array
		case L't': ParseTrue(rfJValue);   break; // true
		case L'f': ParseFalse(rfJValue);  break; // false
		case L'n': ParseNull(rfJValue);   break; // null
		case L'"': ParseString(rfJValue); break; // string
		case L'0':case L'1':case L'2':case L'3':
		case L'4':case L'5':case L'6':case L'7':
		case L'8':case L'9':
		case L'-': ParseNumber(rfJValue); break; // Number
		default: throw std::runtime_error("Json Parse Value Error!");
		}
	}


	void Parser::Open(std::wstring_view wsJson)
	{
		std::wstring json;
		RxFile::Text{ wsJson, RIO_READ, RFM_UTF8 }.ReadRawText(json);

		this->m_nJsonCCH = json.size();
		this->m_wpJson = new wchar_t[m_nJsonCCH + 1];
		wcsncpy_s(this->m_wpJson, m_nJsonCCH + 1, json.data(), this->m_nJsonCCH);
		this->m_wpJson[this->m_nJsonCCH] = L'\0';
	}

	bool Parser::Read(Value& rfJValue)
	{
		this->ParseValue(rfJValue);

		this->SkipWhite();
		return (this->GeReadCCH() >= this->GetJsonCCH()) ? (true) : (false);
	}

	void Parser::Save(Value& rfJVaue, std::wstring_view wsFileName)
	{
		std::wstring text;
		rfJVaue.Dump(text);
		RxFile::Text{ wsFileName, RIO_WRITE, RFM_UTF8 }.WriteLine(text);
	}
}