#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstdint>
#include <vector>

#include "utils.h"

void printDNSHeader(const DNSHeader& header)
{
	std::cout << "----- DNS Header -----\n";
	std::cout << "Transaction ID: " << header.transaction_id << "\n";
	std::cout << "Flags: " << header.flags << "\n";
	std::cout << "Number of questions: " << header.num_questions << "\n";
	std::cout << "Number of answers: " << header.num_answers << "\n";
	std::cout << "Number of authority RRs: " << header.num_authority_rrs << "\n";
	std::cout << "Number of additional RRs: " << header.num_additional_rrs << "\n";
}

SOCKET createSocket(const char* host, uint16_t port)
{
	SOCKADDR_IN address;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, host, &address.sin_addr);

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET)
	{
		std::cerr << "[ERROR] Failed to create socket!\n";
		std::exit(EXIT_FAILURE);
	}

	if (bind(sock, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR)
	{
		std::cerr << "[ERROR] Failed to bind socket!\n";
		std::exit(EXIT_FAILURE);
	}

	return sock;
}

std::vector<char> createBogusQuestion(char* data, int name_len)
{
	char bogus_name[] = {7, 'e', 'x', 'a', 'm', 'p', 'l', 'e', 7, 'i', 'n', 'v', 'a', 'l', 'i', 'd', 0};
	int length = strlen(bogus_name) + 1;

	std::cout << "Bogus length: " << length << "\n";

	std::vector<char> question;
	question.resize(length + (2 * sizeof(uint16_t)));

	memcpy(question.data(), bogus_name, length);
	memcpy(question.data() + length, data + name_len, 2 * sizeof(uint16_t));
	
	return question;
}

std::string getName(char* data)
{
	std::string name;
	
	return name;
}