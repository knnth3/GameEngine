#include "Test.h"
#include "Client.h"
#include "Server.h"

void Net::Tester::TestNet()
{
	int a = 0;
	std::cin >> a;
	std::cin.ignore();
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
		printf("Enter the IP of who you want to connect to: ");
		std::string addr = "";
		addr.resize(1000);
		std::cin.getline(&addr[0], 1000);
		std::string address = std::string(addr.c_str());
		Client c(address, 1234);
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