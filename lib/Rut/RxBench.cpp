#include "RxBench.h"

#include <iostream>


namespace Rut::RxBench
{
	void Record::Beg()
	{
		m_tpBeg = std::chrono::steady_clock::now();
	}

	void Record::End()
	{
		m_tpEnd = std::chrono::steady_clock::now();
		m_vcRecord.emplace_back((m_tpEnd - m_tpBeg));
	}

	void Record::Log()
	{
		std::chrono::duration<double, std::milli> cout{};

		for (auto& dur : m_vcRecord)
		{
			cout += dur;
			std::cout << dur << "\n";
		}

		std::cout << "Avg:" << cout / m_vcRecord.size() << "\n";
	}
}