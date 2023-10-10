#pragma once
#include <string>
#include <vector>


namespace Rut::RxPath
{
	bool AllFilePaths(const std::string_view msBasePath, std::vector<std::string>& vecList);
	bool AllFilePaths(const std::wstring_view wsBasePath, std::vector<std::wstring>& vecList);

	bool CurFileNames(const std::string_view msBasePath, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFileNames(const std::wstring_view wsBasePath, std::vector<std::wstring>& vecList, bool isAddBasePath = false);

	bool CurFolderNames(const std::string_view msFolder, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFolderNames(const std::wstring_view wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath = false);
}
