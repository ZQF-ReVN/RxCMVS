#include "FileX.h"

#include <Windows.h>
#include <shlobj.h>


namespace TDA
{
	bool FileX::SaveFileViaPath(wchar_t* pPath, void* pBuffer, size_t nSize)
	{
		std::wstring path = GetCurrentDirectoryFolder_RETW() + pPath;

		SHCreateDirectoryExW(NULL, PathRemoveFileName_RET(path).c_str(), NULL);

		HANDLE hFile = CreateFileW(pPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) return false;

		bool isWritten = WriteFile(hFile, pBuffer, nSize, NULL, NULL);
		FlushFileBuffers(hFile);
		CloseHandle(hFile);

		return isWritten;
	}


	size_t FileX::GetCurrentDirectoryFolder(char* pPath)
	{
		size_t sz = GetCurrentDirectoryA(MAX_PATH, pPath);
		memcpy(pPath + sz, "\\", 1);
		return ++sz;
	}

	size_t FileX::GetCurrentDirectoryFolder(wchar_t* pPath)
	{
		size_t sz = GetCurrentDirectoryW(MAX_PATH, pPath);
		memcpy(pPath + sz, L"\\", 2);
		return ++sz;
	}

	size_t FileX::GetCurrentDirectoryFolder(std::string& msPath)
	{
		size_t len = 0;
		msPath.resize(MAX_PATH);
		len = GetCurrentDirectoryFolder(const_cast<char*>(msPath.data()));
		msPath.resize(len);
		return len;
	}

	size_t FileX::GetCurrentDirectoryFolder(std::wstring& wsPath)
	{
		size_t len = 0;
		wsPath.resize(MAX_PATH);
		len = GetCurrentDirectoryFolder(const_cast<wchar_t*>(wsPath.data()));
		wsPath.resize(len);
		return len;
	}

	std::string FileX::GetCurrentDirectoryFolder_RETA()
	{
		std::string path;
		GetCurrentDirectoryFolder(path);
		return path;
	}

	std::wstring FileX::GetCurrentDirectoryFolder_RETW()
	{
		std::wstring path;
		GetCurrentDirectoryFolder(path);
		return path;
	}


	void FileX::BackSlash(char* pPath)
	{
		for (size_t ite = 0; pPath[ite]; ite++)
		{
			if ((uint8_t)pPath[ite] > 0x7F) { ite++; continue; }

			switch (pPath[ite])
			{
			case '/':
				pPath[ite] = '\\';
				break;
			case '\\':
				pPath[ite] = '/';
				break;
			}
		}
	}

	void FileX::BackSlash(wchar_t* pPath)
	{
		for (size_t ite = 0; pPath[ite]; ite++)
		{
			switch (pPath[ite])
			{
			case L'/':
				pPath[ite] = L'\\';
				break;
			case L'\\':
				pPath[ite] = L'/';
				break;
			}
		}
	}

	void FileX::BackSlash(const char* pPath, std::string& msPath)
	{
		msPath = pPath;
		BackSlash(const_cast<char*>(msPath.data()));
	}

	void FileX::BackSlash(const wchar_t* pPath, std::wstring& wsPath)
	{
		wsPath = pPath;
		BackSlash(const_cast<wchar_t*>(wsPath.data()));
	}

	std::string FileX::BackSlash_RET(const char* pPath)
	{
		std::string msPath = pPath;
		BackSlash(const_cast<char*>(msPath.data()));
		return msPath;
	}

	std::wstring FileX::BackSlash_RET(const wchar_t* pPath)
	{
		std::wstring wsPath = pPath;
		BackSlash(const_cast<wchar_t*>(wsPath.data()));
		return wsPath;
	}


	bool FileX::PathRemoveFileName(std::string& msPath, std::string& msRemoved)
	{
		size_t pos = msPath.rfind("\\");
		if (pos == std::wstring::npos) return false;
		msRemoved = msPath.substr(0, pos + 1);
		return true;
	}

	bool FileX::PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved)
	{
		size_t pos = wsPath.rfind(L"\\");
		if (pos == std::wstring::npos) return false;
		wsRemoved = wsPath.substr(0, pos + 1);
		return true;
	}

	std::string FileX::PathRemoveFileName_RET(std::string& msPath)
	{
		std::string folder;
		PathRemoveFileName(msPath, folder);
		return folder;
	}

	std::wstring FileX::PathRemoveFileName_RET(std::wstring& wsPath)
	{
		std::wstring folder;
		PathRemoveFileName(wsPath, folder);
		return folder;
	}


	size_t FileX::PathGetFileName(char* pPath)
	{
		//StrLen
		size_t len = 0;
		while (pPath[++len]);

		for (size_t iteChar = len; iteChar > 0; iteChar--)
		{
			if (pPath[iteChar - 1] != '\\') continue;
			len -= iteChar;
			memcpy(pPath, pPath + iteChar, len + 1);
			return len;
		}

		return 0;
	}

	size_t FileX::PathGetFileName(wchar_t* pPath)
	{
		//StrLen
		size_t len = 0;
		while (pPath[++len]);

		for (size_t iteChar = len; iteChar > 0; iteChar--)
		{
			if (pPath[iteChar - 1] != L'\\') continue;
			len -= iteChar;
			memcpy(pPath, pPath + iteChar, (len + 1) * 2);
			return len;
		}

		return 0;
	}

	void FileX::PathGetFileName(std::string& msPath, std::string& msFileName)
	{
		msPath.resize(PathGetFileName(const_cast<char*>(msPath.data())));
		msFileName = msPath;
	}

	void FileX::PathGetFileName(std::wstring& wsPath, std::wstring& wsFileName)
	{
		wsPath.resize(PathGetFileName(const_cast<wchar_t*>(wsPath.data())));
		wsFileName = wsPath;
	}

	std::string FileX::PathGetFileName_RET(std::string& msPath)
	{
		std::string filename;
		PathGetFileName(msPath, filename);
		return filename;
	}

	std::wstring FileX::PathGetFileName_RET(std::wstring& wsPath)
	{
		std::wstring filename;
		PathGetFileName(wsPath, filename);
		return filename;
	}


	std::streamsize FileX::GetFileSize(const wchar_t* wsFile)
	{
		std::ifstream ifsFile(wsFile, std::ios::binary);
		if (!ifsFile.is_open()) return false;

		ifsFile.seekg(0, std::ios::end);
		std::streamsize szFile = ifsFile.tellg();

		return szFile;
	}

	std::streamsize FileX::GetFileSize(const char* msFile)
	{
		std::ifstream ifsFile(msFile, std::ios::binary);
		if (!ifsFile.is_open()) return false;

		ifsFile.seekg(0, std::ios::end);
		std::streamsize szFile = ifsFile.tellg();

		return szFile;
	}

	std::streamsize FileX::GetFileSize(std::ifstream& ifsFile)
	{
		std::streamsize size = 0;
		std::streampos old = ifsFile.tellg();

		ifsFile.seekg(0, std::ios::end);
		size = ifsFile.tellg();
		ifsFile.seekg(old, std::ios::beg);

		return size;
	}

	std::streamsize FileX::GetFileSize(std::fstream& fsFile)
	{
		std::streamsize size = 0;
		std::streampos old = fsFile.tellg();

		fsFile.seekg(0, std::ios::end);
		size = fsFile.tellg();
		fsFile.seekg(old, std::ios::beg);

		return size;
	}
}

