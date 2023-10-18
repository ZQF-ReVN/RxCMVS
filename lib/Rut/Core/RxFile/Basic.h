#pragma once
#include <string>


namespace Rut
{
	constexpr size_t RIO_IN = 0x01;
	constexpr size_t RIO_OUT = 0x02;
	constexpr size_t RIO_CREATE_ALWAYS = 0x04;
	constexpr size_t RIO_CREATE_NEW = 0x08;
	constexpr size_t RIO_OPEN_ALWAYS = 0x10;
	constexpr size_t RIO_OPEN_EXISTING = 0x20;

	constexpr size_t RIO_BEGIN = 0x0;
	constexpr size_t RIO_CURRENT = 0x1;
	constexpr size_t RIO_END = 0x2;

	constexpr size_t RIO_READ = RIO_IN | RIO_OPEN_EXISTING;
	constexpr size_t RIO_WRITE = RIO_OUT | RIO_CREATE_ALWAYS;
}


namespace Rut::RxFile
{
	class Basic
	{
	protected:
		void* m_hFile;

	protected:
		Basic();
		Basic(Basic&& refStream) noexcept;
		virtual ~Basic();

	public:
		void Create(std::string_view msPath, size_t nMode);
		void Create(std::wstring_view wsPath, size_t nMode);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize();
		size_t SetPos(size_t nOffset, size_t nMode);
		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(void* pData, size_t nSize);

	};
}
