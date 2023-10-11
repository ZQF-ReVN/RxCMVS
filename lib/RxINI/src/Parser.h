#pragma once
#include <string>
#include <unordered_map>

#include "Value.h"


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