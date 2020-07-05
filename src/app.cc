#include <include/base.h>
#include <include/config.h>
#include <include/server.h>
#include <include/sysfunc.h>
#include <include/instance.h>

int main(int argc, char *argv[])
{
  config::ConfigManager configManager;
  if (!configManager.initialize())
  {
    return 1;
  }

  base::Log logger(&configManager.GetLogConfig());
  if (logger.initialize())
  {
    return 1;
  }
  auto serverLog = base::Log::GetServerLogger();

  sysfunc::SystemFuncManager sysfuncManager(configManager.GetSysFunctionConfig());
  if (!sysfuncManager.initialize())
  {
    serverLog->error("System function manager initialized failure.");
    return 1;
  }

  instance::IntanceManager instances(configManager.GetInstanceConfig(), sysfuncManager);
  if (!instances.initialize())
  {
    serverLog->error("Instance manager initialized failure.");
    return 1;
  }

  server::Runtime runtime(configManager.GetServerConfig(), &instances);
  runtime.start();
  return 0;
}
