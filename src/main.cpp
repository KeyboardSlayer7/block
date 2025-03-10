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

	// std::unordered_map<uint16_t, SOCKADDR_IN> clients;

	// SOCKADDR_IN dns_server;
	// dns_server.sin_family = AF_INET;
	// dns_server.sin_port = htons(53);
	// inet_pton(AF_INET, argv[1], &dns_server.sin_addr);

	// SOCKET sock = createSocket("0.0.0.0", 53);

	// std::array<char, KILOBYTE> buffer;

	// std::array<char, IPV4_ADDRESS_LENGTH> address;
	// address[IPV4_ADDRESS_LENGTH - 1] = '\0'; 

	// while (true)
	// {
	// 	SOCKADDR_IN from;
	// 	int from_len = sizeof(from);

	// 	int bytes = recvfrom(sock, buffer.data(), buffer.size(), 0, (SOCKADDR*)&from, &from_len);
		
	// 	inet_ntop(AF_INET, &from.sin_addr, address.data(), IPV4_ADDRESS_LENGTH);
	// 	std::cout << "Received packet from: " << address.data() << ":" << ntohs(from.sin_port) << "\n";

	// 	DNSHeader header;
	// 	std::memcpy(&header, buffer.data(), sizeof(DNSHeader));

	// 	header.transaction_id = htons(header.transaction_id);
	// 	header.flags = htons(header.flags);
	// 	header.num_questions = htons(header.num_questions);
	// 	header.num_answers = htons(header.num_answers);
	// 	header.num_authority_rrs = htons(header.num_authority_rrs);
	// 	header.num_additional_rrs = htons(header.num_additional_rrs);

	// 	if (strcmp(address.data(), argv[1]) != 0)
	// 	{
	// 		clients[header.transaction_id] = from;

	// 		sendto(sock, buffer.data(), bytes, 0, (SOCKADDR*)&dns_server, sizeof(dns_server)); 	
	// 	}
	// 	else
	// 	{
	// 		sendto(sock, buffer.data(), bytes, 0, (SOCKADDR*)&clients.at(header.transaction_id), sizeof(clients.at(header.transaction_id)));
	// 		clients.erase(header.transaction_id);
	// 	}

	// 	char* data = buffer.data() + sizeof(DNSHeader);
		
	// 	int index = 0;
		
	// 	while (data[index] != '\0')
	// 	{
	// 		uint8_t length = data[index];

	// 		char* section = new char[length + 1];
	// 		section[length] = '\0';
	
	// 		std::memcpy(section, data + index + 1, length);

	// 		index = index + length + 1;

	// 		std::cout << section << ".";

	// 		delete[] section;
	// 	}	

	// 	std::cout << "\n";

	// 	printDNSHeader(header);
	// }

	WSACleanup();

	return 0;
}