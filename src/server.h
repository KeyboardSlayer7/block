#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdint>
#include <array>
#include <unordered_map>

#include "utils.h"

class Server
{
public:
    Server(const char* host, uint16_t port);
    ~Server();
    
    void run();
    void setDNSServer(const char* server_address);
private:
    const char* _dns_server_address;
    SOCKADDR_IN _dns_server; 
    SOCKET _socket;
    
    std::unordered_map<uint16_t, SOCKADDR_IN> _clients;
    std::array<char, KILOBYTE> _buffer;
    std::array<char, IPV4_ADDRESS_LENGTH> _address;    

    bool _running;
};

#endif // SERVER_H