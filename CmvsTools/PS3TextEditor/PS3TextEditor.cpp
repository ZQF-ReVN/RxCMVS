#include "PS3TextEditor.h"
#include "../TDA/StringX.h"

PS3TextEditor::PS3TextEditor() :
	m_Header{ 0 }, m_PS3Info{ 0 }, m_abFlagPushStr{ 0x01,0x02,0x20,0x01 }
{

}

PS3TextEditor::~PS3TextEditor()
{

}


PS3TextDump::PS3TextDump(std::wstring& wsPath, uint32_t dwCodePage, bool isFullText) :
	m_isFullText(isFullText), m_dwCodePage(dwCodePage), m_wsPath(wsPath), m_fpTextFile(0)
{
	if (GetPS3FileInfo())
	{
		SearchOffset();
		if (CreateDumpFile())
		{
			DumpText();
			std::wcout << "Save:" << m_wsPath << ".txt\n" << std::endl;
		}
		else
		{
			std::wcout << "Create DumpFile Failed!!!\n" << std::endl;
		}
	}
	else
	{
		std::wcout << "Read PS3File Failed!!!\n" << std::endl;
	}

}

PS3TextDump::~PS3TextDump()
{
	VirtualFree((LPVOID)m_PS3Info.pPS3File, NULL, MEM_RELEASE);
	fclose(m_fpTextFile);
}

bool PS3TextDump::GetPS3FileInfo()
{
	HANDLE hFile = CreateFileW(m_wsPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	else
	{
		m_PS3Info.pPS3File = (uint32_t)VirtualAlloc(NULL, GetFileSize(hFile, NULL), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (m_PS3Info.pPS3File != NULL)
		{
			if (ReadFile(hFile, (PBYTE)m_PS3Info.pPS3File, GetFileSize(hFile, NULL), NULL, NULL))
			{
				memcpy(&m_Header, (void*)m_PS3Info.pPS3File, sizeof(m_Header));
				m_PS3Info.pCodeBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen;
				m_PS3Info.pTextBlock = m_PS3Info.pPS3File + m_Header.dwHeaderLen + (4 * m_Header.dwTextCount) + m_Header.dwCodeBlockLen;
				//Fix Len
				m_PS3Info.dwCodeBlockLen = m_PS3Info.pTextBlock - m_PS3Info.pPS3File;
				CloseHandle(hFile);
				return TRUE;
			}
			else
			{
				CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
}

void PS3TextDump::SearchOffset()
{
	for (size_t i = 0; i < m_PS3Info.dwCodeBlockLen; i++)
	{
		if (!memcmp(m_abFlagPushStr, (void*)(m_PS3Info.pCodeBlock + i), sizeof(m_abFlagPushStr)))
		{
			m_vecppStr.push_back(m_PS3Info.pCodeBlock + i + sizeof(m_abFlagPushStr));
		}
	}
}

bool PS3TextDump::CreateDumpFile()
{
	std::wstring dumpFileName = m_wsPath + L".txt";
	errno_t err = _wfopen_s(&m_fpTextFile, dumpFileName.c_str(), L"rt");
	if (!err && m_fpTextFile)
	{
		char flag = 0;
		std::wcout << L"The text file already exist and need to be overwritten?(Y/N)";
		std::cin >> flag;
		if (flag == 'N')
		{
			return FALSE;
		}
		fclose(m_fpTextFile);
	}
	err = _wfopen_s(&m_fpTextFile, dumpFileName.c_str(), L"wt+,ccs=UTF-16LE");
	return ~err;
}

void PS3TextDump::DumpText()
{
	uint32_t strAddr = 0;
	uint32_t textFOA = 0;
	uint32_t codeFOA = 0;
	std::string msText;
	std::wstring wsText;

	for (auto& pStr : m_vecppStr)
	{
		strAddr = m_PS3Info.pTextBlock + *(uint32_t*)pStr;
		msText = (char*)strAddr;

		//Filter Files Name String Line
		if (msText.empty() ||
			msText.find(".ogg") != std::string::npos ||
			msText.find(".wav") != std::string::npos ||
			msText.find(".mv2") != std::string::npos ||
			msText.find(".pb3") != std::string::npos ||
			msText.find(".pb2") != std::string::npos ||
			msText.find(".ps3") != std::string::npos ||
			msText.find(".ps2") != std::string::npos ||
			msText.find(".cur") != std::string::npos ||
			msText.find(".cmv") != std::string::npos)
		{
			continue;
		}
		else
		{
			textFOA = strAddr - m_PS3Info.pPS3File;
			codeFOA = pStr - m_PS3Info.pPS3File;
			TDA::StringX::StrToWStr(msText, wsText, m_dwCodePage);

			if (!m_isFullText)
			{
				fwprintf_s(m_fpTextFile, L"[Text:0x%08X Code:0x%08X]\nRaw:%s\nTra:\n\n", textFOA, codeFOA, wsText.c_str());
			}
			else
			{
				fwprintf_s(m_fpTextFile, L"[Text:0x%08X Code:0x%08X]\nRaw:%s\nTra:%s\n\n", textFOA, codeFOA, wsText.c_str(), wsText.c_str());
			}

			fflush(m_fpTextFile);
		}
	}
}


PS3TextInset::PS3TextInset(std::wstring& wsPath, uint32_t dwCodePage) :
	m_dwCodePage(dwCodePage), m_wsTextPath(wsPath), m_countInset(0), m_fpPS3File(0)
{
	if (GetPS3FileInfo())
	{
		if (InsetTextFile())
		{
			std::wcout << "Save      :" << m_wsPS3FilePath << L".new" << std::endl;
			std::wcout << "InsetCount:" << m_countInset << '\n' << std::endl;
		}
		else
		{
			std::wcout << "Inset Text Failed!!!\n" << std::endl;
		}
	}
	else
	{
		std::wcout << "Read PS3File Failed!!!\n" << std::endl;
	}

}

PS3TextInset::~PS3TextInset()
{
	fclose(m_fpPS3File);
}

bool PS3TextInset::GetPS3FileInfo()
{
	m_wsPS3FilePath = m_wsTextPath;
	m_wsPS3FilePath.erase(m_wsPS3FilePath.length() - 4, 4);//delete end of path string .txt
	CopyFileW(m_wsPS3FilePath.c_str(), (m_wsPS3FilePath + L".new").c_str(), FALSE);

	errno_t err = _wfopen_s(&m_fpPS3File, (m_wsPS3FilePath + L".new").c_str(), L"rb+");
	if (!err && m_fpPS3File)
	{
		fread_s((void*)&m_Header, 0x30, 1, 0x30, m_fpPS3File);
		fflush(m_fpPS3File);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

bool PS3TextInset::InsetTextFile()
{
	FILE* fpTextFile = 0;
	uint32_t strLen = 0;
	uint32_t codeOffset = 0;
	std::string msText;
	std::wstring wsText;
	wchar_t newText[0xFF] = { 0 };

	errno_t err = _wfopen_s(&fpTextFile, m_wsTextPath.c_str(), L"rt+,ccs=UTF-16LE");
	if (!err && fpTextFile != NULL)
	{
		while (feof(fpTextFile) == 0)
		{
			if (fwscanf_s(fpTextFile, L"[Text:0x%*x Code:0x%x]\n\r", &codeOffset))
			{
				continue;
			}

			fwscanf_s(fpTextFile, L"Raw:%*[^\n]\r");

			if (!fwscanf_s(fpTextFile, L"Tra:%[^\n]\r", newText, sizeof(newText)))
			{
				//If Tra: is null
				fwscanf_s(fpTextFile, L"\n\r\n\r");
				continue;
			}

			//Append text at the end of the file
			wsText = newText;
			TDA::StringX::WStrToStr(wsText, msText, m_dwCodePage);

			strLen = msText.length() + 1;

			fseek(m_fpPS3File, 0, SEEK_END);
			fwrite(msText.c_str(), sizeof(byte), strLen, m_fpPS3File);

			//Modify the codeblock PushStr address
			fseek(m_fpPS3File, codeOffset, SEEK_SET);
			fwrite(&m_Header.dwTextBlockLen, sizeof(byte), sizeof(m_Header.dwTextBlockLen), m_fpPS3File);

			//Fix the textblock size
			m_Header.dwTextBlockLen += strLen;
			fflush(m_fpPS3File);

			//Count the number of inserted texts
			m_countInset++;
		}

		if (m_countInset)
		{
			//Write back to .ps3 file header
			fseek(m_fpPS3File, 0, SEEK_SET);
			fwrite(&m_Header, sizeof(byte), sizeof(m_Header), m_fpPS3File);
			fflush(m_fpPS3File);
			fclose(fpTextFile);

			return TRUE;
		}

		fclose(fpTextFile);
		return FALSE;
	}

	return FALSE;
}

