#include "Path.h"
#include "../RxCvt.h"
#include "../RxFs.h"

#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


namespace Rut::RxPath
{
	size_t FileName(char* cpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (cpPath[iteChar] != '\\' && cpPath[iteChar] != '/')
			{
				continue;
			}

			if (cpPath[iteChar - 1] <= 0x7F)
			{
				size_t name_len = nChar - iteChar - 1;
				memcpy(cpPath, cpPath + iteChar + 1, name_len);
				cpPath[name_len] = '\0';
				return name_len;
			}
		}

		return nChar;
	}

	size_t FileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (wpPath[iteChar] != L'\\' && wpPath[iteChar] != L'/')
			{
				continue;
			}

			size_t name_len = nChar - iteChar - 1;
			memcpy(wpPath, wpPath + iteChar + 1, name_len * 2);
			wpPath[name_len] = L'\0';
			return name_len;
		}

		return nChar;
	}

	void FileNameRef(std::string& msPath)
	{
		msPath.resize(FileName(msPath.data(), msPath.length()));
	}

	void FileNameRef(std::wstring& wsPath)
	{
		wsPath.resize(FileName(wsPath.data(), wsPath.length()));
	}

	std::string FileName(std::string msPath)
	{
		FileNameRef(msPath);
		return msPath;
	}

	std::wstring FileName(std::wstring wsPath)
	{
		FileNameRef(wsPath);
		return wsPath;
	}


	size_t NotFileName(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '\\')
			{
				if (cpPath[ite_char - 1] <= 0x7F)
				{
					cpPath[ite_char + 1] = '\0';
					return ite_char + 1;
				}
			}

			if (cpPath[ite_char] == '/')
			{
				cpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	size_t NotFileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'\\')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}

			if (wpPath[ite_char] == L'/')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	void NotFileNameRef(std::string& msPath)
	{
		msPath.resize(NotFileName(msPath.data(), msPath.length()));
	}

	void NotFileNameRef(std::wstring& wsPath)
	{
		wsPath.resize(NotFileName(wsPath.data(), wsPath.length()));
	}

	std::string NotFileName(std::string msPath)
	{
		NotFileNameRef(msPath);
		return msPath;
	}

	std::wstring NotFileName(std::wstring msPath)
	{
		NotFileNameRef(msPath);
		return msPath;
	}


	size_t Suffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(cpPath, cpPath + ite_char, suffix_len);
				return ite_char;
			}
		}

		return nChar;
	}

	size_t Suffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(wpPath, wpPath + ite_char, suffix_len * sizeof(wchar_t));
				return ite_char;
			}
		}

		return nChar;
	}

	void SuffixRef(std::string& msPath)
	{
		msPath.resize(Suffix(msPath.data(), msPath.length()));
	}

	void SuffixRef(std::wstring& wsPath)
	{
		wsPath.resize(Suffix(wsPath.data(), wsPath.length()));
	}

	std::string Suffix(std::string msPath)
	{
		SuffixRef(msPath);
		return msPath;
	}

	std::wstring Suffix(std::wstring wsPath)
	{
		SuffixRef(wsPath);
		return wsPath;
	}


	size_t NotSuffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				cpPath[ite_char] = '\0';
				return ite_char;
			}
		}

		return nChar;
	}

	size_t NotSuffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				wpPath[ite_char] = L'\0';
				return ite_char;
			}
		}

		return nChar;
	}

	void NotSuffixRef(std::string& msPath)
	{
		msPath.resize(NotSuffix(msPath.data(), msPath.length()));
	}

	void NotSuffixRef(std::wstring& wsPath)
	{
		wsPath.resize(NotSuffix(wsPath.data(), wsPath.length()));
	}

	std::string NotSuffix(std::string msPath)
	{
		NotSuffixRef(msPath);
		return msPath;
	}

	std::wstring NotSuffix(std::wstring wsPath)
	{
		NotSuffixRef(wsPath);
		return wsPath;
	}


	char* FormatSlash(char* cpPath, char cSlash)
	{
		switch (cSlash)
		{
		case '\\':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if ((uint8_t)cpPath[ite] > 0x7F) { ite++; continue; }

				if (cpPath[ite] == '/') { cpPath[ite] = '\\'; }
			}
		}
		break;

		case '/':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if (cpPath[ite] == '\\') { cpPath[ite] = '/'; }
			}
		}
		break;
		}

		return cpPath;
	}

	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash)
	{
		switch (wcSlash)
		{
		case L'\\':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'/') { wpPath[ite] = L'\\'; }
			}
		}
		break;

		case L'/':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'\\') { wpPath[ite] = L'/'; }
			}
		}
		break;
		}

		return wpPath;
	}

	void FormatSlashRef(std::string& msPath, char cSlash)
	{
		FormatSlash(msPath.data(), cSlash);
	}

	void FormatSlashRef(std::wstring& wsPath, wchar_t wcSlash)
	{
		FormatSlash(wsPath.data(), wcSlash);
	}

	std::string FormatSlash(std::string msPath, char cSlash)
	{
		FormatSlashRef(msPath, cSlash);
		return msPath;
	}

	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash)
	{
		FormatSlashRef(wsPath, wcSlash);
		return wsPath;
	}
}

namespace Rut::RxPath
{
	bool Exist(const char* cpPath)
	{
		return (GetFileAttributesA(cpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}

	bool Exist(const wchar_t* wpPath)
	{
		return (GetFileAttributesW(wpPath) == INVALID_FILE_ATTRIBUTES) ? false : true;
	}

	bool Exist(std::string_view msPath)
	{
		return Exist(msPath.data());
	}

	bool Exist(std::wstring_view wsPath)
	{
		return Exist(wsPath.data());
	}

	bool DirExist(const char* cpPath)
	{
		return PathIsDirectoryA(cpPath);
	}

	bool DirExist(const wchar_t* wpPath)
	{
		return PathIsDirectoryW(wpPath);
	}

	bool DirExist(std::string_view msPath)
	{
		return DirExist(msPath.data());
	}

	bool DirExist(std::wstring_view wsPath)
	{
		return DirExist(wsPath.data());
	}

	bool FileExist(const char* cpPath)
	{
		return PathFileExistsA(cpPath);
	}

	bool FileExist(const wchar_t* wpPath)
	{
		return PathFileExistsW(wpPath);
	}

	bool FileExist(std::string_view msPath)
	{
		return FileExist(msPath.data());
	}

	bool FileExist(std::wstring_view wsPath)
	{
		return FileExist(wsPath.data());
	}

	bool MakeDir(const char* cpPath)
	{
		return CreateDirectoryA(cpPath, nullptr);
	}

	bool MakeDir(const wchar_t* wpPath)
	{
		return CreateDirectoryW(wpPath, nullptr);
	}

	bool MakeDir(std::string_view msPath)
	{
		return MakeDir(msPath.data());
	}

	bool MakeDir(std::wstring_view wsPath)
	{
		return MakeDir(wsPath.data());
	}

	bool MakeDirViaPath(const char* cpPath)
	{
		return MakeDirViaPath(RxCvt::ToWCS(cpPath, CP_ACP).c_str());
	}

	bool MakeDirViaPath(const wchar_t* wpPath)
	{
		const size_t len = wcslen(wpPath);
		wchar_t path[MAX_PATH];
		memcpy(path, wpPath, (len + 1) * 2);

		for (size_t ite_char = 0; ite_char < len; ite_char++)
		{
			switch (path[ite_char])
			{
			case L'/':
			case L'\\':
			{
				path[ite_char] = L'\0';
				CreateDirectoryW(path, NULL);
				path[ite_char] = L'\\';
			}
			break;

			case L'.':
			case L':': { ite_char++; } break;
			}
		}

		return true;
	}

	bool MakeDirViaPath(std::string_view msPath)
	{
		return MakeDirViaPath(msPath.data());
	}

	bool MakeDirViaPath(std::wstring_view wsPath)
	{
		return MakeDirViaPath(wsPath.data());
	}

	std::uintmax_t FileSize(const char* cpPath)
	{
		const HANDLE hfile = CreateFileA(cpPath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return 0; }
		std::uintmax_t h_size = 0;
		std::uintmax_t size = GetFileSize(hfile, (LPDWORD)(&h_size));
		size |= (h_size << 32);
		CloseHandle(hfile);
		return size;
	}

	std::uintmax_t FileSize(const wchar_t* wpPath)
	{
		const HANDLE hfile = CreateFileW(wpPath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfile == INVALID_HANDLE_VALUE) { return 0; }
		std::uintmax_t h_size = 0;
		std::uintmax_t size = GetFileSize(hfile, (LPDWORD)(&h_size));
		size |= (h_size << 32);
		CloseHandle(hfile);
		return size;
	}

	std::uintmax_t FileSize(std::string_view msPath)
	{
		return FileSize(msPath.data());
	}

	std::uintmax_t FileSize(std::wstring_view wsPath)
	{
		return FileSize(wsPath.data());
	}


	size_t ModulePath(char* cpBuf, void* pBase)
	{
		return ::GetModuleFileNameA((HMODULE)pBase, cpBuf, MAX_PATH);
	}

	size_t ModulePath(wchar_t* wpBuf, void* pBase)
	{
		return ::GetModuleFileNameW((HMODULE)pBase, wpBuf, MAX_PATH);
	}

	std::string ModulePathA(void* pBase)
	{
		std::string name;
		name.resize(MAX_PATH);
		name.resize(ModulePath((char*)name.c_str(), pBase));
		return name;
	}

	std::wstring ModulePathW(void* pBase)
	{
		std::wstring name;
		name.resize(MAX_PATH);
		name.resize(ModulePath((wchar_t*)name.c_str(), pBase));
		return name;
	}


	size_t ModuleName(char* cpBuf, void* pBase)
	{
		size_t size = ModulePath(cpBuf, pBase);
		return FileName(cpBuf, size);
	}

	size_t ModuleName(wchar_t* wpBuf, void* pBase)
	{
		size_t size = ModulePath(wpBuf, pBase);
		return FileName(wpBuf, size);
	}

	std::string ModuleNameA(void* pBase)
	{
		std::string name = ModulePathA(pBase);
		FileNameRef(name);
		return name;
	}

	std::wstring ModuleNameW(void* pBase)
	{
		std::wstring name = ModulePathW(pBase);
		FileNameRef(name);
		return name;
	}


	size_t CurrentDir(char* pPath)
	{
		size_t size = GetCurrentDirectoryA(MAX_PATH, pPath);
		memcpy(pPath + size, "\\", 2);
		return ++size;
	}

	size_t CurrentDir(wchar_t* pPath)
	{
		size_t size = GetCurrentDirectoryW(MAX_PATH, pPath);
		memcpy(pPath + size, L"\\", 4);
		return ++size;
	}

	std::string CurrentDirA()
	{
		char tmp[MAX_PATH];
		size_t sz = CurrentDir(tmp);
		std::string path;
		path.resize(sz);
		memcpy((void*)path.c_str(), tmp, sz);
		return path;
	}

	std::wstring CurrentDirW()
	{
		wchar_t tmp[MAX_PATH];
		size_t sz = CurrentDir(tmp);
		std::wstring path;
		path.resize(sz);
		memcpy((void*)path.c_str(), tmp, sz * 2);
		return path;
	}

}
