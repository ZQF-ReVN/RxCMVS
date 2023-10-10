#pragma once
#include <string>

#include "../FileStream/BinaryStream.hpp"


namespace Rut::RxStream
{
	static constexpr size_t AutoMem_AutoSize = -1;

	class AutoMem
	{
	private:
		uint8_t* m_pMemData;
		size_t m_uiMemSize;
		size_t m_uiMaxSize;

	public:
		AutoMem();
		AutoMem(const AutoMem& buffer);
		AutoMem(AutoMem&& buffer) noexcept;
		template <typename T_STR> AutoMem(T_STR T_PATH) : AutoMem() { LoadFile(T_PATH); }
		template <typename T_STR> AutoMem(T_STR T_PATH, size_t szFile) : AutoMem() { LoadFile(T_PATH, szFile); }
		~AutoMem();

		AutoMem& Copy(const AutoMem& buffer);
		AutoMem& Move(AutoMem& buffer);
		AutoMem& Append(AutoMem& rfAutoMem);

		AutoMem& operator[] (size_t tSize);
		AutoMem& operator=(AutoMem&& rfAutoMem) noexcept;
		AutoMem& operator=(const AutoMem& rfAutoMem);

	public:
		template <typename T_PTR> operator T_PTR* () { return (T_PTR*)m_pMemData; }
		template <typename T_TYPE> operator T_TYPE () { return (T_TYPE)m_uiMemSize; }
		template <typename T_STR> void SaveData(T_STR PATH) { SaveFileViaPath(PATH, m_pMemData, m_uiMemSize); }
		template <typename T_STR> uint8_t* LoadFile(T_STR PATH, size_t uiSize = AutoMem_AutoSize)
		{
			Binary ifs{ PATH, RIO::RIO_IN };
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