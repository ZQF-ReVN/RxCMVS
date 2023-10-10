#include "BasicStream.h"

#ifdef WIN32

namespace Rut::RxStream
{
	BasicStream::BasicStream() : m_hFile(INVALID_HANDLE_VALUE)
	{

	}

	BasicStream::BasicStream(BasicStream&& refStream) noexcept
	{
		this->m_hFile = refStream.m_hFile;
		refStream.m_hFile = INVALID_HANDLE_VALUE;
	}

	BasicStream::~BasicStream()
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

	void BasicStream::Create(const char* cpPath, RIO emAccess, RCO emCreate)
	{
		m_hFile = ::CreateFileA(cpPath, GetAccess(emAccess), FILE_SHARE_READ, NULL, GetCreate(emAccess, emCreate), FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate)
	{
		m_hFile = ::CreateFileW(wpPath, GetAccess(emAccess), FILE_SHARE_READ, NULL, GetCreate(emAccess, emCreate), FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(std::string_view msPath, RIO emAccess, RCO emCreate)
	{
		Create(msPath.data(), emAccess, emCreate);
	}

	void BasicStream::Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate)
	{
		Create(wsPath.data(), emAccess, emCreate);
	}


	bool BasicStream::Close()
	{
		bool is_close = ::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return is_close;
	}

	bool BasicStream::Flush()
	{
		return FlushFileBuffers(m_hFile);
	}

	bool BasicStream::IsEnd()
	{
		size_t file_size = GetSize();
		size_t current_pointer = GetPos();
		return current_pointer >= file_size;
	}

	void BasicStream::SetEnd()
	{
		::SetFilePointer(m_hFile, 0, NULL, FILE_END);
	}

	size_t BasicStream::SetPos(size_t nOffset)
	{
		return (size_t)::SetFilePointer(m_hFile, (LONG)nOffset, NULL, FILE_BEGIN);
	}

	size_t BasicStream::GetPos()
	{
		return ::SetFilePointer(m_hFile, NULL, NULL, FILE_CURRENT);
	}

	size_t BasicStream::MovePos(size_t nDistance)
	{
		return (size_t)::SetFilePointer(m_hFile, (LONG)nDistance, NULL, FILE_CURRENT);
	}

	size_t BasicStream::GetSize(size_t* pHigh)
	{
		return (size_t)::GetFileSize(m_hFile, (LPDWORD)pHigh);
	}


	size_t BasicStream::Read(void* pBuffer, size_t nSize)
	{
		DWORD read = 0;
		return (size_t)(::ReadFile(m_hFile, pBuffer, (DWORD)nSize, &read, NULL) ? read : 0);
	}

	size_t BasicStream::Write(void* pData, size_t nSize)
	{
		DWORD written = 0;
		return (size_t)(::WriteFile(m_hFile, pData, (DWORD)nSize, &written, NULL) ? written : 0);
	}
}

#else

#include <filesystem>


namespace Rut::RxStream::C_
{
	BasicStream::BasicStream() : m_pStream(nullptr)
	{

	}

	BasicStream::BasicStream(BasicStream&& refStream) noexcept
	{
		this->m_pStream = refStream.m_pStream;
		refStream.m_pStream = nullptr;
	}

	BasicStream::~BasicStream()
	{
		Close();
	}

	void BasicStream::Create(const char* cpPath, RIO emAccess, RCO emCreate)
	{
		const char* mode = nullptr;
		switch (emAccess)
		{
		case RIO::RIO_IN: mode = "rb"; break;
		case RIO::RIO_OUT: mode = "wb"; break;
		case RIO::RIO_IN_OUT: mode = "rb+"; break;
		}

		errno_t err = fopen_s(&m_pStream, cpPath, mode);
		if (m_pStream == nullptr) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate)
	{
		const wchar_t* mode = nullptr;
		switch (emAccess)
		{
		case RIO::RIO_IN: mode = L"rb"; break;
		case RIO::RIO_OUT: mode = L"wb"; break;
		case RIO::RIO_IN_OUT: mode = L"rb+"; break;
		}

		errno_t err = _wfopen_s(&m_pStream, wpPath, mode);
		if (m_pStream == nullptr) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(std::string_view msPath, RIO emAccess, RCO emCreate)
	{
		Create(msPath.data(), emAccess, emCreate);
	}

	void BasicStream::Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate)
	{
		Create(wsPath.data(), emAccess, emCreate);
	}


	bool BasicStream::Close()
	{
		fclose(m_pStream);
		m_pStream = nullptr;
		return true;
	}

	bool BasicStream::Flush()
	{
		fflush(m_pStream);
		return true;
	}

	bool BasicStream::IsEnd()
	{
		size_t file_size = GetSize();
		size_t current_pointer = GetPos();
		return current_pointer >= file_size;
	}

	void BasicStream::SetEnd()
	{
		fseek(m_pStream, 0, SEEK_END);
	}

	void BasicStream::SetPos(size_t nOffset)
	{
		fseek(m_pStream, (long)nOffset, SEEK_SET);
	}

	void BasicStream::MovePos(size_t nDistance)
	{
		fseek(m_pStream, (long)nDistance, SEEK_CUR);
	}

	size_t BasicStream::GetPos()
	{
		return (size_t)ftell(m_pStream);
	}

	size_t BasicStream::GetSize()
	{
		size_t cur = GetPos();
		SetEnd();
		size_t end = GetPos();
		SetPos(cur);
		return end;
	}


	size_t BasicStream::Read(void* pBuffer, size_t nSize)
	{
		return fread(pBuffer, 1, nSize, m_pStream);
	}

	size_t BasicStream::Write(void* pData, size_t nSize)
	{
		return fwrite(pData, 1, nSize, m_pStream);
	}
}


namespace Rut::RxStream::Std
{
	BasicStream::~BasicStream()
	{
		Close();
	}

	static std::ios_base::openmode GetMode(RIO emAccess, RCO emCreate)
	{
		std::ios_base::openmode mode = 0;

		switch (emAccess)
		{
		case RIO::RIO_IN: mode = std::ios::binary | std::ios::in; break;
		case RIO::RIO_OUT: mode = std::ios::binary | std::ios::out; break;
		case RIO::RIO_IN_OUT: mode = std::ios::binary | std::ios::in | std::ios::out; break;
		}

		switch (emCreate)
		{
		case RCO::RCO_CREATE: case RCO::RCO_OPEN: case RCO::RCO_AUTO: case RCO::RCO_OPEN_UNSAFE: break;

		}

		return mode;
	}

	void BasicStream::Create(const char* cpPath, RIO emAccess, RCO emCreate)
	{
		if (emCreate == RCO::RCO_CREATE_SAFE) { if (!std::filesystem::exists(cpPath)) { throw std::runtime_error("RxStream File Already Exists"); }; }
		m_fsStream.open(cpPath, GetMode(emAccess, emCreate));
		if (!m_fsStream) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate)
	{
		if (emCreate == RCO::RCO_CREATE_SAFE) { if (!std::filesystem::exists(wpPath)) { throw std::runtime_error("RxStream File Already Exists"); }; }
		m_fsStream.open(wpPath, GetMode(emAccess, emCreate));
		if (!m_fsStream) { throw std::runtime_error("RxStream Create File Error!"); }
	}

	void BasicStream::Create(std::string_view msPath, RIO emAccess, RCO emCreate)
	{
		Create(msPath.data(), emAccess, emCreate);
	}

	void BasicStream::Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate)
	{
		Create(wsPath.data(), emAccess, emCreate);
	}

	bool BasicStream::Close()
	{
		m_fsStream.close();
		return true;
	}

	bool BasicStream::Flush()
	{
		m_fsStream.flush();
		return true;
	}

	bool BasicStream::IsEnd()
	{
		size_t size = GetSize();
		size_t cur = GetPos();
		return cur >= size;
	}

	void BasicStream::SetEnd()
	{
		m_fsStream.seekg(0, std::ios::end);
	}

	void BasicStream::SetPos(size_t nOffset)
	{
		m_fsStream.seekg(nOffset, std::ios::beg);
	}

	size_t BasicStream::GetPos()
	{
		return (size_t)m_fsStream.tellg();
	}

	void BasicStream::MovePos(size_t nDistance)
	{
		m_fsStream.seekg(nDistance, std::ios::cur);
	}

	size_t BasicStream::GetSize()
	{
		size_t cur = this->GetPos();
		this->SetEnd();
		size_t end = this->GetPos();
		this->SetPos(cur);
		return end;
	}


	size_t BasicStream::Read(void* pBuffer, size_t nSize)
	{
		m_fsStream.read((char*)pBuffer, nSize);
		return nSize;
	}

	size_t BasicStream::Write(void* pData, size_t nSize)
	{
		m_fsStream.write((char*)pData, nSize);
		return nSize;
	}
}

#endif // WIN32

