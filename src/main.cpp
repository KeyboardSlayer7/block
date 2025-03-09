#include <iostream>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define KILOBYTE 1024
#define IPV4_ADDRESS_LENGTH 16

struct DNSHeader
{
	uint16_t transaction_id;
	uint16_t flags;
	uint16_t num_questions;
	uint16_t num_answers;
	uint16_t num_authority_rrs;
	uint16_t num_additional_rrs;
};

void printDNSHeader(const DNSHeader& header);
SOCKET createSocket(const char* host, uint16_t port);

int main(int argc, char* argv[])
{
	WSADATA wsa_data;
	WORD version = MAKEWORD(2, 2);
	
	if (WSAStartup(version, &wsa_data) != 0)
	{
		std::cerr << "WSAStartup failed" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::unordered_map<uint16_t, SOCKADDR_IN> clients;

	SOCKADDR_IN dns_server;
	dns_server.sin_family = AF_INET;
	dns_server.sin_port = htons(53);
	inet_pton(AF_INET, "8.8.8.8", &dns_server.sin_addr);

	SOCKET sock = createSocket("0.0.0.0", 53);

	std::array<char, KILOBYTE> buffer;

	std::array<char, IPV4_ADDRESS_LENGTH> address;
	address[IPV4_ADDRESS_LENGTH - 1] = '\0'; 

	while (true)
	{
		SOCKADDR_IN from;
		int from_len = sizeof(from);

		int bytes = recvfrom(sock, buffer.data(), buffer.size(), 0, (SOCKADDR*)&from, &from_len);

		DNSHeader header;
		std::memcpy(&header, buffer.data(), sizeof(DNSHeader));

		header.transaction_id = htons(header.transaction_id);
		header.flags = htons(header.flags);
		header.num_questions = htons(header.num_questions);
		header.num_answers = htons(header.num_answers);
		header.num_authority_rrs = htons(header.num_authority_rrs);
		header.num_additional_rrs = htons(header.num_additional_rrs);

		inet_ntop(AF_INET, &from.sin_addr, address.data(), IPV4_ADDRESS_LENGTH);

		std::cout << "Received packet from: " << address.data() << ":" << ntohs(from.sin_port) << "\n";

		sendto(sock, buffer.data(), bytes, 0, (SOCKADDR*)&dns_server, sizeof(dns_server)); 


		
		printDNSHeader(header);
	}

	return 0;
}

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
		std::cerr << "socket failed" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	if (bind(sock, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR)
	{
		std::cerr << "bind failed" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return sock;
}