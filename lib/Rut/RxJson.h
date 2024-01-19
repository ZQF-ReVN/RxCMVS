#pragma once
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <unordered_map>


namespace Rut::RxJson
{
	class JValue;

	using JBool = bool;
	using JInt = int;
	using JDouble = double;
	using JString = std::wstring;
	using JArray = std::vector<JValue>;
	using JObject = std::unordered_map<std::wstring, JValue>;

	enum Value_Type_T
	{
		JVALUE_NUL,
		JVALUE_BOL,
		JVALUE_INT,
		JVALUE_DBL,
		JVALUE_STR,
		JVALUE_ARY,
		JVALUE_OBJ
	};


	class JValue
	{
	private:
		typedef union
		{
			JBool Bool;
			JInt Int;
			JDouble Double;
			JString* StrPtr;
			JArray* AryPtr;
			JObject* ObjPtr;
		}Value_Union_T;

		Value_Type_T m_Type;
		Value_Union_T m_Value;


	public:
		JValue();
		JValue(bool bValue);
		JValue(int iValue);
		JValue(double dValue);
		JValue(const wchar_t* sValue);
		JValue(std::wstring_view sValue);
		JValue(const std::wstring& sValue);
		JValue(std::wstring&& sValue);
		JValue(const JArray& aValue);
		JValue(JArray&& aValue);
		JValue(const JObject& oValue);
		JValue(JObject&& oValue);
		JValue(const JValue& rfJValue);
		JValue(JValue&& rfJValue) noexcept;
		~JValue();

		JValue& Copy(const JValue& rfJValue);
		JValue& Move(JValue& rfJValue);
		JValue& operator = (const JValue& rfJValue);
		JValue& operator = (JValue&& rfJValue) noexcept;

		//Array
		void SureArray();
		void Append(const JValue& rfJValue);
		void Append(JValue&& rfJValue);

		// Obj
		void SureObject();
		JValue& operator[](const wchar_t* wpKey);
		JValue& operator[](bool) = delete;
		JValue& operator[](size_t) = delete;
		void AddKey(std::wstring_view wsKey);
		void AddKey(std::wstring_view wsKey, const JValue& rfJValue);
		void AddKey(std::wstring_view wsKey, JValue&& rfJValue);
		JObject::iterator FindKey(std::wstring_view wsKey) const;
		JObject::iterator EndKey() const noexcept;
		JValue& GetValue(JObject::iterator itObj) const;

		Value_Type_T GetType() const noexcept;

		operator bool() const;
		operator size_t() const;
		operator int() const;
		operator double() const;
		operator JString& () const;
		operator JArray& () const;
		operator JObject& () const;
		operator std::wstring_view() const;

		JArray& ToAry();
		JObject& ToOBJ();
		int ToInt() const;
		bool ToBool() const;
		double ToDouble() const;
		const wchar_t* ToStrPtr() const;
		std::wstring_view ToStrView() const;

		void Dump(std::wstring& wsText, bool isFormat = true, bool isOrder = false) const;
	};

}

namespace Rut::RxJson
{
	class Parser
	{
	private:
		std::unique_ptr<wchar_t[]> m_uqJson;
		size_t m_nJsonCCH;
		size_t m_nReadCCH;

	public:
		Parser();
		Parser(std::wstring_view wsJson);
		Parser(std::wstring_view wsJson, JValue& rfJValue);
		~Parser();

	private:
		wchar_t SkipWhite();
		wchar_t GetToken();

		void AddReadCCH(size_t nCount = 1);
		wchar_t GetCurChar();
		wchar_t* GetCurPtr();
		size_t GeReadCCH() const;
		size_t GetJsonCCH() const;

		void ParseArray(JValue& rfJValue);
		void ParseObject(JValue& rfJValue);
		void ParseNumber(JValue& rfJValue);
		void ParseString(JValue& rfJValue);
		void ParseTrue(JValue& rfJValue);
		void ParseFalse(JValue& rfJValue);
		void ParseNull(JValue& rfJValue);

		void ParseValue(JValue& rfJValue);

	public:
		void Open(const std::filesystem::path& phJson);
		bool Read(JValue& rfJValue);
		bool Load(const std::filesystem::path& phJson, JValue& rfJValue);
		JValue Load(const std::filesystem::path& phJson);
		static void Save(const JValue& rfJVaue, const std::filesystem::path& phJson, bool isFormat = false, bool isOrder = false);
	};
}