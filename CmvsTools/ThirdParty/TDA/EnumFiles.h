#pragma once
#include <string>
#include <vector>


namespace TDA
{
	class EnumFiles
	{
	public:
		static bool GetAllFilesPathA(std::string msBasePath, std::vector<std::string>& vecList);
		static bool GetAllFileNameA(std::string msBasePath, std::vector<std::string>& vecList);

		static bool GetAllFilesPathW(std::wstring wsBasePath, std::vector<std::wstring>& vecList);
		static bool GetAllFileNameW(std::wstring wsBasePath, std::vector<std::wstring>& vecList);
	};
}

