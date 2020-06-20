#pragma once
#include <include/base.h>
#include <include/server.h>
#include <include/instance.h>
#include <include/sysfunc.h>

namespace config
{

/**
 * configuration manager.
 * 
*/
class ConfigManager : public base::Module
{
public:
    ConfigManager();
    ~ConfigManager();

    /**
     * initialize configuration.
     * configuration like below in 'config.yaml'.
     * 
     * server:
     *   port: 6666
     * 
     * log:
     *   debug: "logs/debug.log"
     *   server: "logs/server.log"
     *   instance: "logs/instance.log"
     *   maxSize: 50
     *   rotate: 10
     * 
     * instance:
     *   startupData: "data/"
     *   stackLimit: 1000
     *   physicalMemory: 2048
     *   virtualMemoryLimit: 0
     *   maxOldeGenerationSize: 500
     *   initialOldGenerationSize: 200
     *   maxYounGenerationSize: 500
     *   initialYoungGenerationSize: 200
     *   codeRangeSize: 500
     *   maxZonePoolSize: 500
     *   instanceCount: 50
     * 
     * sysfunc:
     *   requireRemote: false
     *   requireRemoteServer: ""
     * 
    */
    virtual bool Initialize();

    /**
     * get server configuration.
     * 
    */
    server::ServerConfig &GetServerConfig();

    /**
     * get log configuration.
     * 
    */
    base::LogConfig &GetLogConfig();

    /**
     * get v8 instance configuration.
     * 
    */
    instance::InstanceConfig &GetInstanceConfig();

    /**
     * get system function configuration.
     * 
    */
    sysfunc::SystemFunctionConfig &GetSysFunctionConfig();

private:
    /**
     * runtime service configuration.
     * 
    */
    server::ServerConfig serverConfig;

    /**
     * log configuration.
     * 
    */
    base::LogConfig logConfig;

    /**
     * v8 instance configuration.
     * 
    */
    instance::InstanceConfig instanceConfig;

    /**
     * system function configuration.
     * 
    */
    sysfunc::SystemFunctionConfig sysfunctionConfig;
};
} // namespace config