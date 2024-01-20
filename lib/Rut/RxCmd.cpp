#include "RxCmd.h"
#include "RxStr.h"
#include "RxSys.h"
#include "RxPath.h"

#include <ranges>
#include <clocale>
#include <stdexcept>


namespace Rut::RxCmd::ArgManager
{
	Value::Value()
	{

	}

	Value::Value(std::wstring_view wsValue, std::wstring_view wsHelp)
	{
		m_wsValue = wsValue;
		m_wsHelp = wsHelp;
	}

	void Value::SetValue(std::wstring_view wsValue)
	{
		m_wsValue = wsValue;
	}

	void Value::SetHelp(std::wstring_view wsHelp)
	{
		m_wsHelp = wsHelp;
	}

	const std::wstring& Value::GetValue() const
	{
		return m_wsValue;
	}

	const std::wstring& Value::GetHelp() const
	{
		return m_wsHelp;
	}

	size_t Value::ToNum() const
	{
		return ::_wtoi(this->GetValue().c_str());
	}

	const std::wstring& Value::ToWStr() const
	{
		return this->GetValue();
	}

	std::wstring_view Value::ToWStrView() const
	{
		return this->GetValue();
	}

	Value::operator size_t() const
	{
		return this->ToNum();
	}

	Value::operator std::wstring_view () const
	{
		return this->GetValue();
	}

	Value::operator const std::wstring& () const
	{
		return this->GetValue();
	}

	Value::operator std::filesystem::path() const
	{
		return this->GetValue();
	}

	bool Value::operator==(std::wstring_view wsValue) const
	{
		return this->GetValue() == wsValue;
	}
}

namespace Rut::RxCmd::ArgManager
{
	Parser::Parser()
	{

	}

	bool Parser::Load(size_t nArg, wchar_t* aArgPtr[])
	{
		if (nArg < 1) { throw std::runtime_error("RxCmd::Parser::Parse: Arg Count Error!"); }

		m_wsProgramName = Rut::RxPath::GetFileName(aArgPtr[0]);

		if (nArg == 1)
		{
			this->ShowHelp();
			return false;
		}

		for (size_t ite_arg = 1; ite_arg < nArg; ite_arg += 2)
		{
			std::wstring_view option = aArgPtr[ite_arg + 0];
			std::wstring_view value = aArgPtr[ite_arg + 1];
			auto ite_map = m_mpCmd.find(option.data());
			if (ite_map == m_mpCmd.end()) { throw std::runtime_error("RxCmd::Parser::Parse: Not Cmd Find!"); }
			ite_map->second.SetValue(value.data());
		}

		return true;
	}

	void Parser::ShowHelp()
	{
		std::wstring help_log;

		help_log.append(L"Command:\n");
		for (auto& cmd : m_mpCmd)
		{
			help_log.append(L"\t");
			help_log.append(cmd.first);
			help_log.append(L"\t");
			help_log.append(cmd.second.GetHelp());
			help_log.append(L"\n");
		}

		help_log.append(L"Example:\n");
		for (auto& exp : m_vcExample)
		{
			help_log.append(L"\t");
			help_log.append(m_wsProgramName);
			help_log.append(L" ");
			help_log.append(exp);
			help_log.append(L"\n");
		}

		RxCmd::Put(help_log);
	}

	void Parser::AddCmd(std::wstring_view wsOption, std::wstring_view wsHelp)
	{
		Value& value = m_mpCmd.operator[](wsOption.data());
		value.SetHelp(wsHelp);
	}

	void Parser::AddExample(std::wstring_view wsExample)
	{
		m_vcExample.emplace_back(wsExample);
	}

	bool Parser::Ready()
	{
		return !m_mpCmd.empty();
	}

	const Value& Parser::operator[](std::wstring_view wsOption)
	{
		auto ite_map = m_mpCmd.find(wsOption.data());
		if (ite_map == m_mpCmd.end()) { throw std::runtime_error("RxCmd::Parser::GetValue: Not Cmd Find!"); }
		return ite_map->second;
	}
}

namespace Rut::RxCmd
{
	static constexpr size_t PUT_BUFFER_MAX = 1024;


	bool Alloc(const wchar_t* lpTitle, bool isEdit)
	{
		Rut::RxSys::ConsoleAlloc(lpTitle);
		Rut::RxSys::ConsoleQuickEditMode(isEdit);
		Rut::RxSys::ConsoleRedirectionSTDIO();

		//system("chcp 65001");
		//system("chcp 936");
		::setlocale(LC_ALL, "chs");
		//std::locale::global(std::locale(""));

		return true;
	}


	bool Put(const char* cpStr, size_t nChar)
	{
		return Rut::RxSys::PutConsole(cpStr, nChar);
	}

	bool Put(std::string_view msStr)
	{
		return Put(msStr.data(), msStr.size());
	}

	bool Put(const wchar_t* wpStr, size_t nChar)
	{
		return Rut::RxSys::PutConsole(wpStr, nChar);
	}

	bool Put(std::wstring_view wsStr)
	{
		return Put(wsStr.data(), wsStr.size());
	}

	bool PutMBCS(const char* cpStr, size_t nCodePage)
	{
		std::wstring out_str;
		size_t cch = RxStr::ToWCS(cpStr, out_str, nCodePage);
		return Put(out_str.c_str(), cch);
	}

	bool PutMBCS(std::string_view msStr, size_t nCodePage)
	{
		return PutMBCS(msStr.data(), nCodePage);
	}

	bool PutFormat(const char* cpFormat, ...)
	{
		char buffer[PUT_BUFFER_MAX];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = RxSys::Sprintf_V(buffer, PUT_BUFFER_MAX, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}

	bool PutFormat(const wchar_t* cpFormat, ...)
	{
		wchar_t buffer[PUT_BUFFER_MAX];

		va_list args = nullptr;
		va_start(args, cpFormat);
		size_t cch = RxSys::Sprintf_V(buffer, PUT_BUFFER_MAX, cpFormat, args);
		va_end(args);

		return (cch <= 0) ? (false) : (Put(buffer, cch));
	}
}


