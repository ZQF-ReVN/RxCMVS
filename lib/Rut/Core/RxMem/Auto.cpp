#include "Auto.h"
#include "../../RxFile.h"


namespace Rut::RxMem
{
	Auto::Auto() : m_uiMemSize(0), m_uiMaxSize(0)
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
		m_uiMemSize = 0;
		m_uiMaxSize = 0;
	}

	Auto& Auto::Copy(const Auto& buffer)
	{
		if (buffer.m_upMemData != nullptr)
		{
			this->m_uiMaxSize = buffer.m_uiMaxSize;
			this->m_uiMemSize = buffer.m_uiMemSize;
			this->m_upMemData = std::make_unique_for_overwrite<uint8_t[]>(buffer.m_uiMemSize);
			memcpy(m_upMemData.get(), buffer.m_upMemData.get(), m_uiMemSize);
		}
		else
		{
			this->m_uiMaxSize = 0;
			this->m_uiMemSize = 0;
		}

		return *this;
	}

	Auto& Auto::Move(Auto& buffer)
	{
		this->m_upMemData = std::move(buffer.m_upMemData);

		this->m_uiMemSize = buffer.m_uiMemSize;
		this->m_uiMaxSize = buffer.m_uiMaxSize;
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
		RxFile::SaveFileViaPath(msPath, m_upMemData.get(), m_uiMemSize);
	}

	void Auto::SaveData(std::wstring_view wsPath)
	{
		RxFile::SaveFileViaPath(wsPath, m_upMemData.get(), m_uiMemSize);
	}

	uint8_t* Auto::LoadFile(std::string_view msPath, size_t nSize)
	{
		RxFile::Binary ifs{ msPath, RIO_READ };
		if (nSize == AutoMem_AutoSize) { nSize = ifs.GetSize(); }
		ifs.Read(SetSize(nSize), nSize);
		return m_upMemData.get();
	}

	uint8_t* Auto::LoadFile(std::wstring_view wsPath, size_t nSize)
	{
		RxFile::Binary ifs{ wsPath, RIO_READ };
		if (nSize == AutoMem_AutoSize) { nSize = ifs.GetSize(); }
		ifs.Read(SetSize(nSize), nSize);
		return m_upMemData.get();
	}

	uint8_t* Auto::GetPtr()
	{
		return m_upMemData.get();
	}

	size_t Auto::GetSize()
	{
		return m_uiMemSize;
	}

	uint8_t* Auto::SetSize(size_t uiNewSize, bool isCopy)
	{
		if (this->m_uiMemSize == 0)
		{
			this->m_upMemData = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);
			this->m_uiMemSize = uiNewSize;
			this->m_uiMaxSize = uiNewSize;
		}
		else if (uiNewSize > this->m_uiMaxSize)
		{
			std::unique_ptr<uint8_t[]> tmp = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);

			if (isCopy) { memcpy(tmp.get(), this->m_upMemData.get(), this->m_uiMemSize); }
			m_upMemData.release();

			this->m_upMemData = std::move(tmp);
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