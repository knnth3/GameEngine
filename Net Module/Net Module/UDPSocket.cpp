#include "UDPSocket.h"


namespace Net
{

	bool UDPSocket::Open(unsigned short port)
	{
		//Assign random port if invalid
		if (port <= 1024)
		{
			port = 0;
		}

		//Create a socket and assign the id to m_handle
		m_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_handle <= 0)
		{
			return false;
		}

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons((unsigned short)port);

		//Bind the socket to the address
		int bound = bind(m_handle, (const sockaddr*)&address, sizeof(sockaddr_in));
		if (bound < 0)
		{
			return false;
		}

#if NET_PLATFORM == NET_PLATFORM_MAC || NET_PLATFORM == NET_PLATFORM_UNIX

		int nonBlocking = 1;
		if (fcntl(m_handle,
			F_SETFL,
			O_NONBLOCK,
			nonBlocking) == -1)
		{
			return false;
		}

#elif NET_PLATFORM == NET_PLATFORM_WINDOWS

		DWORD nonBlocking = 1;
		if (ioctlsocket(m_handle,
			FIONBIO,
			&nonBlocking) != 0)
		{
			return false;
		}

#endif

		return true;
	}

	void UDPSocket::Close()
	{
#if NET_PLATFORM == NET_PLATFORM_MAC || NET_PLATFORM == NET_PLATFORM_UNIX
		close(m_handle);
#elif NET_PLATFORM == NET_PLATFORM_WINDOWS
		closesocket(m_handle);
#endif
	}

	int UDPSocket::Send(const Address& destination, const void * data, int size)
	{
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(destination.GetPackedIPv4());
		address.sin_port = htons(destination.GetPort());

		int sent_bytes = sendto(m_handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

		if (sent_bytes != size)
		{
			return 0;
		}

		return sent_bytes;
	}

	int UDPSocket::Receive(Address& sender, void * data, int max_size)
	{

#if NET_PLATFORM == NET_PLATFORM_WINDOWS
		typedef int socklen_t;
#endif

		sockaddr_in from;
		socklen_t fromLength = sizeof(from);

		int bytes = recvfrom(m_handle, (char*)data, max_size, 0, (sockaddr*)&from, &fromLength);

		unsigned int from_address = ntohl(from.sin_addr.s_addr);

		unsigned short from_port = ntohs(from.sin_port);

		sender = Address(from_address, from_port);

		return bytes;
	}

}