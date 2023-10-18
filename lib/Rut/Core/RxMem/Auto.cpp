#include "Auto.h"
#include "../../RxFile.h"


namespace Rut::RxMem
{
	Auto::Auto() : m_pMemData(nullptr), m_uiMemSize(0), m_uiMaxSize(0)
	{

	}

	Auto::Auto(const Auto& buffer) :Auto()
	{ 
		this->Copy(buffer);
	}

	Auto::Auto(Auto&& buffer)noexcept :Auto()
	{ 
		this->Move(buffer);
	}

	Auto::Auto(std::string_view msPath) : Auto() 
	{ 
		this->LoadFile(msPath);
	}

	Auto::Auto(std::wstring_view wsPath) : Auto() 
	{
		this->LoadFile(wsPath);
	}

	Auto::Auto(std::string_view msPath, size_t szFile) : Auto() 
	{ 
		this->LoadFile(msPath, szFile);
	}

	Auto::Auto(std::wstring_view wsPath, size_t szFile) : Auto() 
	{ 
		this->LoadFile(wsPath, szFile);
	}

	Auto::~Auto()
	{
		if (m_pMemData != nullptr) { delete[] m_pMemData; }
		m_uiMemSize = 0;
		m_uiMaxSize = 0;
		m_pMemData = nullptr;
	}

	Auto& Auto::Copy(const Auto& buffer)
	{
		if (buffer.m_pMemData != nullptr)
		{
			this->m_uiMaxSize = buffer.m_uiMaxSize;
			this->m_uiMemSize = buffer.m_uiMemSize;
			this->m_pMemData = new uint8_t[buffer.m_uiMemSize];
			memcpy(m_pMemData, buffer.m_pMemData, m_uiMemSize);
		}
		else
		{
			this->m_uiMaxSize = 0;
			this->m_uiMemSize = 0;
			this->m_pMemData = nullptr;
		}

		return *this;
	}

	Auto& Auto::Move(Auto& buffer)
	{
		this->m_pMemData = buffer.m_pMemData;
		this->m_uiMemSize = buffer.m_uiMemSize;
		this->m_uiMaxSize = buffer.m_uiMaxSize;

		buffer.m_pMemData = nullptr;
		buffer.m_uiMemSize = 0;
		buffer.m_uiMaxSize = 0;

		return *this;
	}

	Auto& Auto::Append(Auto& rfAutoMem)
	{
		size_t cur_size = this->GetSize();
		size_t append_size = rfAutoMem.GetSize();
		if (append_size)
		{
			this->SetSize(cur_size + append_size, true);
			memcpy(this->GetPtr() + cur_size, rfAutoMem.GetPtr(), append_size);
		}
		return *this;
	}

	Auto& Auto::operator[] (size_t tSize)
	{
		this->SetSize(tSize);
		return *this;
	}

	Auto& Auto::operator=(Auto&& rfAutoMem) noexcept
	{
		this->~Auto();
		return this->Move(rfAutoMem);
	}

	Auto& Auto::operator=(const Auto& rfAutoMem)
	{
		this->~Auto();
		return this->Copy(rfAutoMem);
	}

	void Auto::SaveData(std::string_view msPath)
	{
		RxFile::SaveFileViaPath(msPath, m_pMemData, m_uiMemSize);
	}

	void Auto::SaveData(std::wstring_view wsPath)
	{
		RxFile::SaveFileViaPath(wsPath, m_pMemData, m_uiMemSize);
	}

	uint8_t* Auto::LoadFile(std::string_view msPath, size_t nSize)
	{
		RxFile::Binary ifs{ msPath, RIO_READ };
		if (nSize == AutoMem_AutoSize) { nSize = ifs.GetSize(); }
		ifs.Read(SetSize(nSize), nSize);
		return m_pMemData;
	}

	uint8_t* Auto::LoadFile(std::wstring_view wsPath, size_t nSize)
	{
		RxFile::Binary ifs{ wsPath, RIO_READ };
		if (nSize == AutoMem_AutoSize) { nSize = ifs.GetSize(); }
		ifs.Read(SetSize(nSize), nSize);
		return m_pMemData;
	}

	uint8_t* Auto::GetPtr()
	{
		return m_pMemData;
	}

	size_t Auto::GetSize()
	{
		return m_uiMemSize;
	}

	uint8_t* Auto::SetSize(size_t uiNewSize, bool isCopy)
	{
		if (this->m_uiMemSize == 0)
		{
			this->m_pMemData = new uint8_t[uiNewSize];
			this->m_uiMemSize = uiNewSize;
			this->m_uiMaxSize = uiNewSize;
		}
		else if (uiNewSize > this->m_uiMaxSize)
		{
			uint8_t* new_mem = new uint8_t[uiNewSize];
			if (isCopy) { memcpy(new_mem, this->m_pMemData, this->m_uiMemSize); }
			delete[] this->m_pMemData;
			this->m_pMemData = new_mem;
			this->m_uiMemSize = uiNewSize;
			this->m_uiMaxSize = uiNewSize;
		}
		else
		{
			this->m_uiMemSize = uiNewSize;
		}

		return GetPtr();
	}
}