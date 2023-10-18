#include "Parser.h"
#include "../../Rut/RxStr.h"
#include "../../Rut/RxFile.h"

#include <sstream>


namespace Rut::RxINI
{
	Parser::Parser()
	{

	}

	Parser::Parser(const std::wstring& wsINI)
	{
		Parse(wsINI);
	}

	void Parser::Parse(const std::wstring& wsINI)
	{
		std::vector<std::wstring> text_line;
		RxFile::Text wifs_ini = { wsINI, RIO_IN, RFM_UTF8 };
		wifs_ini.ReadAllLine(text_line);

		std::size_t pos = -1;
		std::wstring node_name;
		for (auto& line : text_line)
		{
			if (line.empty()) { continue; }

			switch (line[0])
			{
			case L'#':case L';':case L'/': // Comment Char
				break;

			case L'[': // [Node]
			{
				pos = line.find_first_of(L']');
				if (pos == std::wstring::npos) { throw std::runtime_error("INI_File:Parse: Get Node Error!"); }
				node_name = RxStr::Trim(line.substr(1, pos - 1));
			}
			break;

			default:// Key = Value
			{
				pos = line.find_first_of(L'=');
				if ((pos == std::wstring::npos) || (pos == 0)) { throw std::runtime_error("INI_File::Parse: Get Key Error!"); }
				m_mpNodes[node_name][RxStr::Trim(line.substr(0, pos))] = RxStr::Trim(line.substr(pos + 1));
			}
			break;
			}
		}
	}

	void Parser::Save(const std::wstring& wsFile)
	{
		RxFile::Text wofs_ini{ wsFile, RIO_OUT, RFM_UTF8 };
		std::wstring dump = Dump();
		wofs_ini.WriteLine(dump);
	}

	std::wstring Parser::Dump()
	{
		std::wstringstream ss;
		for (auto& node : m_mpNodes)
		{
			ss << L"[" << node.first << L"]" << L'\n';
			for (auto& key : node.second) { ss << key.first << L"=" << std::wstring(key.second) << L'\n'; }
			ss << L'\n';
		}
		return ss.str();
	}


	NodesMap::iterator Parser::At(const std::wstring& wsNode)
	{
		return m_mpNodes.find(wsNode);
	}

	NodesMap::iterator Parser::End()
	{
		return m_mpNodes.end();
	}

	KeysMap& Parser::Get(const std::wstring& wsNode)
	{
		const auto& ite_node = At(wsNode);
		if (ite_node == End()) { throw std::runtime_error("INI_File::Get: INI File No Find Node"); }
		return ite_node->second;
	}

	Value& Parser::Get(const std::wstring& wsNode, const std::wstring& wsName)
	{
		auto& keys = Get(wsNode);
		const auto& ite_keys = keys.find(wsName);
		if (ite_keys == keys.end()) { throw std::runtime_error("INI_File::Get: INI File No Find Key"); }
		return ite_keys->second;
	}

	KeysMap& Parser::operator[] (const std::wstring& wsNode)
	{
		return Get(wsNode);
	}

	void Parser::Add(const std::wstring& wsNode, const std::wstring& wsName, const Value& vValue)
	{
		m_mpNodes[wsNode][wsName] = vValue;
	}

	bool Parser::Has(const std::wstring& wsNode)
	{
		return At(wsNode) != End() ? true : false;
	}

	bool Parser::Has(const std::wstring& wsNode, const std::wstring& wsName)
	{
		auto ite_node = At(wsNode);
		if (ite_node != End())
		{
			auto& keys = ite_node->second;
			auto ite_keys = keys.find(wsName);
			return ite_keys != keys.end() ? true : false;
		}
		return false;
	}
}