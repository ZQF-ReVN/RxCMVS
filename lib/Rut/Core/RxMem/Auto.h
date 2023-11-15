#pragma once
#include <string>
#include <memory>


namespace Rut::RxMem
{
	constexpr size_t AutoMem_AutoSize = -1;

	class Auto
	{
	private:
		size_t m_uiMemSize;
		size_t m_uiMaxSize;
		std::unique_ptr<uint8_t[]> m_upMemData;

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