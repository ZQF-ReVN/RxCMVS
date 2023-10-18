#pragma once
#include <chrono>


namespace Rut::RxBench
{
	class Record
	{
	private:
		std::chrono::steady_clock::time_point m_tpBeg;
		std::chrono::steady_clock::time_point m_tpEnd;
		std::vector<std::chrono::duration<double, std::milli>> m_vcRecord;

	public:
		void Beg();
		void End();
		void Log();
	};
}