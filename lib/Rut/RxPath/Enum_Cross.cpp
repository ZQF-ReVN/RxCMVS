#ifndef WIN32

#include "Enum.h"

#include <queue>
#include <filesystem>


namespace Rut::RxPath::Std
{
	bool AllFilePaths(const std::string_view msBasePath, std::vector<std::string>& vecList)
	{
		std::queue<std::filesystem::path> folders;
		folders.push(msBasePath);

		for (; !folders.empty(); folders.pop())
		{
			for (auto& entry : std::filesystem::directory_iterator(folders.front()))
			{
				if (entry.is_regular_file())
				{
					vecList.push_back(std::move(entry.path().string()));
					continue;
				}

				if (entry.is_directory())
				{
					folders.push(entry.path());
					continue;
				}
			}
		}

		return true;
	}

	bool AllFilePaths(const std::wstring_view msBasePath, std::vector<std::wstring>& vecList)
	{
		std::queue<std::filesystem::path> folders;
		folders.push(msBasePath);

		for (; !folders.empty(); folders.pop())
		{
			for (auto& entry : std::filesystem::directory_iterator(folders.front()))
			{
				if (entry.is_regular_file())
				{
					vecList.push_back(std::move(entry.path().wstring()));
					continue;
				}

				if (entry.is_directory())
				{
					folders.push(entry.path());
				}
			}
		}

		return true;
	}

	bool CurFileNames(const std::string_view wsBasePath, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		for (auto& entry : std::filesystem::directory_iterator(wsBasePath))
		{
			if (entry.is_regular_file() == false) { continue; }

			if (isAddBasePath)
			{
				vecList.emplace_back(std::move(entry.path().string()));
			}
			else
			{
				vecList.emplace_back(std::move(entry.path().filename().string()));
			}
		}

		return true;
	}

	bool CurFileNames(const std::wstring_view wsBasePath, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		for (auto& entry : std::filesystem::directory_iterator(wsBasePath))
		{
			if (entry.is_regular_file() == false) { continue; }

			if (isAddBasePath)
			{
				vecList.emplace_back(std::move(entry.path().wstring()));
			}
			else
			{
				vecList.emplace_back(std::move(entry.path().filename().wstring()));
			}
		}

		return true;
	}

	bool CurFolderNames(const std::string_view msBasePath, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		for (auto& entry : std::filesystem::directory_iterator(msBasePath))
		{
			if (entry.is_directory() == false) { continue; }

			if (isAddBasePath)
			{
				vecList.emplace_back(std::move(entry.path().string()));
			}
			else
			{
				vecList.emplace_back(std::move(entry.path().filename().string()));
			}
		}

		return true;
	}

	bool CurFolderNames(const std::wstring_view wsBasePath, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		for (auto& entry : std::filesystem::directory_iterator(wsBasePath))
		{
			if (entry.is_directory()) { continue; }

			if (isAddBasePath)
			{
				vecList.emplace_back(std::move(entry.path().wstring()));
			}
			else
			{
				vecList.emplace_back(std::move(entry.path().filename().wstring()));
			}
		}

		return true;
	}
}

#endif // WIN32