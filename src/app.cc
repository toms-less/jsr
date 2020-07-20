#include <include/base.h>
#include <include/config.h>
#include <include/server.h>
#include <include/sysfunc.h>
#include <include/instance.h>

int main(int argc, char *argv[])
{
  config::ConfigManager config_manager;
  if (!config_manager.initialize())
  {
    return 1;
  }

  v8::V8::InitializeExternalStartupData(config_manager.GetInstanceConfig()
                                            .GetStartupData()
                                            .c_str());
  std::unique_ptr<v8::Platform> platform(v8::platform::NewDefaultPlatform());
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  base::Log logger(&config_manager.GetLogConfig());
  if (!logger.initialize())
  {
    return 1;
  }
  auto server_log = base::Log::server_logger();

  sysfunc::SystemFuncManager sysfunc_manager(config_manager.GetSysFunctionConfig());
  if (!sysfunc_manager.initialize())
  {
    server_log->error("System function manager initialized failure.");
    return 1;
  }

  instance::IntanceManager instance_manager(config_manager.GetInstanceConfig(), sysfunc_manager);
  if (!instance_manager.initialize())
  {
    server_log->error("Instance manager initialized failure.");
    return 1;
  }

  server::Runtime runtime(config_manager.GetServerConfig(), &instance_manager);
  runtime.start();
  return 0;
}
