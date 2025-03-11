#include <iostream>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "utils.h"
#include "server.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <host>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	WSADATA wsa_data;
	WORD version = MAKEWORD(2, 2);
	
	if (WSAStartup(version, &wsa_data) != 0)
	{
		std::cerr << "WSAStartup failed" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	Server server("0.0.0.0", 53);
	server.setDNSServer(argv[1]);

	server.run();

	WSACleanup();

	return 0;
}