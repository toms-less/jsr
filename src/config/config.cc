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
    YAML::Node log = config["log"];
    YAML::Node instance = config["instance"];
    YAML::Node sysfunc = config["sysfunc"];
    if (!server.IsDefined() || !log.IsDefined() || !instance.IsDefined() || !sysfunc.IsDefined())
    {
        return false;
    }

    // check server configuration.
    YAML::Node serverPort = server["port"];
    if (!serverPort.IsDefined())
    {
        return false;
    }

    // check log configuration.
    YAML::Node logDebug = log["debug"];
    YAML::Node logServer = log["server"];
    YAML::Node logInstance = log["instance"];
    YAML::Node logMaxSize = log["maxSize"];
    YAML::Node logRotate = log["rotate"];
    if (!logDebug.IsDefined() || !logServer.IsDefined() || !logInstance.IsDefined() || !logMaxSize.IsDefined() || !logRotate.IsDefined())
    {
        return false;
    }

    // check instance configuration.
    YAML::Node instanceStartupData = instance["startupData"];
    YAML::Node instanceStackLimit = instance["stackLimit"];
    YAML::Node instancePhysicalMemory = instance["physicalMemory"];
    YAML::Node instanceVirtualMemoryLimit = instance["virtualMemoryLimit"];
    YAML::Node instanceMaxOldeGenerationSize = instance["maxOldeGenerationSize"];
    YAML::Node instanceInitialOldGenerationSize = instance["initialOldGenerationSize"];
    YAML::Node instanceMaxYounGenerationSize = instance["maxYounGenerationSize"];
    YAML::Node instanceInitialYoungGenerationSize = instance["initialYoungGenerationSize"];
    YAML::Node instanceCodeRangeSize = instance["codeRangeSize"];
    YAML::Node instanceMaxZonePoolSize = instance["maxZonePoolSize"];
    YAML::Node instanceCount = instance["instanceCount"];
    if (!instanceStartupData.IsDefined() || !instanceStackLimit.IsDefined() || !instancePhysicalMemory.IsDefined() || !instanceVirtualMemoryLimit.IsDefined() || !instanceMaxOldeGenerationSize.IsDefined() || !instanceInitialOldGenerationSize.IsDefined() || !instanceMaxYounGenerationSize.IsDefined() || !instanceInitialYoungGenerationSize.IsDefined() || !instanceCodeRangeSize.IsDefined() || !instanceMaxZonePoolSize.IsDefined() || !instanceCount.IsDefined())
    {
        return false;
    }

    // check system function configuration.
    YAML::Node sysfuncRemote = sysfunc["requireRemote"];
    YAML::Node sysfuncRemoteServer = sysfunc["requireRemoteServer"];
    if (!sysfuncRemote.IsDefined() || !sysfuncRemoteServer.IsDefined())
    {
        return false;
    }

    // set configuration to internal class.
    serverConfig.SetPort(serverPort.as<int>());

    logConfig.SetDebugLog(logDebug.as<std::string>());
    logConfig.SetServerLog(logServer.as<std::string>());
    logConfig.SetInstanceLog(logInstance.as<std::string>());
    logConfig.SetMaxSize(logMaxSize.as<int>());
    logConfig.SetRotate(logRotate.as<int>());

    instanceConfig.SetStartupData(instanceStartupData.as<std::string>());
    instanceConfig.SetStackLimit(instanceStackLimit.as<uint32_t>());
    instanceConfig.SetPhysicalMemory(instancePhysicalMemory.as<uint64_t>());
    instanceConfig.SetVirtualMemoryLimit(instanceVirtualMemoryLimit.as<uint64_t>());
    instanceConfig.SetMaxOldeGenerationSize(instanceMaxOldeGenerationSize.as<uint64_t>());
    instanceConfig.SetInitialOldGenerationSize(instanceInitialOldGenerationSize.as<uint64_t>());
    instanceConfig.SetMaxYounGenerationSize(instanceMaxYounGenerationSize.as<uint64_t>());
    instanceConfig.SetInitialYoungGenerationSize(instanceInitialYoungGenerationSize.as<uint64_t>());
    instanceConfig.SetCodeRangeSize(instanceCodeRangeSize.as<uint64_t>());
    instanceConfig.SetMaxZonePoolSize(instanceMaxZonePoolSize.as<uint64_t>());
    instanceConfig.SetInstanceCount(instanceCount.as<uint32_t>());

    sysfunctionConfig.SetRequireRemote(sysfuncRemote.as<bool>());
    sysfunctionConfig.SetRequireRemoteServer(sysfuncRemoteServer.as<std::string>());
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