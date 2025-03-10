#ifndef UTILS_H
#define UTILS_H

#include <WinSock2.h>
#include <cstdint>

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

void printDNSHeader(const DNSHeader& header);
SOCKET createSocket(const char* host, uint16_t port);

#endif // UTILS_H