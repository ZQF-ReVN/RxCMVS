#pragma once
#include <string>
#include <sstream>
#include <filesystem>
#include <functional>


namespace Rut
{
	constexpr size_t RIO_DATA_IN = 0x01;
	constexpr size_t RIO_DATA_OUT = 0x02;
	constexpr size_t RIO_CREATE_ALWAYS = 0x04;
	constexpr size_t RIO_CREATE_NEW = 0x08;
	constexpr size_t RIO_OPEN_ALWAYS = 0x10;
	constexpr size_t RIO_OPEN_EXISTING = 0x20;

	constexpr size_t RIO_BEGIN = 0x0;
	constexpr size_t RIO_CURRENT = 0x1;
	constexpr size_t RIO_END = 0x2;

	constexpr size_t RIO_READ = RIO_DATA_IN | RIO_OPEN_EXISTING;
	constexpr size_t RIO_WRITE = RIO_DATA_OUT | RIO_CREATE_ALWAYS;

	constexpr size_t RFM_ANSI = 0x0;
	constexpr size_t RFM_UTF8 = 0x1;
	constexpr size_t RFM_UTF16 = 0x2;
}

namespace Rut::RxFile::Stream
{
	class BasicStream
	{
	protected:
		void* m_hFile;

	protected:
		BasicStream();
		BasicStream(BasicStream&& refStream) noexcept;
		~BasicStream();

	public:
		void Create(const std::filesystem::path& phPath, size_t nMode);

		bool Close();
		bool Flush();
		bool IsEnd();

		size_t GetPos();
		size_t GetSize();
		size_t SetPos(size_t nOffset, size_t nMode = RIO_BEGIN);
		size_t Read(void* pBuffer, size_t nSize);
		size_t Write(const void* pData, size_t nSize);

	};
}

namespace Rut::RxFile::Stream
{
	class BinaryStream : public BasicStream
	{
	public:
		BinaryStream();
		BinaryStream(const std::filesystem::path& phPath, size_t nMode);
		BinaryStream(const BinaryStream& refStream) = delete;

	public:
		template <class T> BinaryStream& operator >>(T&& rfOBJ);
		template <class T> BinaryStream& operator <<(T&& rfOBJ);
		template <class T, size_t nSize = sizeof(T)> T Get();
	};

	template <class T> BinaryStream& BinaryStream::operator >>(T&& rfOBJ)
	{
		this->Read((void*)&rfOBJ, sizeof(rfOBJ));
		return *this;
	}

	template <class T> BinaryStream& BinaryStream::operator <<(T&& rfOBJ)
	{
		this->Write((void*)&rfOBJ, sizeof(rfOBJ));
		return *this;
	}

	template <class T, size_t nSize> T BinaryStream::Get()
	{
		T tmp_obj;
		this->Read((void*)&tmp_obj, nSize);
		return tmp_obj;
	}
}

namespace Rut::RxFile::Stream
{
	class TextStream : private BinaryStream
	{
	private:
		size_t m_rxFormat;

	public:
		TextStream();
		TextStream(const std::filesystem::path& phPath, size_t nMode, size_t nFormat);

		TextStream& operator <<(std::string_view msStr) { WriteLine(msStr); return *this; }
		TextStream& operator <<(std::wstring_view wsStr) { WriteLine(wsStr); return *this; }

		void WriteBOM();
		void CheckBOM();
		void EnsureBom(size_t nMode);

		size_t WriteLine(std::string_view msStr);
		size_t WriteLine(std::wstring_view wsStr);

		void WriteAllLine(std::vector<std::wstring>& vecLine);
		void WriteAllLine(std::vector<std::string>& vecLine);

		void ReadAllLine(std::vector<std::string>& vecLine);
		void ReadAllLine(std::vector<std::wstring>& vecLine);
		void ReadAllLine(std::function<bool(char*, char*)> fnPerline);
		void ReadAllLine(std::function<bool(wchar_t*, wchar_t*)> fnPerline);

		void ReadRawText(std::string& msText);
		void ReadRawText(std::wstring& wsText);

		void ReadToSStream(std::wstringstream& rfSStream);
		void ReadToSStream(std::stringstream& rfSStream);

		void Flush();
		void Close();

		void Rewind();
	};
}

namespace Rut::RxFile
{
	using Binary = Rut::RxFile::Stream::BinaryStream;
	using Text = Rut::RxFile::Stream::TextStream;
	void SaveFileViaPath(const std::filesystem::path& phPath, void* pData, size_t nBytes);
}