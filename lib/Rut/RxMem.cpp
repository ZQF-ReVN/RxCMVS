#include "RxMem.h"
#include "RxFile.h"

#include <cassert>


namespace Rut::RxMem
{
	Auto::Auto() : m_uiMemSize(0), m_uiMaxSize(0)
	{

	}

	Auto::Auto(std::initializer_list<Auto> lsOBJ) :Auto()
	{
		size_t count_size = 0;
		for (auto& obj : lsOBJ)
		{
			count_size += obj.GetSize();
		}

		this->SetSize(count_size);

		uint8_t* cur_ptr = this->GetPtr();
		for (auto& obj : lsOBJ)
		{
			memcpy(cur_ptr, obj.GetPtr(), obj.GetSize());
			cur_ptr += obj.GetSize();
		}
	}

	Auto::Auto(size_t nSize) :Auto()
	{
		this->SetSize(nSize);
	}

	Auto::Auto(const Auto& buffer) :Auto()
	{
		this->Copy(buffer);
	}

	Auto::Auto(Auto&& buffer) noexcept :Auto()
	{
		this->Move(std::move(buffer));
	}

	Auto::Auto(const std::filesystem::path& phPath, size_t szFile) : Auto()
	{
		this->LoadFile(phPath, szFile);
	}

	Auto::~Auto()
	{
		m_uiMemSize = 0;
		m_uiMaxSize = 0;
	}

	Auto& Auto::Copy(const Auto& buffer)
	{
		assert(this != &buffer);

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

	Auto& Auto::Move(Auto&& buffer) noexcept
	{
		assert(this != &buffer);

		m_upMemData = std::move(buffer.m_upMemData);

		m_uiMemSize = buffer.m_uiMemSize;
		m_uiMaxSize = buffer.m_uiMaxSize;

		buffer.m_uiMemSize = 0;
		buffer.m_uiMaxSize = 0;

		return *this;
	}

	Auto& Auto::Append(const Auto& rfMem)
	{
		size_t cur_size = this->GetSize();
		size_t append_size = rfMem.GetSize();
		if (append_size)
		{
			this->SetSize(cur_size + append_size, true);
			memcpy(this->GetPtr() + cur_size, rfMem.GetPtr(), append_size);
		}
		return *this;
	}

	uint8_t Auto::operator[] (size_t nIndex) noexcept
	{
		assert(nIndex < m_uiMemSize);
		return this->GetPtr()[nIndex];
	}

	Auto& Auto::operator=(Auto&& rfAutoMem) noexcept
	{
		return this->Move(std::move(rfAutoMem));
	}

	Auto& Auto::operator=(const Auto& rfAutoMem)
	{
		return this->Copy(rfAutoMem);
	}

	Auto& Auto::operator+(const Auto& rfAutoMem)
	{
		this->Append(rfAutoMem);
		return *this;
	}

	uint8_t* Auto::begin() const noexcept
	{
		return this->GetPtr();
	}

	uint8_t* Auto::end() const noexcept
	{
		return this->GetPtr() + this->GetSize();
	}

	void Auto::SaveData(const std::filesystem::path& phPath)
	{
		RxFile::SaveFileViaPath(phPath, m_upMemData.get(), m_uiMemSize);
	}


	void Auto::LoadFile(const std::filesystem::path& phPath, size_t nSize)
	{
		RxFile::Binary ifs{ phPath, RIO_READ };
		nSize == AUTO_MEM_AUTO_SIZE ? (void)(nSize = ifs.GetSize()) : (void)(0);
		this->SetSize(nSize);
		ifs.Read(this->GetPtr(), nSize);
	}



	void Auto::SetSize(size_t uiNewSize, bool isCopy)
	{
		if (m_uiMemSize == 0)
		{
			m_upMemData = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);
			m_uiMaxSize = uiNewSize;
		}
		else if (uiNewSize > m_uiMaxSize)
		{
			std::unique_ptr<uint8_t[]> tmp = std::make_unique_for_overwrite<uint8_t[]>(uiNewSize);
			isCopy ? (void)(memcpy(tmp.get(), m_upMemData.get(), m_uiMemSize)) : (void)(0);
			m_upMemData = std::move(tmp);
			m_uiMaxSize = uiNewSize;
		}

		m_uiMemSize = uiNewSize;
	}
}