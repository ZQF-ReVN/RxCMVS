#pragma once
#include <string>
#include <unordered_map>


namespace Rut::RxINI
{
	class Value
	{
	private:
		std::wstring m_wsValue;

	public:
		Value();

		Value(const int iValue);
		Value(const uint32_t uiValue);
		Value(const float fValue);
		Value(const double dValue);
		Value(const bool bValue);
		Value(const wchar_t* wValue);
		Value(const std::wstring& wsValue);

		operator const int();
		operator const uint32_t();
		operator const float();
		operator const double();
		operator const bool();
		operator const wchar_t* ();
		operator const std::string() const;
		operator const std::wstring() const;
		operator std::wstring& ();

		bool Empty();
	};
}

namespace Rut::RxINI
{
	typedef std::wstring							Name;
	typedef std::wstring							NodeName;
	typedef std::unordered_map<Name, Value>			KeysMap;
	typedef std::unordered_map<NodeName, KeysMap>	NodesMap;

	class Parser
	{
	private:
		NodesMap m_mpNodes;

	public:
		NodesMap::iterator At(const std::wstring& wsNode);
		NodesMap::iterator End();
		void Parse(const std::wstring& wsINI);

		Parser();
		Parser(const std::wstring& wsINI);

		std::wstring Dump();
		void Save(const std::wstring& wsFile);

		KeysMap& operator[] (const std::wstring& wsNode);
		KeysMap& Get(const std::wstring& wsNode);
		Value& Get(const std::wstring& wsNode, const std::wstring& wsName);
		void Add(const std::wstring& wsNode, const std::wstring& wsName, const Value& vValue);
		bool Has(const std::wstring& wsNode);
		bool Has(const std::wstring& wsNode, const std::wstring& wsName);

	};
}