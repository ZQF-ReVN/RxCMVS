#include "AutoMem.hpp"


namespace Rut::RxStream
{
	AutoMem::AutoMem() : m_pMemData(nullptr), m_uiMemSize(0), m_uiMaxSize(0)
	{

	}

	AutoMem::AutoMem(const AutoMem& buffer) :AutoMem()
	{ 
		this->Copy(buffer);
	}

	AutoMem::AutoMem(AutoMem&& buffer)noexcept :AutoMem() 
	{ 
		this->Move(buffer);
	}

	AutoMem::~AutoMem()
	{
		if (m_pMemData != nullptr) { delete[] m_pMemData; }
		m_uiMemSize = 0;
		m_uiMaxSize = 0;
		m_pMemData = nullptr;
	}

	AutoMem& AutoMem::Copy(const AutoMem& buffer)
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

	AutoMem& AutoMem::Move(AutoMem& buffer)
	{
		this->m_pMemData = buffer.m_pMemData;
		this->m_uiMemSize = buffer.m_uiMemSize;
		this->m_uiMaxSize = buffer.m_uiMaxSize;

		buffer.m_pMemData = nullptr;
		buffer.m_uiMemSize = 0;
		buffer.m_uiMaxSize = 0;

		return *this;
	}

	AutoMem& AutoMem::Append(AutoMem& rfAutoMem)
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

	AutoMem& AutoMem::operator[] (size_t tSize)
	{
		SetSize(tSize);
		return *this;
	}

	AutoMem& AutoMem::operator=(AutoMem&& rfAutoMem) noexcept
	{
		this->~AutoMem();
		return this->Move(rfAutoMem);
	}

	AutoMem& AutoMem::operator=(const AutoMem& rfAutoMem)
	{
		this->~AutoMem();
		return this->Copy(rfAutoMem);
	}

	uint8_t* AutoMem::GetPtr()
	{
		return m_pMemData;
	}

	size_t AutoMem::GetSize()
	{
		return m_uiMemSize;
	}

	uint8_t* AutoMem::SetSize(size_t uiNewSize, bool isCopy)
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