#pragma once
#include <string>
#include <filesystem>


namespace CMVS::MGV
{
	class Editor
	{
	public:
		static void Extract(const std::filesystem::path& phMGV);
		static void Replace(const std::filesystem::path& phMGV, const std::filesystem::path& phVideo);
	};
}

