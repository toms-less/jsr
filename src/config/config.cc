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
    server_config_.set_port(config["server"]["port"].as<int>());

    log_config_.set_debug_log(config["log"]["debug"].as<std::string>());
    log_config_.set_server_log(config["log"]["server"].as<std::string>());
    log_config_.set_instance_log(config["log"]["instance"].as<std::string>());
    log_config_.set_max_size(config["log"]["max_size"].as<int>());
    log_config_.set_rotate(config["log"]["rotate"].as<int>());

    instance_config_.set_startup_data(config["instance"]["startup_data"].as<std::string>());
    instance_config_.set_stack_limit(config["instance"]["stack_limit"].as<uint32_t>());
    instance_config_.set_physical_memory(config["instance"]["physical_memory"].as<uint64_t>());
    instance_config_.set_virtual_memory_limit(config["instance"]["virtual_memory_limit"].as<uint64_t>());
    instance_config_.set_max_old_generation_size(config["instance"]["max_old_generation_size"].as<uint64_t>());
    instance_config_.set_initial_old_generation_size(config["instance"]["initial_old_generation_size"].as<uint64_t>());
    instance_config_.set_max_young_generation_size(config["instance"]["max_young_generation_size"].as<uint64_t>());
    instance_config_.set_initial_young_generation_size(config["instance"]["initial_young_generation_size"].as<uint64_t>());
    instance_config_.set_instance_count(config["instance"]["instance_count"].as<uint32_t>());
    return true;
}

server::ServerConfig &config::ConfigManager::server_config()
{
    return server_config_;
}

base::LogConfig &config::ConfigManager::log_config()
{
    return log_config_;
}

instance::InstanceConfig &config::ConfigManager::instance_config()
{
    return instance_config_;
}