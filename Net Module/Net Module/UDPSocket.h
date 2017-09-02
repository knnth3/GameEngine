#pragma once
#include "Address.h"
#include "NetModule.h"
#include <memory>

//Author:Eric Marquez
//+ Created as an interpretation of Glenn Fiedler's documentation
//+acts as the fundamental connection to serve under either a server or client


namespace Net
{
	class UDPSocket
	{
	public:

		NET_API bool Open(unsigned short port);
		NET_API void Close();
		NET_API int Send(const std::shared_ptr<Address>& destination, const void * data, int size);
		NET_API int Receive(std::shared_ptr<Address>& sender, void * data, int max_size);

	private:

		SOCKET m_handle;
	};


}