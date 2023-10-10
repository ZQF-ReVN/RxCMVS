#pragma once
#include <cstdint>
#include <stdexcept>



namespace Rut
{
	enum RIO
	{
		RIO_IN,
		RIO_OUT,
		RIO_IN_OUT
	};

	enum RCO
	{
		RCO_CREATE,
		RCO_OPEN,
		RCO_AUTO,
		RCO_CREATE_SAFE,
		RCO_OPEN_UNSAFE,
	};
}


#ifdef WIN32

#include <Windows.h>


namespace Rut::RxStream
{
	class BasicStream
	{
	protected:
		HANDLE m_hFile;

	protected:
		BasicStream();
		BasicStream(BasicStream&& refStream) noexcept;
		virtual ~BasicStream();

	public:
		void Create(const char* cpPath, RIO emAccess, RCO emCreate);
		void Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate);
		void Create(std::string_view msPath, RIO emAccess, RCO emCreate);
		void Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize(size_t* pHigh = nullptr);

		void SetEnd();
		size_t SetPos(size_t nOffset);
		size_t MovePos(size_t nDistance);

		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(void* pData, size_t nSize);

	};
}

#else

#include <fstream>


namespace Rut::RxStream::C_
{
	class BasicStream
	{
	protected:
		FILE* m_pStream;

	protected:
		BasicStream();
		BasicStream(BasicStream&& refStream) noexcept;
		virtual ~BasicStream();

	public:
		void Create(const char* cpPath, RIO emAccess, RCO emCreate);
		void Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate);
		void Create(std::string_view msPath, RIO emAccess, RCO emCreate);
		void Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize();
		void SetEnd();
		void SetPos(size_t nOffset);
		void MovePos(size_t nDistance);

		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(void* pData, size_t nSize);

	};
}

namespace Rut::RxStream::Std
{
	class BasicStream
	{
	protected:
		std::fstream m_fsStream;

	protected:
		BasicStream() {};
		BasicStream(BasicStream&& refStream) = delete;
		virtual ~BasicStream();

	public:
		void Create(const char* cpPath, RIO emAccess, RCO emCreate);
		void Create(const wchar_t* wpPath, RIO emAccess, RCO emCreate);
		void Create(std::string_view msPath, RIO emAccess, RCO emCreate);
		void Create(std::wstring_view wsPath, RIO emAccess, RCO emCreate);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize();
		void SetEnd();
		void SetPos(size_t nOffset);
		void MovePos(size_t nDistance);

		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(void* pData, size_t nSize);

	};
}

#endif // WIN32
