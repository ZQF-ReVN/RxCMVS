#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>


namespace Rut::RxCmd::ArgManager
{
	class Value
	{
	private:
		std::wstring m_wsValue;
		std::wstring m_wsHelp;

	public:
		Value();
		Value(std::wstring_view wsValue, std::wstring_view wsHelp);

	public:
		void SetValue(std::wstring_view wsValue);
		void SetHelp(std::wstring_view wsHelp);
		const std::wstring& GetValue() const;
		const std::wstring& GetHelp() const;

	public:
		size_t ToNum() const;
		const std::wstring& ToWStr() const;
		std::wstring_view ToWStrView() const;

	public:
		operator bool() const = delete;
		operator size_t() const;
		operator std::wstring_view () const;
		operator const std::wstring& () const;
		operator std::filesystem::path() const;
		bool operator==(std::wstring_view wsValue) const;
	};

}

namespace Rut::RxCmd::ArgManager
{
	class Parser
	{
	private:
		std::wstring m_wsProgramName;
		std::vector<std::wstring> m_vcExample;
		std::unordered_map<std::wstring, Value> m_mpCmd;

	public:
		Parser();

	public:
		bool Load(size_t nArg, wchar_t* aArgPtr[]);
		void ShowHelp();
		void AddCmd(std::wstring_view wsOption, std::wstring_view wsHelp);
		void AddExample(std::wstring_view wsExample);
		bool Ready();

	public:
		const Value& operator[](int) = delete;
		const Value& operator[](size_t) = delete;
		const Value& operator[](std::wstring_view wsOption);
	};
}

namespace Rut::RxCmd
{
	bool Alloc(const wchar_t* lpTitle, bool isEdit = false);

	bool Put(const char* cpStr, size_t nChar);
	bool Put(std::string_view msStr);
	bool Put(const wchar_t* wpStr, size_t nChar);
	bool Put(std::wstring_view wsStr);
	bool PutMBCS(const char* cpStr, size_t nCodePage);
	bool PutMBCS(std::string_view msStr, size_t nCodePage);
	bool PutFormat(const char* cpFormat, ...);
	bool PutFormat(const wchar_t* cpFormat, ...);

	using Arg = Rut::RxCmd::ArgManager::Parser;
}
