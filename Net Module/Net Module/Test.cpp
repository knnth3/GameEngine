#include "Test.h"
#include "Client.h"
#include "Server.h"

void Net::TestFunc()
{
	int a = 0;
	std::cin >> a;
	switch (a)
	{
	case 1:
	{
		Server s(1234);
		bool result = s.Initialize();
		while (result)
		{

		}
		s.Close();
		break;
	}
	case 2:
	{
		Client c("127.0.0.1", 1234);
		bool result = c.Initialize();
		while (result)
		{
			std::string a = "";
			a.resize(1000);
			std::cin.getline(&a[0], 1000);
			std::string data = std::string(a.c_str());
			if (data.size() != 0)
			{
				c.Send(data);
			}
			edata datas = c.Recive();
			if (!datas.empty())
			{
				std::cout << "got a download\n";
			}
		}
		c.Close();
		break;
	}
	}
}