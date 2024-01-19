#pragma once
#include <string>
#include <vector>


namespace Rut::RxPath
{
	size_t GetFileName(char* cpPath, size_t nChar);
	size_t GetFileName(wchar_t* wpPath, size_t nChar);
	std::string GetFileName(std::string msPath);
	std::wstring GetFileName(std::wstring wsPath);

	size_t RemoveFileName(char* cpPath, size_t nChar);
	size_t RemoveFileName(wchar_t* wpPath, size_t nChar);
	std::string RemoveFileName(std::string msPath);
	std::wstring RemoveFileName(std::wstring wsPath);

	size_t GetSuffix(char* cpPath, size_t nBytes);
	size_t GetSuffix(wchar_t* wpPath, size_t nChar);
	std::string GetSuffix(std::string msPath);
	std::wstring GetSuffix(std::wstring wsPath);

	size_t RemoveSuffix(char* cpPath, size_t nBytes);
	size_t RemoveSuffix(wchar_t* wpPath, size_t nChar);
	std::string RemoveSuffix(std::string msPath);
	std::wstring RemoveSuffix(std::wstring wsPath);

	char* Format(char* cpPath, char cSlash);
	wchar_t* Format(wchar_t* wpPath, wchar_t wcSlash);

	bool MakeDir(std::string_view msPath);
	bool MakeDir(std::wstring_view wsPath);

	bool MakeDirViaPath(std::string_view msPath);
	bool MakeDirViaPath(std::wstring_view wsPath);

	std::uintmax_t GetFileSize(std::string_view msPath);
	std::uintmax_t GetFileSize(std::wstring_view wsPath);

	std::string  ModuleDirA();
	std::wstring ModuleDirW();

	std::string  ModulePathA(void* pBase = nullptr);
	std::wstring ModulePathW(void* pBase = nullptr);

	std::string ModuleNameA(void* pBase = nullptr);
	std::wstring ModuleNameW(void* pBase = nullptr);
}
