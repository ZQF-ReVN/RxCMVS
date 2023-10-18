#pragma once
#include <string>


namespace Rut::RxMem
{
	constexpr size_t AutoMem_AutoSize = -1;

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
		Auto(std::string_view msPath);
		Auto(std::wstring_view wsPath);
		Auto(std::string_view msPath, size_t szFile);
		Auto(std::wstring_view wsPath, size_t szFile);
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

		void SaveData(std::string_view msPath);
		void SaveData(std::wstring_view wsPath);
		uint8_t* LoadFile(std::string_view msPath, size_t nSize = AutoMem_AutoSize);
		uint8_t* LoadFile(std::wstring_view wsPath, size_t nSize = AutoMem_AutoSize);

	public:
		size_t GetSize();
		uint8_t* GetPtr();
		uint8_t* SetSize(size_t uiNewSize, bool isCopy = false);
	};
}