#include "UDPSocket.h"


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
		printf("failed to create socket\n");
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
		printf("failed to bind socket. There may be another instance active.\n");
		return false;
	}
	printf("Listening on port %d\n", port);

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	int nonBlocking = 1;
	if (fcntl(m_handle,
		F_SETFL,
		O_NONBLOCK,
		nonBlocking) == -1)
	{
		printf("failed to set non-blocking\n");
		return false;
	}

#elif PLATFORM == PLATFORM_WINDOWS

	DWORD nonBlocking = 1;
	if (ioctlsocket(m_handle,
		FIONBIO,
		&nonBlocking) != 0)
	{
		printf("failed to set non-blocking\n");
		return false;
	}

#endif

	return true;
}

void UDPSocket::Close()
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	close(m_handle);
#elif PLATFORM == PLATFORM_WINDOWS
	closesocket(m_handle);
#endif
}

int UDPSocket::Send(const std::shared_ptr<Address>& destination, const void * data, int size)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination->GetAddress());
	address.sin_port = htons(destination->GetPort());

	int sent_bytes = sendto(m_handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

	if (sent_bytes != size)
	{
		printf("failed to send packet\n");
		return 0;
	}

	return sent_bytes;
}

int UDPSocket::Receive(std::shared_ptr<Address>& sender, void * data, int max_size)
{

#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	int bytes = recvfrom(m_handle, (char*)data, max_size, 0, (sockaddr*)&from, &fromLength);

	unsigned int from_address = ntohl(from.sin_addr.s_addr);

	unsigned short from_port = ntohs(from.sin_port);

	sender = std::make_shared<Address>(from_address, from_port, 0);

	return bytes;
}
