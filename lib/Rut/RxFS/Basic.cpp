#include "Basic.h"


namespace Rut::RxFS
{
	Basic::Basic() : m_hFile(INVALID_HANDLE_VALUE)
	{

	}

	Basic::Basic(Basic&& refStream) noexcept
	{
		this->m_hFile = refStream.m_hFile;
		refStream.m_hFile = INVALID_HANDLE_VALUE;
	}

	Basic::~Basic()
	{
		Close();
	}

	static DWORD GetAccess(RIO emAccess)
	{
		DWORD access_flag = 0;

		switch (emAccess)
		{
		case RIO::RIO_IN: access_flag = GENERIC_READ; break;
		case RIO::RIO_OUT: access_flag = GENERIC_WRITE; break;
		case RIO::RIO_IN_OUT: access_flag = GENERIC_READ | GENERIC_WRITE; break;
		}

		return access_flag;
	}

	static DWORD GetCreate(RIO emAccess, RCO emCreate)
	{
		DWORD create_flag = 0;

		switch (emCreate)
		{
		case RCO::RCO_OPEN: create_flag = OPEN_EXISTING; break;
		case RCO::RCO_CREATE: create_flag = CREATE_ALWAYS; break;
		case RCO::RCO_OPEN_UNSAFE: create_flag = OPEN_ALWAYS; break;
		case RCO::RCO_CREATE_SAFE: create_flag = CREATE_NEW; break;
		case RCO::RCO_AUTO:
		{
			switch (emAccess)
			{
			case RIO::RIO_IN: create_flag = OPEN_EXISTING; break;
			case RIO::RIO_OUT: create_flag = CREATE_ALWAYS; break;
			case RIO::RIO_IN_OUT: create_flag = OPEN_EXISTING; break;
			}
		}
		break;
		}

		return create_flag;
	}

	void Basic::Create(const char* cpPath, RIO emAccess, RCO emCreate)
	{
		m_hFile = ::CreateFileA(cpPath, GetAccess(emAccess), FILE_SHARE_READ, NULL, GetCreate(emAccess, emCreate), FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void Basic::Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate)
	{
		m_hFile = ::CreateFileW(wpPath, GetAccess(emAccess), FILE_SHARE_READ, NULL, GetCreate(emAccess, emCreate), FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void Basic::Create(std::string_view msPath, RIO emAccess, RCO emCreate)
	{
		Create(msPath.data(), emAccess, emCreate);
	}

	void Basic::Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate)
	{
		Create(wsPath.data(), emAccess, emCreate);
	}


	bool Basic::Close()
	{
		bool is_close = ::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return is_close;
	}

	bool Basic::Flush()
	{
		return FlushFileBuffers(m_hFile);
	}

	bool Basic::IsEnd()
	{
		size_t file_size = GetSize();
		size_t current_pointer = GetPos();
		return current_pointer >= file_size;
	}

	void Basic::SetEnd()
	{
		::SetFilePointer(m_hFile, 0, NULL, FILE_END);
	}

	size_t Basic::SetPos(size_t nOffset)
	{
		return (size_t)::SetFilePointer(m_hFile, (LONG)nOffset, NULL, FILE_BEGIN);
	}

	size_t Basic::GetPos()
	{
		return ::SetFilePointer(m_hFile, NULL, NULL, FILE_CURRENT);
	}

	size_t Basic::MovePos(size_t nDistance)
	{
		return (size_t)::SetFilePointer(m_hFile, (LONG)nDistance, NULL, FILE_CURRENT);
	}

	size_t Basic::GetSize(size_t* pHigh)
	{
		return (size_t)::GetFileSize(m_hFile, (LPDWORD)pHigh);
	}


	size_t Basic::Read(void* pBuffer, size_t nSize)
	{
		DWORD read = 0;
		return (size_t)(::ReadFile(m_hFile, pBuffer, (DWORD)nSize, &read, NULL) ? read : 0);
	}

	size_t Basic::Write(void* pData, size_t nSize)
	{
		DWORD written = 0;
		return (size_t)(::WriteFile(m_hFile, pData, (DWORD)nSize, &written, NULL) ? written : 0);
	}
}
