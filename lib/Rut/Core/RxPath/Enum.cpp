#include "Enum.h"

#include <queue>
#include <stdexcept>
#include <Windows.h>


namespace Rut::RxPath
{
	static void CheckPath(const std::string_view msPath)
	{
		switch (msPath.back())
		{
		case '\\': break;
		default: throw std::runtime_error("Error Path");
		}
	}

	static void CheckPath(const std::wstring_view wsPath)
	{
		switch (wsPath.back())
		{
		case L'\\': break;
		default: throw std::runtime_error("Error Path");
		}
	}


	bool AllFilePaths(const std::string_view msPath, std::vector<std::string>& vecList)
	{
		CheckPath(msPath);

		std::queue<std::string> dirs;
		WIN32_FIND_DATAA find_data = { 0 };

		dirs.push(msPath.data());

		for (HANDLE hFind = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::string& dirName = dirs.front();

			hFind = FindFirstFileA((dirName + "*").c_str(), &find_data);
			if (hFind == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + "\\");
					continue;
				}

				vecList.emplace_back(dirName + find_data.cFileName);

			} while (FindNextFileA(hFind, &find_data));

			FindClose(hFind);
		}

		return true;
	}

	bool AllFilePaths(const std::wstring_view wsPath, std::vector<std::wstring>& vecList)
	{
		CheckPath(wsPath);

		std::queue<std::wstring> dirs;
		WIN32_FIND_DATAW find_data = { 0 };

		dirs.push(wsPath.data());

		for (HANDLE hfile = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::wstring& dirName = dirs.front();

			hfile = FindFirstFileW((dirName + L"*").c_str(), &find_data);
			if (hfile == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + L"\\");
					continue;
				}

				vecList.emplace_back(dirName + find_data.cFileName);

			} while (FindNextFileW(hfile, &find_data));

			FindClose(hfile);
		}

		return true;
	}

	bool CurFileNames(const std::string_view msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(folder + find_data.cFileName);
			}
			else
			{
				vecList.emplace_back(find_data.cFileName);
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFileNames(const std::wstring_view wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		const std::wstring folder = wsFolder.data();

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((folder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(folder + find_data.cFileName);
			}
			else
			{
				vecList.emplace_back(find_data.cFileName);
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(const std::string_view msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(folder + find_data.cFileName);
				}
				else
				{
					vecList.emplace_back(find_data.cFileName);
				}
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(const std::wstring_view wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		const std::wstring folder = wsFolder.data();

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((folder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(folder + find_data.cFileName);
				}
				else
				{
					vecList.emplace_back(find_data.cFileName);
				}
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}
}
