#include "Path.h"


namespace Rut::RxPath
{
	size_t FileName(char* cpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (cpPath[iteChar] != '\\' && cpPath[iteChar] != '/')
			{
				continue;
			}

			if (cpPath[iteChar - 1] <= 0x7F)
			{
				size_t name_len = nChar - iteChar - 1;
				memcpy(cpPath, cpPath + iteChar + 1, name_len);
				cpPath[name_len] = '\0';
				return name_len;
			}
		}

		return nChar;
	}

	size_t FileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t iteChar = nChar - 1; iteChar > 0; iteChar--)
		{
			if (wpPath[iteChar] != L'\\' && wpPath[iteChar] != L'/')
			{
				continue;
			}

			size_t name_len = nChar - iteChar - 1;
			memcpy(wpPath, wpPath + iteChar + 1, name_len * 2);
			wpPath[name_len] = L'\0';
			return name_len;
		}

		return nChar;
	}

	void FileNameRef(std::string& msPath)
	{
		msPath.resize(FileName(msPath.data(), msPath.length()));
	}

	void FileNameRef(std::wstring& wsPath)
	{
		wsPath.resize(FileName(wsPath.data(), wsPath.length()));
	}

	std::string FileName(std::string msPath)
	{
		FileNameRef(msPath);
		return msPath;
	}

	std::wstring FileName(std::wstring wsPath)
	{
		FileNameRef(wsPath);
		return wsPath;
	}


	size_t NotFileName(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '\\')
			{
				if (cpPath[ite_char - 1] <= 0x7F)
				{
					cpPath[ite_char + 1] = '\0';
					return ite_char + 1;
				}
			}

			if (cpPath[ite_char] == '/')
			{
				cpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	size_t NotFileName(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'\\')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}

			if (wpPath[ite_char] == L'/')
			{
				wpPath[ite_char + 1] = L'\0';
				return ite_char + 1;
			}
		}

		return nChar;
	}

	void NotFileNameRef(std::string& msPath)
	{
		msPath.resize(NotFileName(msPath.data(), msPath.length()));
	}

	void NotFileNameRef(std::wstring& wsPath)
	{
		wsPath.resize(NotFileName(wsPath.data(), wsPath.length()));
	}

	std::string NotFileName(std::string msPath)
	{
		NotFileNameRef(msPath);
		return msPath;
	}

	std::wstring NotFileName(std::wstring msPath)
	{
		NotFileNameRef(msPath);
		return msPath;
	}


	size_t Suffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(cpPath, cpPath + ite_char, suffix_len);
				return ite_char;
			}
		}

		return nChar;
	}

	size_t Suffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				size_t suffix_len = nChar - ite_char + 1;
				memcpy(wpPath, wpPath + ite_char, suffix_len * sizeof(wchar_t));
				return ite_char;
			}
		}

		return nChar;
	}

	void SuffixRef(std::string& msPath)
	{
		msPath.resize(Suffix(msPath.data(), msPath.length()));
	}

	void SuffixRef(std::wstring& wsPath)
	{
		wsPath.resize(Suffix(wsPath.data(), wsPath.length()));
	}

	std::string Suffix(std::string msPath)
	{
		SuffixRef(msPath);
		return msPath;
	}

	std::wstring Suffix(std::wstring wsPath)
	{
		SuffixRef(wsPath);
		return wsPath;
	}


	size_t NotSuffix(char* cpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (cpPath[ite_char] == '.')
			{
				cpPath[ite_char] = '\0';
				return ite_char;
			}
		}

		return nChar;
	}

	size_t NotSuffix(wchar_t* wpPath, size_t nChar)
	{
		for (size_t ite_char = nChar - 1; ite_char > 0; ite_char--)
		{
			if (wpPath[ite_char] == L'.')
			{
				wpPath[ite_char] = L'\0';
				return ite_char;
			}
		}

		return nChar;
	}

	void NotSuffixRef(std::string& msPath)
	{
		msPath.resize(NotSuffix(msPath.data(), msPath.length()));
	}

	void NotSuffixRef(std::wstring& wsPath)
	{
		wsPath.resize(NotSuffix(wsPath.data(), wsPath.length()));
	}

	std::string NotSuffix(std::string msPath)
	{
		NotSuffixRef(msPath);
		return msPath;
	}

	std::wstring NotSuffix(std::wstring wsPath)
	{
		NotSuffixRef(wsPath);
		return wsPath;
	}


	char* FormatSlash(char* cpPath, char cSlash)
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

	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash)
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

	void FormatSlashRef(std::string& msPath, char cSlash)
	{
		FormatSlash(msPath.data(), cSlash);
	}

	void FormatSlashRef(std::wstring& wsPath, wchar_t wcSlash)
	{
		FormatSlash(wsPath.data(), wcSlash);
	}

	std::string FormatSlash(std::string msPath, char cSlash)
	{
		FormatSlashRef(msPath, cSlash);
		return msPath;
	}

	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash)
	{
		FormatSlashRef(wsPath, wcSlash);
		return wsPath;
	}
}
