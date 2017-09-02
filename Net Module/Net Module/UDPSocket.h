#pragma once
#include "Address.h"
#include "Helper.h"
#include <memory>

//Author:Eric Marquez
//+ Created as an interpretation of Glenn Fiedler's documentation
//+acts as the fundamental connection to serve under either a server or client


namespace Net
{
	class UDPSocket
	{
	public:

		bool Open(unsigned short port);
		void Close();
		int Send(const std::shared_ptr<Address>& destination, const void * data, int size);
		int Receive(std::shared_ptr<Address>& sender, void * data, int max_size);

	private:

		SOCKET m_handle;
	};


}