#include "RxJson.h"
#include "RxFile.h"

#include <map>
#include <cassert>
#include <stdexcept>


namespace Rut::RxJson
{
	JValue::JValue() : m_Type(JVALUE_NUL)
	{
		m_Value = { 0 };
	}

	JValue::JValue(bool bValue) : m_Type(JVALUE_BOL)
	{
		m_Value.Bool = bValue;
	}

	JValue::JValue(int iValue) : m_Type(JVALUE_INT)
	{
		m_Value.Int = iValue;
	}

	JValue::JValue(double dValue) : m_Type(JVALUE_DBL)
	{
		m_Value.Double = dValue;
	}

	JValue::JValue(const wchar_t* sValue) : m_Type(JVALUE_STR)
	{
		m_Value.StrPtr = new JString(sValue);
	}

	JValue::JValue(std::wstring_view sValue) : m_Type(JVALUE_STR)
	{
		m_Value.StrPtr = new JString(sValue);
	}

	JValue::JValue(const std::wstring& sValue) : m_Type(JVALUE_STR)
	{
		m_Value.StrPtr = new JString(sValue);
	}

	JValue::JValue(std::wstring&& sValue) : m_Type(JVALUE_STR)
	{
		m_Value.StrPtr = new JString(std::move(sValue));
	}

	JValue::JValue(const JArray& aValue) : m_Type(JVALUE_ARY)
	{
		m_Value.AryPtr = new JArray(aValue);
	}

	JValue::JValue(JArray&& aValue) : m_Type(JVALUE_ARY)
	{
		m_Value.AryPtr = new JArray(std::move(aValue));
	}

	JValue::JValue(const JObject& oValue) : m_Type(JVALUE_OBJ)
	{
		m_Value.ObjPtr = new JObject(oValue);
	}

	JValue::JValue(JObject&& oValue) : m_Type(JVALUE_OBJ)
	{
		m_Value.ObjPtr = new JObject(std::move(oValue));
	}

	JValue::JValue(const JValue& rfJValue):m_Type(JVALUE_NUL)
	{
		Copy(rfJValue);
	}

	JValue::JValue(JValue&& rfJValue) noexcept: m_Type(JVALUE_NUL)
	{
		Move(rfJValue);
	}

	JValue::~JValue()
	{
		switch (m_Type)
		{
		case Value_Type_T::JVALUE_STR: { delete m_Value.StrPtr; }break;
		case Value_Type_T::JVALUE_ARY: { delete m_Value.AryPtr; }break;
		case Value_Type_T::JVALUE_OBJ: { delete m_Value.ObjPtr; }break;
		}
	}


	JValue& JValue::Copy(const JValue& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;

		switch (m_Type)
		{
		case Value_Type_T::JVALUE_STR: this->m_Value.StrPtr = new JString(*(rfJValue.m_Value.StrPtr)); break;
		case Value_Type_T::JVALUE_ARY: this->m_Value.AryPtr = new JArray(*(rfJValue.m_Value.AryPtr)); break;
		case Value_Type_T::JVALUE_OBJ: this->m_Value.ObjPtr = new JObject(*(rfJValue.m_Value.ObjPtr)); break;
		}

		return *this;
	}

	JValue& JValue::Move(JValue& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;
		rfJValue.m_Type = JVALUE_NUL;
		rfJValue.m_Value = { 0 };
		return *this;
	}


	JValue& JValue::operator = (const JValue& rfJValue)
	{
		this->~JValue();
		return this->Copy(rfJValue);
	}

	JValue& JValue::operator = (JValue&& rfJValue) noexcept
	{
		this->~JValue();
		return this->Move(rfJValue);
	}

	//Array
	void JValue::SureArray()
	{
		if (m_Type != JVALUE_ARY)
		{
			if (m_Type == JVALUE_NUL)
			{
				m_Type = JVALUE_ARY;
				m_Value.AryPtr = new JArray();
			}
			else
			{
				throw std::runtime_error("JValue::SureArray: Json Value Not Array!");
			}
		}
	}

	void JValue::Append(const JValue& rfJValue)
	{
		this->SureArray();
		m_Value.AryPtr->emplace_back(rfJValue);
	}

	void JValue::Append(JValue&& rfJValue)
	{
		this->SureArray();
		m_Value.AryPtr->emplace_back(std::move(rfJValue));
	}


	// Obj
	void JValue::SureObject()
	{
		if (m_Type != JVALUE_OBJ)
		{
			if (m_Type == JVALUE_NUL)
			{
				m_Type = JVALUE_OBJ;
				m_Value.ObjPtr = new JObject();
			}
			else
			{
				throw std::runtime_error("JValue::SureObject: Json Value Not Object!");
			}
		}
	}

	JValue& JValue::operator[](const wchar_t* wpKey)
	{
		JObject& obj = this->ToOBJ();
		return (*this->m_Value.ObjPtr)[wpKey];
	}

	void JValue::AddKey(std::wstring_view wsKey)
	{
		this->SureObject();
		(*this->m_Value.ObjPtr)[std::wstring(wsKey)];
	}

	void JValue::AddKey(std::wstring_view wsKey, const JValue& rfJValue)
	{
		this->SureObject();
		(*this->m_Value.ObjPtr)[std::wstring(wsKey)] = rfJValue;
	}

	void JValue::AddKey(std::wstring_view wsKey, JValue&& rfJValue)
	{
		this->SureObject();
		(*this->m_Value.ObjPtr)[std::wstring(wsKey)] = std::move(rfJValue);
	}

	JObject::iterator JValue::FindKey(std::wstring_view wsKey) const
	{
		return m_Value.ObjPtr->find(std::wstring(wsKey));
	}

	JObject::iterator JValue::EndKey() const noexcept
	{
		return m_Value.ObjPtr->end();
	}

	JValue& JValue::GetValue(JObject::iterator itObj) const
	{
		return (*itObj).second;
	}

	Value_Type_T JValue::GetType() const noexcept
	{
		return m_Type;
	}

	JValue::operator bool() const
	{
		if (m_Type == JVALUE_BOL) { return m_Value.Bool; }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator size_t() const
	{
		return (size_t)this->operator int();
	}

	JValue::operator int() const
	{
		if (m_Type == JVALUE_INT) { return m_Value.Int; }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator double() const
	{
		if (m_Type == JVALUE_DBL) { return m_Value.Double; }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator JString& () const
	{
		if (m_Type == JVALUE_STR) { return *(m_Value.StrPtr); }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator JArray& () const
	{
		if (m_Type == JVALUE_ARY) { return *(m_Value.AryPtr); }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator JObject& () const
	{
		if (m_Type == JVALUE_OBJ) { return *(m_Value.ObjPtr); }
		throw std::runtime_error("Error Json Type!");
	}

	JValue::operator std::wstring_view() const
	{
		return this->operator JString & ();
	}


	JArray& JValue::ToAry()
	{
		this->SureArray();
		return *m_Value.AryPtr;
	}

	JObject& JValue::ToOBJ()
	{
		this->SureObject();
		return *(m_Value.ObjPtr);
	}

	int JValue::ToInt() const
	{
		return this->operator int();
	}

	bool JValue::ToBool() const
	{
		return this->operator bool();
	}

	double JValue::ToDouble() const
	{
		return this->operator double();
	}

	const wchar_t* JValue::ToStrPtr() const
	{
		return this->operator std::wstring_view().data();
	}

	std::wstring_view JValue::ToStrView() const
	{
		return this->operator std::wstring_view();
	}


	void JValue::Dump(std::wstring& wsText, bool isFormat, bool isOrder) const
	{
		static size_t count = 0;

		switch (m_Type)
		{
		case Value_Type_T::JVALUE_NUL: { wsText.append(L"null"); } break;
		case Value_Type_T::JVALUE_BOL: { wsText.append((m_Value.Bool) ? L"true" : L"false"); } break;
		case Value_Type_T::JVALUE_INT: { wsText.append(std::to_wstring(m_Value.Int)); } break;
		case Value_Type_T::JVALUE_DBL: { wsText.append(std::to_wstring(m_Value.Double)); } break;

		case Value_Type_T::JVALUE_STR:
		{
			wsText.append(1, L'\"');
			for (auto ch : *(m_Value.StrPtr))
			{
				switch (ch)
				{
				case L'\n': ch = L'n'; break;
				case L'\r': ch = L'r'; break;
				case L'\b': ch = L'b'; break;
				case L'\t': ch = L't'; break;
				case L'\f': ch = L'f'; break;
				case L'\"': ch = L'"'; break;
				case L'\\': ch = L'\\'; break;
				default:
				{
					wsText.append(1, ch);
					continue;
				}
				}

				wsText.append(1, L'\\');
				wsText.append(1, ch);
			}
			wsText.append(1, L'\"');
		}
		break;

		case Value_Type_T::JVALUE_ARY:
		{
			wsText.append(1, L'[');

			count++;
			{
				for (auto& value : *(m_Value.AryPtr))
				{
					if (isFormat)
					{
						wsText.append(1, L'\n');
						wsText.append(count, L'\t');
					}
					value.Dump(wsText, isFormat);
					wsText.append(1, L',');
				}
			}
			count--;

			// if not null array
			if (wsText.back() == L',')
			{
				wsText.pop_back();
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
			}

			wsText.append(1, L']');
		}
		break;

		case Value_Type_T::JVALUE_OBJ:
		{
			wsText.append(1, L'{');

			count++;
			{
				auto fn_proce_one = [&wsText, &isFormat](const std::pair<const std::wstring, JValue>& rfKV)
					{
						if (isFormat)
						{
							wsText.append(1, L'\n');
							wsText.append(count, L'\t');
						}
						wsText.append(1, L'\"');
						wsText.append(rfKV.first);
						wsText.append(1, L'\"');
						wsText.append(1, L':');
						if (isFormat)
						{
							wsText.append(1, L' ');
						}
						rfKV.second.Dump(wsText, isFormat);
						wsText.append(1, L',');
					};

				const std::unordered_map<std::wstring, JValue>& unorder_map = *(m_Value.ObjPtr);

				if (isOrder)
				{
					std::map<std::wstring, JValue> map;
					for (auto& value : unorder_map)
					{
						map.insert(value);
					}

					for (const auto& value : map)
					{
						fn_proce_one(value);
					}
				}
				else
				{

					for (const auto& value : unorder_map)
					{
						fn_proce_one(value);
					}
				}

			}
			count--;

			// if not null object
			if (wsText.back() == L',')
			{
				wsText.pop_back();
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
			}

			wsText.append(1, L'}');
		}
		break;
		}
	}
}

namespace Rut::RxJson
{
	Parser::Parser()
	{
		m_nJsonCCH = 0;
		m_nReadCCH = 0;
	}

	Parser::Parser(std::wstring_view wsJson) : Parser()
	{
		this->Open(wsJson);
	}

	Parser::Parser(std::wstring_view wsJson, JValue& rfJValue) : Parser(wsJson)
	{
		this->Read(rfJValue);
	}

	Parser::~Parser()
	{

	}


	void Parser::AddReadCCH(size_t nCount)
	{
		this->m_nReadCCH += nCount;
	}

	wchar_t Parser::GetCurChar()
	{
		return m_uqJson[m_nReadCCH];
	}

	wchar_t* Parser::GetCurPtr()
	{
		return m_uqJson.get() + m_nReadCCH;
	}

	size_t Parser::GeReadCCH() const
	{
		return m_nReadCCH;
	}

	size_t Parser::GetJsonCCH() const
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


	void Parser::ParseArray(JValue& rfJValue)
	{
		assert(this->GetCurChar() == L'[');

		this->AddReadCCH();

		while (this->GetToken() != L']')
		{
			JValue value;
			this->ParseValue(value);
			rfJValue.Append(std::move(value));
		}

		this->AddReadCCH();

		rfJValue.SureArray();
		return;
	}

	void Parser::ParseObject(JValue& rfJValue)
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

				JValue value;
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

	void Parser::ParseNumber(JValue& rfJValue)
	{
		wchar_t* end = nullptr;
		wchar_t* beg = this->GetCurPtr();
		double num_org = ::wcstod(beg, &end);

		this->AddReadCCH(end - beg);

		int num_int = (int)(num_org);
		double num_loss = (double)(num_int);

		num_org == num_loss ? (rfJValue = (int)num_int) : (rfJValue = num_org);
	}

	void Parser::ParseString(JValue& rfJValue)
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

		JValue value(std::move(text));
		rfJValue = std::move(value);
	}

	void Parser::ParseTrue(JValue& rfJValue)
	{
		assert(this->GetCurChar() == L't');

		this->AddReadCCH(4);
		rfJValue = true;
	}

	void Parser::ParseFalse(JValue& rfJValue)
	{
		assert(this->GetCurChar() == L'f');

		this->AddReadCCH(5);
		rfJValue = false;
	}

	void Parser::ParseNull(JValue& rfJValue)
	{
		assert(this->GetCurChar() == L'n');

		this->AddReadCCH(4);
		rfJValue = JValue();
	}

	void Parser::ParseValue(JValue& rfJValue)
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


	void Parser::Open(const std::filesystem::path& phJson)
	{
		std::wstring json_text;
		RxFile::Text{ phJson, RIO_READ, RFM_UTF8 }.ReadRawText(json_text);

		this->m_nJsonCCH = json_text.size();
		this->m_nReadCCH = 0;
		this->m_uqJson = std::make_unique_for_overwrite<wchar_t[]>(m_nJsonCCH + 1);
		wcsncpy_s(this->m_uqJson.get(), m_nJsonCCH + 1, json_text.data(), this->m_nJsonCCH);
		this->m_uqJson[this->m_nJsonCCH] = L'\0';
	}

	bool Parser::Read(JValue& rfJValue)
	{
		this->ParseValue(rfJValue);
		this->SkipWhite();
		return (this->GeReadCCH() >= this->GetJsonCCH()) ? (true) : (false);
	}

	bool Parser::Load(const std::filesystem::path& phJson, JValue& rfJValue)
	{
		this->Open(phJson);
		return this->Read(rfJValue);
	}

	JValue Parser::Load(const std::filesystem::path& phJson)
	{
		JValue json;
		this->Load(phJson, json);
		return json;
	}

	void Parser::Save(const JValue& rfJVaue, const std::filesystem::path& phJson, bool isFormat, bool isOrder)
	{
		std::wstring text;
		rfJVaue.Dump(text, isFormat, isOrder);
		RxFile::Text{ phJson ,RIO_WRITE, RFM_UTF8 }.WriteLine(text);
	}
}