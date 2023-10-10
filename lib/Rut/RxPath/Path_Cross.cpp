#ifndef WIN32

#include "Path.h"

#include <filesystem>


namespace Rut::RxPath
{
	bool Exist(const char* cpPath)
	{
		return std::filesystem::exists(cpPath);
	}

	bool Exist(const wchar_t* wpPath)
	{
		return std::filesystem::exists(wpPath);
	}

	bool Exist(std::string_view msPath)
	{
		return std::filesystem::exists(msPath);
	}

	bool Exist(std::wstring_view wsPath)
	{
		return std::filesystem::exists(wsPath);
	}


	bool DirExist(const char* cpPath)
	{
		return std::filesystem::is_directory(cpPath);
	}

	bool DirExist(const wchar_t* wpPath)
	{
		return std::filesystem::is_directory(wpPath);
	}

	bool DirExist(std::string_view msPath)
	{
		return std::filesystem::is_directory(msPath);
	}

	bool DirExist(std::wstring_view wsPath)
	{
		return std::filesystem::is_directory(wsPath);
	}
	

	bool FileExist(const char* cpPath)
	{
		return std::filesystem::is_regular_file(cpPath);
	}

	bool FileExist(const wchar_t* wpPath)
	{
		return std::filesystem::is_regular_file(wpPath);
	}

	bool FileExist(std::string_view msPath)
	{
		return std::filesystem::is_regular_file(msPath);
	}

	bool FileExist(std::wstring_view wsPath)
	{
		return std::filesystem::is_regular_file(wsPath);
	}


	bool MakeDir(const char* cpPath)
	{
		return std::filesystem::create_directory(cpPath);
	}

	bool MakeDir(const wchar_t* wpPath)
	{
		return std::filesystem::create_directory(wpPath);
	}

	bool MakeDir(std::string_view msPath)
	{
		return std::filesystem::create_directory(msPath);
	}

	bool MakeDir(std::wstring_view wsPath)
	{
		return std::filesystem::create_directory(wsPath);
	}


	bool MakeDirViaPath(const char* cpPath)
	{
		return std::filesystem::create_directories(cpPath);
	}

	bool MakeDirViaPath(const wchar_t* cpPath)
	{
		return std::filesystem::create_directories(cpPath);
	}

	bool MakeDirViaPath(std::string_view msPath)
	{
		return std::filesystem::create_directories(msPath);
	}

	bool MakeDirViaPath(std::wstring_view wsPath)
	{
		return std::filesystem::create_directories(wsPath);
	}


	std::uintmax_t FileSize(const char* cpPath)
	{
		return std::filesystem::file_size(cpPath);
	}

	std::uintmax_t FileSize(const wchar_t* wpPath)
	{
		return std::filesystem::file_size(wpPath);
	}

	std::uintmax_t FileSize(std::string_view msPath)
	{
		return std::filesystem::file_size(msPath);
	}

	std::uintmax_t FileSize(std::wstring_view wsPath)
	{
		return std::filesystem::file_size(wsPath);
	}


	size_t CurrentDir(char* pPath)
	{
		std::string path = CurrentDirA();
		if (path.size() > 260) { return 0; }
		memcpy(pPath, path.data(), path.size());
		return path.size();
	}

	size_t CurrentDir(wchar_t* pPath)
	{
		std::wstring path = CurrentDirW();
		if (path.size() > 260) { return 0; }
		memcpy(pPath, path.data(), path.size() * sizeof(wchar_t));
		return path.size();
	}

	std::string CurrentDirA()
	{
		return std::filesystem::current_path().string();
	}

	std::wstring CurrentDirW()
	{
		return std::filesystem::current_path().wstring();
	}
}


//namespace Rut::RxPath::inline Std
//{
//	template <typename T_STR> bool Exist(const T_STR& tPath)
//	{
//		return std::filesystem::exists(tPath);
//	}
//
//	template <typename T_STR> bool FileExist(const T_STR& tPath)
//	{
//		return std::filesystem::is_regular_file(tPath);
//	}
//
//	template <typename T_STR> bool DirExist(const T_STR& tPath)
//	{
//		return std::filesystem::is_directory(tPath);
//	}
//
//	template <typename T_STR> void MakeDir(const T_STR& tPath)
//	{
//		std::filesystem::create_directory(tPath);
//	}
//
//	template <typename T_STR> void MakeDirViaPath(const T_STR& tPath)
//	{
//		std::filesystem::create_directories(tPath);
//	}
//
//	template <typename T_STR> std::uintmax_t FileSize(const T_STR& tPath)
//	{
//		return std::filesystem::file_size(tPath);
//	}
//
//	
//std::filesystem::path CurrentPath()
//{
//	return std::filesystem::current_path();
//}
//
//
//	//template <typename T_STR> std::filesystem::path FileName(const T_STR& tPath)
//	//{
//	//	return std::filesystem::path(tPath).filename();
//	//}
//
//	//template <typename T_STR> std::filesystem::path NotFileName(const T_STR& tPath)
//	//{
//	//	return std::filesystem::path(tPath).remove_filename();
//	//}
//
//	//template <typename T_STR> std::filesystem::path Suffix(const T_STR& tPath)
//	//{
//	//	return std::filesystem::path(tPath).extension();
//	//}
//
//	//template <typename T_STR> std::filesystem::path NotSuffix(const T_STR& tPath)
//	//{
//	//	return std::filesystem::path(tPath).replace_extension();
//	//}
//
//}

#endif // WIN32
