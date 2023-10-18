#include "Basic.h"
#include "../../Platform/Platform.h"

#include <stdexcept>


namespace Rut::RxFile
{
	Basic::Basic() : m_hFile(nullptr)
	{

	}

	Basic::Basic(Basic&& refStream) noexcept
	{
		this->m_hFile = refStream.m_hFile;
		refStream.m_hFile = nullptr;
	}

	Basic::~Basic()
	{
		Close();
	}


	void Basic::Create(std::string_view msPath, size_t nMode)
	{
		m_hFile = Platform::FileOpen(msPath.data(), nMode);
		if (m_hFile == nullptr) { throw std::runtime_error("Create File Error!"); }
	}

	void Basic::Create(std::wstring_view wsPath, size_t nMode)
	{
		m_hFile = Platform::FileOpen(wsPath.data(), nMode);
		if (m_hFile == nullptr) { throw std::runtime_error("Create File Error!"); }
	}


	bool Basic::Close()
	{
		bool is_close = Platform::FileClose(m_hFile);
		m_hFile = nullptr;
		return is_close;
	}

	bool Basic::Flush()
	{
		return Platform::FileFlush(m_hFile);
	}

	bool Basic::IsEnd()
	{
		size_t file_size = GetSize();
		size_t current_pointer = GetPos();
		return current_pointer >= file_size;
	}

	size_t Basic::SetPos(size_t nOffset, size_t nWay)
	{
		return Platform::FileSetPtr(m_hFile, nOffset, nWay);
	}

	size_t Basic::GetPos()
	{
		return Platform::FileGetPtr(m_hFile);
	}

	size_t Basic::GetSize()
	{
		return Platform::FileGetSize(m_hFile);
	}

	size_t Basic::Read(void* pBuffer, size_t nSize)
	{
		return Platform::FileRead(m_hFile, pBuffer, nSize);
	}

	size_t Basic::Write(void* pData, size_t nSize)
	{
		return Platform::FileWrite(m_hFile, pData, nSize);
	}
}
