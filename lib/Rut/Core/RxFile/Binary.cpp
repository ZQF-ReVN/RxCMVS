#include "Binary.h"
#include "../../RxPath.h"


namespace Rut::RxFile
{
	void SaveFileViaPath(std::wstring_view wsPath, void* pData, size_t nBytes)
	{
		RxPath::MakeDirViaPath(wsPath);
		Binary{ wsPath, RIO_WRITE }.Write(pData, nBytes);
	}

	void SaveFileViaPath(std::string_view msPath, void* pData, size_t nBytes)
	{
		RxPath::MakeDirViaPath(msPath);
		Binary{ msPath, RIO_WRITE }.Write(pData, nBytes);
	}


	Binary::Binary()
	{

	}

	Binary::Binary(std::string_view msPath, size_t nMode)
	{
		this->Create(msPath, nMode);
	}

	Binary::Binary(std::wstring_view wsPath, size_t nMode)
	{
		this->Create(wsPath, nMode);
	}

	Binary::~Binary()
	{
		this->Close();
	}
}