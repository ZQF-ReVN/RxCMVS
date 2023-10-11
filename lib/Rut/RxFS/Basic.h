#pragma once
#include <cstdint>
#include <stdexcept>
#include <Windows.h>


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


namespace Rut::RxFS
{
	class Basic
	{
	protected:
		HANDLE m_hFile;

	protected:
		Basic();
		Basic(Basic&& refStream) noexcept;
		virtual ~Basic();

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
