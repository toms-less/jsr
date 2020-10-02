#include <include/base.h>
#include <include/config.h>
#include <include/server.h>
#include <include/binding.h>
#include <include/instance.h>

int main(int argc, char *argv[])
{
  config::ConfigManager config_manager;
  if (!config_manager.initialize())
  {
    return 1;
  }

  v8::V8::InitializeExternalStartupData(config_manager.instance_config()
                                            .startup_data()
                                            .c_str());
  std::unique_ptr<v8::Platform> platform(v8::platform::NewDefaultPlatform());
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  base::Log logger(&config_manager.log_config());
  if (!logger.initialize())
  {
    return 1;
  }
  auto server_log = base::Log::server_logger();

  instance::IntanceManager instance_manager(config_manager.instance_config());
  if (!instance_manager.initialize())
  {
    server_log->error("Instance manager initialized failure.");
    return 1;
  }

  binding::SystemFuncManager binding_manager(instance_manager);
  if (!binding_manager.initialize())
  {
    server_log->error("System function manager initialized failure.");
    return 1;
  }

  server::Runtime runtime(config_manager.server_config(), &instance_manager);
  runtime.start();
  return 0;
}
