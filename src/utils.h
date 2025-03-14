#ifndef UTILS_H
#define UTILS_H

#include <WinSock2.h>
#include <cstdint>
#include <vector>

#define DNS_SERVER_PORT 53
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

struct DomainName
{
	size_t encoded_length;
	std::string name;
};

void printDNSHeader(const DNSHeader& header);
SOCKET createSocket(const char* host, uint16_t port);

std::vector<char> createBogusQuestion(char* data, int name_len);
DomainName getName(char* data);

#endif // UTILS_H