#include "Value.h"

#include <stdexcept>


namespace Rut::RxJson
{
	Value::Value()
	{
		m_Type = JVALUE_NUL;
		m_Value = { 0 };
	}

	Value::Value(bool bValue)
	{
		m_Type = JVALUE_BOL;
		m_Value.Bool = bValue;
	}

	Value::Value(int iValue)
	{
		m_Type = JVALUE_INT;
		m_Value.Int = iValue;
	}

	Value::Value(double dValue)
	{
		m_Type = JVALUE_DBL;
		m_Value.Double = dValue;
	}

	Value::Value(const wchar_t* sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(std::wstring_view sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(const std::wstring& sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(sValue);
	}

	Value::Value(std::wstring&& sValue)
	{
		m_Type = JVALUE_STR;
		m_Value.pStr = new JString(std::move(sValue));
	}

	Value::Value(const JArray& aValue)
	{
		m_Type = JVALUE_ARY;
		m_Value.pAry = new JArray(aValue);
	}

	Value::Value(JArray&& aValue)
	{
		m_Type = JVALUE_ARY;
		m_Value.pAry = new JArray(std::move(aValue));
	}

	Value::Value(const JObject& oValue)
	{
		m_Type = JVALUE_OBJ;
		m_Value.pObj = new JObject(oValue);
	}

	Value::Value(JObject&& oValue)
	{
		m_Type = JVALUE_OBJ;
		m_Value.pObj = new JObject(std::move(oValue));
	}

	Value::Value(const Value& rfJValue)
	{
		m_Type = JVALUE_NUL;
		Copy(rfJValue);
	}

	Value::Value(Value&& rfJValue) noexcept
	{
		m_Type = JVALUE_NUL;
		Move(rfJValue);
	}

	Value::~Value()
	{
		switch (m_Type)
		{
		case JValue_Type::JVALUE_STR:
		{
			delete m_Value.pStr;
		}
		break;

		case JValue_Type::JVALUE_ARY:
		{
			delete m_Value.pAry;
		}
		break;

		case JValue_Type::JVALUE_OBJ:
		{
			delete m_Value.pObj;
		}
		break;
		}
	}


	Value& Value::Copy(const Value& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;

		switch (m_Type)
		{
		case JValue_Type::JVALUE_STR:
			this->m_Value.pStr = new JString(*(rfJValue.m_Value.pStr)); break;
		case JValue_Type::JVALUE_ARY:
			this->m_Value.pAry = new JArray(*(rfJValue.m_Value.pAry)); break;
		case JValue_Type::JVALUE_OBJ:
			this->m_Value.pObj = new JObject(*(rfJValue.m_Value.pObj)); break;
		}

		return *this;
	}

	Value& Value::Move(Value& rfJValue)
	{
		this->m_Type = rfJValue.m_Type;
		this->m_Value = rfJValue.m_Value;
		rfJValue.m_Type = JVALUE_NUL;
		rfJValue.m_Value = { 0 };
		return *this;
	}


	Value& Value::operator = (const Value& rfJValue)
	{
		this->~Value();
		return Copy(rfJValue);
	}

	Value& Value::operator = (Value&& rfJValue) noexcept
	{
		this->~Value();
		return Move(rfJValue);
	}

	//Array
	void Value::SureArray()
	{
		if (m_Type == JVALUE_NUL)
		{
			m_Type = JVALUE_ARY;
			m_Value.pAry = new JArray();
		}

		if (m_Type != JVALUE_ARY)
		{
			throw std::runtime_error("Error Json Value Not Array!");
		}
	}

	void Value::Append(const Value& rfJValue)
	{
		SureArray();
		m_Value.pAry->push_back(rfJValue);
	}

	void Value::Append(Value&& rfJValue)
	{
		SureArray();
		m_Value.pAry->emplace_back(std::move(rfJValue));
	}


	// Obj
	Value& Value::operator[](const wchar_t* wpKey)
	{
		JObject& obj = this->ToOBJ();
		return (*this->m_Value.pObj)[wpKey];
	}

	void Value::SureObject()
	{
		if (m_Type == JVALUE_NUL)
		{
			m_Type = JVALUE_OBJ;
			m_Value.pObj = new JObject();
		}

		if (m_Type != JVALUE_OBJ)
		{
			throw std::runtime_error("Error Json Value Not Object!");
		}
	}

	void Value::AddKey(std::wstring_view wsKey)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))];
	}

	void Value::AddKey(std::wstring_view wsKey, const Value& rfJValue)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))] = rfJValue;
	}

	void Value::AddKey(std::wstring_view wsKey, Value&& rfJValue)
	{
		SureObject();
		(*this->m_Value.pObj)[std::move(std::wstring(wsKey))] = std::move(rfJValue);
	}

	bool Value::HasKey(std::wstring_view wsKey)
	{
		auto ite = m_Value.pObj->find(std::wstring(wsKey));
		return ite == m_Value.pObj->end() ? false : true;
	}


	JValue_Type Value::GetType()
	{
		return m_Type;
	}

	Value::operator bool()
	{
		if (m_Type == JVALUE_BOL) { return m_Value.Bool; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator int()
	{
		if (m_Type == JVALUE_INT) { return m_Value.Int; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator double()
	{
		if (m_Type == JVALUE_DBL) { return m_Value.Double; }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JString& ()
	{
		if (m_Type == JVALUE_STR) { return *(m_Value.pStr); }
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JArray& ()
	{
		return this->ToAry();
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator JObject& ()
	{
		return this->ToOBJ();
		throw std::runtime_error("Error Json Type!");
	}

	Value::operator std::wstring_view()
	{
		const std::wstring& str = this->operator JString & ();
		return str;
	}


	JArray& Value::ToAry()
	{
		SureArray();
		return *m_Value.pAry;
	}

	JObject& Value::ToOBJ()
	{
		SureObject();
		return *(m_Value.pObj);
	}

	int Value::ToInt()
	{
		return this->operator int();
	}

	bool Value::ToBool()
	{
		return this->operator bool();
	}

	double Value::ToDouble()
	{
		return this->operator double();
	}

	const wchar_t* Value::ToStringPtr()
	{
		return this->operator std::wstring_view().data();
	}

	std::wstring_view Value::ToStringView()
	{
		return this->operator std::wstring_view();
	}


	void Value::Dump(std::wstring& wsText, bool isFormat)
	{
		static size_t count = 0;

		switch (m_Type)
		{
		case JValue_Type::JVALUE_NUL:
		{
			wsText.append(L"null");
		}
		break;

		case JValue_Type::JVALUE_BOL:
		{
			wsText.append((m_Value.Bool) ? L"true" : L"false");
		}
		break;

		case JValue_Type::JVALUE_INT:
		{
			wsText.append(std::to_wstring(m_Value.Int));
		}
		break;

		case JValue_Type::JVALUE_DBL:
		{
			wsText.append(std::to_wstring(m_Value.Double));
		}
		break;

		case JValue_Type::JVALUE_STR:
		{
			wsText.append(1, L'\"');
			for (auto ch : *(m_Value.pStr))
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

		case JValue_Type::JVALUE_ARY:
		{
			wsText.append(1, L'[');

			count++;
			for (auto& value : *(m_Value.pAry))
			{
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
				value.Dump(wsText, isFormat);
				wsText.append(1, L',');
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

		case JValue_Type::JVALUE_OBJ:
		{
			wsText.append(1, L'{');

			count++;
			for (auto& value : *(m_Value.pObj))
			{
				if (isFormat)
				{
					wsText.append(1, L'\n');
					wsText.append(count, L'\t');
				}
				wsText.append(1, L'\"');
				wsText.append(value.first);
				wsText.append(1, L'\"');
				wsText.append(1, L':');
				if (isFormat)
				{
					wsText.append(1, L' ');
				}
				value.second.Dump(wsText, isFormat);
				wsText.append(1, L',');
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