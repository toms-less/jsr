#pragma once
#include <include/base.h>
#include <include/server.h>
#include <include/instance.h>
#include <include/binding.h>

namespace config
{

    /**
 * configuration manager.
 * 
*/
    class ConfigManager
    {
    public:
        ConfigManager();
        ~ConfigManager();

        /**
         * Initialize configuration.
         * configuration like below in 'config.yaml'.
         * 
         * server:
         *   port: 6666
         * log:
         *   debug: logs/debug.log
         *   server: logs/server.log
         *   instance: logs/instance.log
         *   max_size: 50 # in MB.
         *   rotate: 10
         * instance:
         *   startup_data: data/
         *   stack_limit: 1000
         *   physical_memory: 2048
         *   virtual_memory_limit: 0
         *   max_old_generation_size: 500
         *   initial_old_generation_size: 200
         *   max_young_generation_size: 500
         *   initial_young_generation_size: 200
         *   instance_count: 10
         */
        bool initialize();

        /**
         * get server configuration.
         * 
         */
        server::ServerConfig &server_config();

        /**
         * get log configuration.
         * 
         */
        base::LogConfig &log_config();

        /**
         * get v8 instance configuration.
         * 
         */
        instance::InstanceConfig &instance_config();

    private:
        /**
         * runtime service configuration.
         *
         */
        server::ServerConfig server_config_;

        /**
         * log configuration.
         *
         */
        base::LogConfig log_config_;

        /**
         * v8 instance configuration.
         *
         */
        instance::InstanceConfig instance_config_;
    };
} // namespace config