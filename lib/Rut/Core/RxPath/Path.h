#pragma once
#include <string>


namespace Rut::RxPath
{
	size_t FileName(char* cpPath, size_t nChar);
	size_t FileName(wchar_t* wpPath, size_t nChar);
	void FileNameRef(std::string& msPath);
	void FileNameRef(std::wstring& wsPath);
	std::string FileName(std::string msPath);
	std::wstring FileName(std::wstring wsPath);

	size_t NotFileName(char* cpPath, size_t nChar);
	size_t NotFileName(wchar_t* wpPath, size_t nChar);
	void NotFileNameRef(std::wstring& sPath);
	void NotFileNameRef(std::string& msPath);
	std::string NotFileName(std::string msPath);
	std::wstring NotFileName(std::wstring wsPath);

	size_t Suffix(char* cpPath, size_t nChar);
	size_t Suffix(wchar_t* wpPath, size_t nChar);
	void SuffixRef(std::string& msPath);
	void SuffixRef(std::wstring& wsPath);
	std::string Suffix(std::string msPath);
	std::wstring Suffix(std::wstring wsPath);

	size_t NotSuffix(char* cpPath, size_t nChar);
	size_t NotSuffix(wchar_t* wpPath, size_t nChar);
	void NotSuffixRef(std::string& msPath);
	void NotSuffixRef(std::wstring& wsPath);
	std::string NotSuffix(std::string msPath);
	std::wstring NotSuffix(std::wstring wsPath);

	char* FormatSlash(char* cpPath, char cSlash);
	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash);
	void FormatSlashRef(std::string& msPath, char cSlash);
	void FormatSlashRef(std::wstring& wsPath, wchar_t cSlash);
	std::string FormatSlash(std::string msPath, char cSlash);
	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash);

	bool Exist(std::string_view msPath);
	bool Exist(std::wstring_view wsPath);

	bool DirExist(std::string_view msPath);
	bool DirExist(std::wstring_view wsPath);

	bool FileExist(std::string_view msPath);
	bool FileExist(std::wstring_view wsPath);

	bool MakeDir(std::string_view msPath);
	bool MakeDir(std::wstring_view wsPath);

	bool MakeDirViaPath(std::string_view msPath);
	bool MakeDirViaPath(std::wstring_view wsPath);

	std::uintmax_t FileSize(std::string_view msPath);
	std::uintmax_t FileSize(std::wstring_view wsPath);

	std::string  ModuleDirA();
	std::wstring ModuleDirW();

	std::string  ModulePathA(void* pBase = nullptr);
	std::wstring ModulePathW(void* pBase = nullptr);

	std::string ModuleNameA(void* pBase = nullptr);
	std::wstring ModuleNameW(void* pBase = nullptr);
}
