#include "Enum.h"
#include "Path.h"

#include <queue>
#include <stdexcept>
#include <Windows.h>


namespace Rut::RxPath
{
	void CheckPath(std::string& msPath)
	{
		FormatSlash(msPath, '\\');
		if (msPath.back() != '\\') { msPath.append(1, '\\'); }
	}

	void CheckPath(std::wstring& wsPath)
	{
		FormatSlash(wsPath, L'\\');
		if (wsPath.back() != L'\\') { wsPath.append(1, L'\\'); }
	}

	bool AllFilePaths(std::string msPath, std::vector<std::string>& vecList)
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

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileA(hFind, &find_data));

			FindClose(hFind);
		}

		return true;
	}

	bool AllFilePaths(std::wstring wsPath, std::vector<std::wstring>& vecList)
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

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileW(hfile, &find_data));

			FindClose(hfile);
		}

		return true;
	}

	bool CurFileNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
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
				vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFileNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
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
				vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
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
					vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
				}
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((wsFolder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
				}
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}
}
