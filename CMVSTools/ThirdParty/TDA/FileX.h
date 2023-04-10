#pragma once
#include <string>
#include <fstream>


namespace TDA
{
	class FileX
	{
	public:
		static bool         SaveFileViaPath(wchar_t* pPath, void* pBuffer, size_t nSize);

		static size_t       GetCurrentDirectoryFolder(char* pPath);
		static size_t       GetCurrentDirectoryFolder(wchar_t* pPath);
		static size_t       GetCurrentDirectoryFolder(std::string& msPath);
		static size_t       GetCurrentDirectoryFolder(std::wstring& wsPath);
		static std::string  GetCurrentDirectoryFolder_RETA();
		static std::wstring GetCurrentDirectoryFolder_RETW();


		static void         BackSlash(char* pPath);
		static void         BackSlash(wchar_t* pPath);
		static void         BackSlash(const char* pPath, std::string& msPath);
		static void         BackSlash(const wchar_t* pPath, std::wstring& wsPath);
		static std::string  BackSlash_RET(const char* pPath);
		static std::wstring BackSlash_RET(const wchar_t* pPath);
			

		static bool         PathRemoveFileName(std::string& msPath, std::string& msRemoved);
		static bool         PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved);
		static std::string  PathRemoveFileName_RET(std::string& msPath);
		static std::wstring PathRemoveFileName_RET(std::wstring& wsPath);

						    
		static size_t       PathGetFileName(wchar_t* pPath);
		static size_t       PathGetFileName(char* pPath);
		static void         PathGetFileName(std::string& msPath, std::string& msFileName);
		static void         PathGetFileName(std::wstring& msPath, std::wstring& wsFileName);
		static std::string  PathGetFileName_RET(std::string& msPath);
		static std::wstring PathGetFileName_RET(std::wstring& wsPath);



		static std::streamsize GetFileSize(const wchar_t* wsFile);
		static std::streamsize GetFileSize(const char* msFile);
		static std::streamsize GetFileSize(std::ifstream& ifsFile);
		static std::streamsize GetFileSize(std::fstream& fsFile);
	};
}

