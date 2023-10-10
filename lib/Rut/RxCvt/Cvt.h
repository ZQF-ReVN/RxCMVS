#pragma once
#include <string>


namespace Rut::RxCvt
{
	std::wstring ToWCS(const std::string_view msStr, size_t uCodePage);
	size_t ToWCS(const std::string_view msStr, std::wstring& wsStr, size_t uCodePage);

	std::string ToMBCS(const std::wstring_view wsStr, size_t uCodePage);
	size_t ToMBCS(const std::wstring_view wsStr, std::string& msStr, size_t uCodePage);
}
