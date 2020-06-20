#include <include/server.h>

void server::ServerConfig::SetPort(int _port)
{
    this->port = _port;
}

int server::ServerConfig::GetPort()
{
    return this->port;
}