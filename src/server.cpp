#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstdint>
#include <array>
#include <unordered_map>

#include "utils.h"
#include "server.h"

Server::Server(const char* host, uint16_t port)
{
    _socket = createSocket(host, port);
    _running = true;

    _buffer.fill(0);
    _address.fill(0);
}

Server::~Server()
{
    closesocket(_socket);
}
    
void Server::run()
{
	while (_running)
	{
		SOCKADDR_IN from;
		int from_len = sizeof(from);

		int bytes = recvfrom(_socket, _buffer.data(), _buffer.size(), 0, (SOCKADDR*)&from, &from_len);
		
		inet_ntop(AF_INET, &from.sin_addr, _address.data(), IPV4_ADDRESS_LENGTH);
		std::cout << "Received packet from: " << _address.data() << ":" << ntohs(from.sin_port) << "\n";

		uint16_t transaction_id;
		std::memcpy(&transaction_id, _buffer.data(), sizeof(uint16_t));
		transaction_id = htons(transaction_id);

		/* DNS header is not actually required as the only field that matters is the transation id */
		// DNSHeader header;
		// std::memcpy(&header, _buffer.data(), sizeof(DNSHeader));

		// header.transaction_id = htons(header.transaction_id);
		// header.flags = htons(header.flags);
		// header.num_questions = htons(header.num_questions);
		// header.num_answers = htons(header.num_answers);
		// header.num_authority_rrs = htons(header.num_authority_rrs);
		// header.num_additional_rrs = htons(header.num_additional_rrs);

		if (strcmp(_address.data(), _dns_server_address) != 0)
		{
			_clients[transaction_id] = from;

			sendto(_socket, _buffer.data(), bytes, 0, (SOCKADDR*)&_dns_server, sizeof(_dns_server)); 	
		}
		else
		{
			sendto(_socket, _buffer.data(), bytes, 0, (SOCKADDR*)&_clients.at(transaction_id), sizeof(_clients.at(transaction_id)));
			_clients.erase(transaction_id);
		}

		char* data = _buffer.data() + sizeof(DNSHeader);
		
		int index = 0;
		
		while (data[index] != '\0')
		{
			uint8_t length = data[index];

			char* section = new char[length + 1];
			section[length] = '\0';
	
			std::memcpy(section, data + index + 1, length);

			index = index + length + 1;

			std::cout << section << ".";

			delete[] section;
		}	

		std::cout << "\n";

		// printDNSHeader(header);
	}
}

void Server::setDNSServer(const char* server_address)
{
    _dns_server.sin_family = AF_INET;
    _dns_server.sin_port = htons(DNS_SERVER_PORT);
    inet_pton(AF_INET, server_address, &_dns_server.sin_addr);

	_dns_server_address = server_address;
}