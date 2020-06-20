#include <include/sysfunc.h>

sysfunc::SystemFunctionConfig::SystemFunctionConfig()
{
}

sysfunc::SystemFunctionConfig::~SystemFunctionConfig()
{
}

void sysfunc::SystemFunctionConfig::SetRequireRemote(bool remote)
{
    this->requireRemote = remote;
}

bool sysfunc::SystemFunctionConfig::IsRequireRemote()
{
    return this->requireRemote;
}

void sysfunc::SystemFunctionConfig::SetRequireRemoteServer(const std::string &server)
{
    this->requireRemoteServer = server;
}

const std::string &sysfunc::SystemFunctionConfig::GetRequireRemoteServer()
{
    return this->requireRemoteServer;
}