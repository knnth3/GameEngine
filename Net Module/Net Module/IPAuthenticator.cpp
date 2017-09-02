#include "IPAuthenticator.h"


namespace Net
{

	IPAuthenticator::IPAuthenticator()
	{
		t1 = std::chrono::high_resolution_clock::now();
		m_asyncThread = std::async(std::launch::async, &IPAuthenticator::CountdownTimeouts, this);
	}

	IPAuthenticator::~IPAuthenticator()
	{
		using namespace std::chrono_literals;
		m_closeThread = true;
		std::future_status status;
		do
		{
			status = m_asyncThread.wait_for(0ms);
		} while (status != std::future_status::ready);

	}

	bool IPAuthenticator::IsTimedOut(Identification IP)
	{
		if (m_attemptsLeft.find(IP) == m_attemptsLeft.end())
		{
			m_attemptsLeft[IP] = 4;
			return false;
		}
		else
		{
			if (m_attemptsLeft[IP] != 0)
			{
				m_attemptsLeft[IP] -= 1;
				return false;
			}
			else
			{
				if (m_secondsLeft.find(IP) != m_secondsLeft.end())
				{
					if (m_secondsLeft[IP] == 0.f)
					{
						printf("Too many connection attempts. Timing out IP: %i\n", IP);
						m_secondsLeft[IP] = m_timeoutTime;
						return true;
					}
					else
						return true;
				}
				printf("Too many connection attempts. Timing out IP: %i\n", IP);
				m_secondsLeft[IP] = m_timeoutTime;
				return true;
			}
		}
	}

	void IPAuthenticator::CountdownTimeouts()
	{
		while (!m_closeThread)
		{
			t2 = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

			for (auto x = m_secondsLeft.begin(); x != m_secondsLeft.end(); x++)
			{
				if (x->second != 0.f)
				{
					if (x->second >(float)time_span.count())
					{
						m_secondsLeft[x->first].store(m_secondsLeft[x->first].load() - (float)time_span.count());
					}
					else
					{
						printf("Time out for  IP: %i has been removed\n", x->first);
						m_secondsLeft[x->first] = 0.f;
						m_attemptsLeft[x->first] = 5;
					}
				}
			}
			t1 = t2;
		}
	}
}
