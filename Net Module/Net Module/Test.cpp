
#ifdef _DEBUG
#include "Client.h"
#include "Server.h"
#include <iostream>


using namespace std;
using namespace Net;

std::string GetInput()
{
	string message;
	getline(cin, message);
	return message;
}

bool TestNet()
{
	bool result = false;
	string strResult = "";

BEGIN:
	cout << "Net Module test example. Enter in 0 for server or 1 for client. Press enter when selected." << endl;
	strResult = GetInput();

	if (!strResult.empty())
	{
		if (!strResult.compare("0"))
		{
			auto server = std::make_unique<Server>();
			if (!server->Open(1234))
			{
				cout << "Could not init Server." << endl;
				return false;
			}
			else
			{
				uint32_t ID = 0;
				cout << "Server init success. Type -exit to close." << endl;
				while (true)
				{
					std::string username;
					if (server->GetNewLoginRequest(ID, username))
					{
						cout << "[" << ID << "] " << username << " logged in." << endl;
					}

					ByteBuffer recieved;
					if (server->Recieve(ID, recieved))
					{
						std::string bytes(recieved.begin(), recieved.end());
						cout << "[" << ID << "]: " << bytes << endl;
						server->SendToAll(recieved);
					}

					uint32_t logoutID = 0;
					if (server->GetLogoutRequest(logoutID))
					{
						cout << "[" << logoutID << "] has logged out." << endl;
					}
				}
			}
			server->CloseAllConnections();
		}
		else if (!strResult.compare("1"))
		{
			std::string IPV4addr;
			cout << "Enter the IP of who you want to connect to: " << endl;
			IPV4addr = GetInput();
			auto client = std::make_unique<Client>();
			if (client->Initialize(IPV4addr, 1234))
			{
				if (!client->Login("Kenneth", "1234"))
				{
					cout << "Could not login to Server at given IP. Check the port number if IP is correct." << endl;
				}
				else
				{
					result = true;
					cout << "Client login success. Type -exit to close." << endl;
					int comparison = 0;
					do
					{
						ByteBuffer recieved;
						if (client->Recieve(recieved))
						{
							std::string bytes(recieved.begin(), recieved.end());
							cout << "[SERVER]: " << bytes << endl;
						}

						std::string usrInput = GetInput();
						comparison = usrInput.compare("-exit");
						if (comparison != 0)
						{
							client->Send(usrInput);
						}
					} while (comparison != 0);
					client->Logout();
				}
			}
		}
		else
		{
			cout << "Invalid response. " << endl;
			goto BEGIN;
		}
	}
	return result;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	TestNet();
	return 0;
}
#endif