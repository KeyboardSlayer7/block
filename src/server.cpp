#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <fstream>
#include <string>

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

		uint16_t transaction_id, num_questions;

		std::memcpy(&transaction_id, _buffer.data(), sizeof(uint16_t));
		transaction_id = htons(transaction_id);

		std::memcpy(&num_questions, _buffer.data() + sizeof(uint16_t) * 2, sizeof(uint16_t));
		num_questions = htons(num_questions);

		std::cout << "Transaction ID: " << transaction_id << "\n";
		std::cout << "Number of questions: " << num_questions << "\n";

		/* 
		DNS header is not actually required as the only field that matters is the transation id
		and number of questions.
		*/

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
			if (_clients.find(transaction_id) == _clients.end())
			{
				_clients[transaction_id] = std::make_pair(from, 1);
			}
			else
			{
				_clients.at(transaction_id).second++;
			}

			char* data = _buffer.data() + sizeof(DNSHeader);	
			
			DomainName name = getName(data);
			std::cout << "Name: " << name.name << "\n";
			std::cout << "Length: " << name.encoded_length << "\n";

			if (_blacklist.find(name.name) == _blacklist.end())
			{
				sendto(_socket, _buffer.data(), bytes, 0, (SOCKADDR*)&_dns_server, sizeof(_dns_server)); 	
			}
			else
			{
				std::cout << "sending bogus\n";
	
				std::vector<char> bogus = createBogusQuestion(data, name.encoded_length);
				std::cout << "Bogus: " << getName(bogus.data()).name << "\n";

				std::vector<char> bogus_request;
				bogus_request.resize(sizeof(DNSHeader) + bogus.size());

				memcpy(bogus_request.data(), _buffer.data(), sizeof(DNSHeader));
				memcpy(bogus_request.data() + sizeof(DNSHeader), bogus.data(), bogus.size());

				sendto(_socket, bogus_request.data(), bogus_request.size(), 0, (SOCKADDR*)&_dns_server, sizeof(_dns_server));
			}
		}
		else
		{
			sendto(_socket, _buffer.data(), bytes, 0, (SOCKADDR*)&_clients.at(transaction_id), sizeof(_clients.at(transaction_id)));
			
			_clients.at(transaction_id).second--;	
			if (_clients.at(transaction_id).second <= 0)
			{
				_clients.erase(transaction_id);
			}
		}

		// std::cout << "[";
		// for (int i = 0; i < bytes - sizeof(DNSHeader); ++i)
		// {
		// 	std::cout << (int)data[i] << ", ";
		// }
		// std::cout << "]";

		// std::cout << "[";
		// for (int i = 0; i < bogus.size(); ++i)
		// {
		// 	std::cout << (int)bogus[i] << ", ";
		// }
		// std::cout << "]";

		std::cout << "\n\n";

		// printDNSHeader(header);
	}
}

void Server::loadBlacklist(const char* filename)
{
	std::ifstream file(filename);

	std::string buffer;
	int index = 0;
	while (std::getline(file, buffer))
	{
		// if (index < 10)
		// 	std::cout << buffer << "\n";
		// else
		// 	break;
			
		// index++;

		if (buffer[0] == '0')
		{
			// std::cout << buffer.substr(8, buffer.length() - 8) << "\n";
			_blacklist.insert(buffer.substr(8, buffer.length() - 8));
		}
	}
}

void Server::setDNSServer(const char* server_address)
{
    _dns_server.sin_family = AF_INET;
    _dns_server.sin_port = htons(DNS_SERVER_PORT);
    inet_pton(AF_INET, server_address, &_dns_server.sin_addr);

	_dns_server_address = server_address;
}