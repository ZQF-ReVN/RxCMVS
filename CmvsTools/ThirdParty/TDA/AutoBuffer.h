#pragma once


namespace TDA
{
	class AutoBuffer
	{
	protected:
		char* m_pBuffer;
		size_t m_szAllocMax;

		AutoBuffer();
		~AutoBuffer();

		char* BufferReSize(size_t szRes);
	};
}