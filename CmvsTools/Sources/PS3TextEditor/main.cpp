#include <iostream>

#include "../../Modules/CmvsTools/PS3Editor.h"
#include "../../ThirdParty/TDA/EnumFiles.h"

using namespace CmvsTools::PS3Editor;


int main()
{
	char flag = 0;
	bool isDoubleLine = FALSE;
	uint32_t insetCodePage = 936;
	uint32_t extractCodePage = 932;
	std::vector<std::wstring> vecFileName;

	std::cout << "Input [ d ] to Extract Text From .ps3 .ps2 Files and Save it as .txt\n";
	std::cout << "Input [ i ] to Insert Text From .txt Files and Save it as .ps3.new\n";
	std::cout << "Input [ t ] to Copy Raw: text to Tra: When Extracting  Text\n";
	std::cout << "Input [ c ] to Set Extract / Inset Sscript CodePage\n" << std::endl;

	while (true)
	{
		std::cout << "Input:";
		std::cin >> flag;
		std::cout << std::endl;

		TDA::EnumFiles::GetAllFileNameW(L"", vecFileName);

		switch (flag)
		{
		case 'd':
			for (std::wstring& fw : vecFileName)
			{

				if (fw.find(L".ps3", fw.length() - 0x4) != std::wstring::npos ||
					fw.find(L".ps2", fw.length() - 0x4) != std::wstring::npos)
				{
					std::wcout << "Dump:" << fw << std::endl;
					PS3Extract dump(fw, extractCodePage, isDoubleLine);
				}
			}
			break;

		case 'i':
			for (std::wstring& fw : vecFileName)
			{
				if (fw.find(L".ps3.txt", fw.length() - 0x9) != std::wstring::npos ||
					fw.find(L".ps2.txt", fw.length() - 0x9) != std::wstring::npos)
				{
					std::wcout << "Inset     :" << fw << std::endl;
					PS3Inset inset(fw, insetCodePage);
				}
			}
			std::vector<std::wstring>().swap(vecFileName);
			break;

		case 't':
			isDoubleLine = TRUE;
			break;

		case 'c':
			std::wcout << L"Set Extract / Inset Sscript CodePage" << std::endl;
			std::wcout << L"Insert CodePage:";
			std::wcin >> insetCodePage;
			std::wcout << L"Set Insert CodePage to " << insetCodePage << std::endl;
			std::wcout << L"Extract CodePage:";
			std::wcin >> extractCodePage;
			std::wcout << L"Set Extract CodePage to " << extractCodePage << std::endl;
			std::cout << std::endl;
			break;
		}
	}


	system("pause");

}