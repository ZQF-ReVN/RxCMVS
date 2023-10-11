#pragma once
#include <string>

#include "../RxFS/Binary.hpp"


namespace Rut::RxMem
{
	static constexpr size_t AutoMem_AutoSize = -1;

	class Auto
	{
	private:
		uint8_t* m_pMemData;
		size_t m_uiMemSize;
		size_t m_uiMaxSize;

	public:
		Auto();
		Auto(const Auto& buffer);
		Auto(Auto&& buffer) noexcept;
		template <typename T_STR> Auto(T_STR T_PATH) : Auto() { LoadFile(T_PATH); }
		template <typename T_STR> Auto(T_STR T_PATH, size_t szFile) : Auto() { LoadFile(T_PATH, szFile); }
		~Auto();

		Auto& Copy(const Auto& buffer);
		Auto& Move(Auto& buffer);
		Auto& Append(Auto& rfAutoMem);

		Auto& operator[] (size_t tSize);
		Auto& operator=(Auto&& rfAutoMem) noexcept;
		Auto& operator=(const Auto& rfAutoMem);

	public:
		template <typename T_PTR> operator T_PTR* () { return (T_PTR*)m_pMemData; }
		template <typename T_TYPE> operator T_TYPE () { return (T_TYPE)m_uiMemSize; }
		template <typename T_STR> void SaveData(T_STR PATH) { RxFS::SaveFileViaPath(PATH, m_pMemData, m_uiMemSize); }
		template <typename T_STR> uint8_t* LoadFile(T_STR PATH, size_t uiSize = AutoMem_AutoSize)
		{
			RxFS::Binary ifs{ PATH, RIO::RIO_IN };
			if (uiSize == AutoMem_AutoSize) { uiSize = ifs.GetSize(); }
			ifs.Read(SetSize(uiSize), uiSize);
			return m_pMemData;
		}

	public:
		size_t GetSize();
		uint8_t* GetPtr();
		uint8_t* SetSize(size_t uiNewSize, bool isCopy = false);
	};
}