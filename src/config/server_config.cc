#include <include/server.h>

void server::ServerConfig::set_port(int port)
{
    port_ = port;
}

int server::ServerConfig::port()
{
    return port_;
}