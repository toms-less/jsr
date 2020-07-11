#include <include/config.h>
#include "yaml-cpp/yaml.h"

config::ConfigManager::ConfigManager()
{
}

config::ConfigManager::~ConfigManager()
{
}

bool config::ConfigManager::initialize()
{
    // read configuration from 'config/config.yaml' file.
    YAML::Node config = YAML::LoadFile("config/config.yaml");
    YAML::Node server = config["server"];

    // set configuration to internal class.
    serverConfig.SetPort(config["server"]["port"].as<int>());
    serverConfig.SetInited(true);

    logConfig.SetDebugLog(config["log"]["debug"].as<std::string>());
    logConfig.SetServerLog(config["log"]["server"].as<std::string>());
    logConfig.SetInstanceLog(config["log"]["instance"].as<std::string>());
    logConfig.SetMaxSize(config["log"]["maxSize"].as<int>());
    logConfig.SetRotate(config["log"]["rotate"].as<int>());
    logConfig.SetInited(true);

    instanceConfig.SetStartupData(config["instance"]["startupData"].as<std::string>());
    instanceConfig.SetStackLimit(config["instance"]["stackLimit"].as<uint32_t>());
    instanceConfig.SetPhysicalMemory(config["instance"]["physicalMemory"].as<uint64_t>());
    instanceConfig.SetVirtualMemoryLimit(config["instance"]["virtualMemoryLimit"].as<uint64_t>());
    instanceConfig.SetMaxOldeGenerationSize(config["instance"]["maxOldeGenerationSize"].as<uint64_t>());
    instanceConfig.SetInitialOldGenerationSize(config["instance"]["initialOldGenerationSize"].as<uint64_t>());
    instanceConfig.SetMaxYounGenerationSize(config["instance"]["maxYounGenerationSize"].as<uint64_t>());
    instanceConfig.SetInitialYoungGenerationSize(config["instance"]["initialYoungGenerationSize"].as<uint64_t>());
    instanceConfig.SetCodeRangeSize(config["instance"]["codeRangeSize"].as<uint64_t>());
    instanceConfig.SetMaxZonePoolSize(config["instance"]["maxZonePoolSize"].as<uint64_t>());
    instanceConfig.SetInstanceCount(config["instance"]["instanceCount"].as<uint32_t>());
    instanceConfig.SetInited(true);

    sysfunctionConfig.SetRequireRemote(config["sysfunc"]["requireRemote"].as<bool>());
    sysfunctionConfig.SetRequireRemoteServer(config["sysfunc"]["requireRemoteServer"].as<std::string>());
    sysfunctionConfig.SetInited(true);
    return true;
}

server::ServerConfig &config::ConfigManager::GetServerConfig()
{
    return this->serverConfig;
}

base::LogConfig &config::ConfigManager::GetLogConfig()
{
    return this->logConfig;
}

instance::InstanceConfig &config::ConfigManager::GetInstanceConfig()
{
    return this->instanceConfig;
}

sysfunc::SystemFunctionConfig &config::ConfigManager::GetSysFunctionConfig()
{
    return this->sysfunctionConfig;
}