#include "RxPath.h"
#include "RxStr.h"
#include "RxFile.h"
#include "RxSys.h"

#include <cassert>
#include <stdexcept>


namespace Rut::RxPath
{
	size_t GetFileName(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;
		size_t file_name_len = 0;
		char* file_name_ptr = nullptr;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '/' || tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				file_name_len = nBytes - (ite_unit + 1);
				file_name_ptr = cpPath + (ite_unit + 1);
				break;
			}
		} while ((--ite_unit) != -1);

		if (file_name_ptr)
		{
			file_name_len ? (void)(memcpy(cpPath, file_name_ptr, file_name_len * sizeof(char))) : (void)(0);
			cpPath[file_name_len] = '\0';
		}
		else
		{
			file_name_len = nBytes;
		}

		return file_name_len;
	}

	size_t GetFileName(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;
		size_t file_name_len = 0;
		wchar_t* file_name_ptr = nullptr;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'/' || tmp_unit == L'\\')
			{
				file_name_len = nChar - (ite_unit + 1);
				file_name_ptr = wpPath + (ite_unit + 1);
				break;
			}
		} while ((--ite_unit) != -1);

		if (file_name_ptr)
		{
			file_name_len ? (void)(memcpy(wpPath, file_name_ptr, file_name_len * sizeof(wchar_t))) : (void)(0);
			wpPath[file_name_len] = L'\0';
		}
		else
		{
			file_name_len = nChar;
		}

		return file_name_len;
	}

	std::string GetFileName(std::string msPath)
	{
		size_t bytes = GetFileName(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring GetFileName(std::wstring wsPath)
	{
		size_t chars = GetFileName(wsPath.data(), wsPath.size());
		wsPath.resize(chars);
		return wsPath;
	}


	size_t RemoveFileName(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '/' || tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				cpPath[ite_unit + 1] = '\0';
				return ite_unit + 1;
			}
		} while ((--ite_unit) != -1);

		return nBytes;
	}

	size_t RemoveFileName(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'/' || tmp_unit == L'\\')
			{
				wpPath[ite_unit + 1] = L'\0';
				return ite_unit + 1;
			}
		} while ((--ite_unit) != -1);

		return nChar;
	}

	std::string RemoveFileName(std::string msPath)
	{
		size_t bytes = RemoveFileName(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring RemoveFileName(std::wstring msPath)
	{
		size_t chars = RemoveFileName(msPath.data(), msPath.size());
		msPath.resize(chars);
		return msPath;
	}


	// nBytes exclude null char
	size_t GetSuffix(char* cpPath, const size_t nBytes)
	{
		size_t ite_byte = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_byte];

			if (tmp_unit == '.')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				size_t suffix_bytes = nBytes - ite_byte;
				memcpy(cpPath, cpPath + ite_byte, suffix_bytes * sizeof(char));
				cpPath[suffix_bytes] = '\0';
				return suffix_bytes;
			}
			else if (tmp_unit == '/')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
			else if (tmp_unit == '\\')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
		} while ((--ite_byte) != -1);

		cpPath[0] = '\0';
		return 0;
	}

	// nChar exclude null char
	size_t GetSuffix(wchar_t* wpPath, const size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'.')
			{
				size_t suffix_bytes = nChar - ite_unit;
				memcpy(wpPath, wpPath + ite_unit, suffix_bytes * sizeof(wchar_t));
				wpPath[suffix_bytes] = L'\0';
				return suffix_bytes;
			}
			else if (tmp_unit == L'/')
			{
				break;
			}
			else if (tmp_unit == L'\\')
			{
				break;
			}
		} while ((--ite_unit) != -1);

		wpPath[0] = L'\0';
		return 0;
	}

	std::string GetSuffix(std::string msPath)
	{
		size_t suffix_bytes = GetSuffix(msPath.data(), msPath.size());
		msPath.resize(suffix_bytes);
		return msPath;
	}

	std::wstring GetSuffix(std::wstring wsPath)
	{
		size_t suffix_chars = GetSuffix(wsPath.data(), wsPath.size());
		wsPath.resize(suffix_chars);
		return wsPath;
	}


	// nBytes exclude null char
	size_t RemoveSuffix(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '.')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				cpPath[ite_unit] = '\0';
				return ite_unit;
			}
			else if (tmp_unit == '/')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
			else if (tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
		} while ((--ite_unit) != -1);

		return nBytes;
	}

	// nChar exclude null char
	size_t RemoveSuffix(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'.')
			{
				wpPath[ite_unit] = L'\0';
				return ite_unit;
			}
			else if (tmp_unit == L'/')
			{
				break;
			}
			else if (tmp_unit == L'\\')
			{
				break;
			}
		} while ((--ite_unit) != -1);

		return nChar;
	}

	std::string RemoveSuffix(std::string msPath)
	{
		size_t bytes = RemoveSuffix(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring RemoveSuffix(std::wstring wsPath)
	{
		size_t chars = RemoveSuffix(wsPath.data(), wsPath.size());
		wsPath.resize(chars);
		return wsPath;
	}


	char* Format(char* cpPath, char cSlash)
	{
		switch (cSlash)
		{
		case '\\':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if ((uint8_t)cpPath[ite] > 0x7F) { ite++; continue; }

				if (cpPath[ite] == '/') { cpPath[ite] = '\\'; }
			}
		}
		break;

		case '/':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if (cpPath[ite] == '\\') { cpPath[ite] = '/'; }
			}
		}
		break;
		}

		return cpPath;
	}

	wchar_t* Format(wchar_t* wpPath, wchar_t wcSlash)
	{
		switch (wcSlash)
		{
		case L'\\':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'/') { wpPath[ite] = L'\\'; }
			}
		}
		break;

		case L'/':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'\\') { wpPath[ite] = L'/'; }
			}
		}
		break;
		}

		return wpPath;
	}
}

namespace Rut::RxPath
{
	static bool MakeDirViaPath(const char* cpPath)
	{
		const size_t len = RxSys::StrLen(cpPath);
		assert(len <= RxSys::PLATFORM_MAX_PATH);

		char path[RxSys::PLATFORM_MAX_PATH];
		RxSys::StrCpy(path, RxSys::PLATFORM_MAX_PATH, cpPath);

		for (size_t ite_unit = 0; ite_unit < len; ite_unit++)
		{
			switch (path[ite_unit])
			{
			case '/':
			case '\\':
			{
				if ((ite_unit > 0) && ((uint8_t)path[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				path[ite_unit] = '\0';
				RxSys::MakeDir(path);
				path[ite_unit] = '\\';
			}
			break;

			case '.':
			case ':': { ite_unit++; } break;
			}
		}

		return true;
	}

	static bool MakeDirViaPath(const wchar_t* wpPath)
	{
		const size_t len = RxSys::StrLen(wpPath);
		assert(len <= RxSys::PLATFORM_MAX_PATH);

		wchar_t path[RxSys::PLATFORM_MAX_PATH];
		RxSys::StrCpy(path, RxSys::PLATFORM_MAX_PATH, wpPath);

		for (size_t ite_char = 0; ite_char < len; ite_char++)
		{
			switch (path[ite_char])
			{
			case L'/':
			case L'\\':
			{
				path[ite_char] = L'\0';
				RxSys::MakeDir(path);
				path[ite_char] = L'\\';
			}
			break;

			case L'.':
			case L':': { ite_char++; } break;
			}
		}

		return true;
	}

	bool MakeDirViaPath(std::string_view msPath)
	{
		return MakeDirViaPath(msPath.data());
	}

	bool MakeDirViaPath(std::wstring_view wsPath)
	{
		return MakeDirViaPath(wsPath.data());
	}

	std::string ModulePathA(void* pBase)
	{
		std::string path;
		path.resize(RxSys::PLATFORM_MAX_PATH);
		path.resize(RxSys::GetModulePath(path.data(), RxSys::PLATFORM_MAX_PATH, pBase));
		return path;
	}

	std::wstring ModulePathW(void* pBase)
	{
		std::wstring path;
		path.resize(RxSys::PLATFORM_MAX_PATH);
		path.resize(RxSys::GetModulePath(path.data(), RxSys::PLATFORM_MAX_PATH, pBase));
		return path;
	}


	std::string ModuleNameA(void* pBase)
	{
		std::string name = ModulePathA(pBase);
		size_t bytes = GetFileName(name.data(), name.size());
		name.resize(bytes);
		return name;
	}

	std::wstring ModuleNameW(void* pBase)
	{
		std::wstring name = ModulePathW(pBase);
		size_t len = GetFileName(name.data(), name.size());
		name.resize(len);
		return name;
	}


	std::string ModuleDirA()
	{
		std::string path;
		path.resize(RxSys::PLATFORM_MAX_PATH);
		path.resize(RxSys::GetModuleDir(path.data(), RxSys::PLATFORM_MAX_PATH));
		return path;
	}

	std::wstring ModuleDirW()
	{
		std::wstring path;
		path.resize(RxSys::PLATFORM_MAX_PATH);
		path.resize(RxSys::GetModuleDir(path.data(), RxSys::PLATFORM_MAX_PATH));
		return path;
	}

}
