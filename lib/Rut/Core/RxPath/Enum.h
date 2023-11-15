#pragma once
#include <string>
#include <vector>


namespace Rut::RxPath
{
	bool AllFilePaths(std::string msBasePath, std::vector<std::string>& vecList);
	bool AllFilePaths(std::wstring wsBasePath, std::vector<std::wstring>& vecList);

	bool CurFileNames(std::string msBasePath, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFileNames(std::wstring wsBasePath, std::vector<std::wstring>& vecList, bool isAddBasePath = false);

	bool CurFolderNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath = false);
	bool CurFolderNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath = false);
}
