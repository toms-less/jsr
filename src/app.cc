#include <include/base.h>
#include <include/config.h>
#include <include/server.h>
#include <include/sysfunc.h>
#include <include/instance.h>

int main(int argc, char *argv[])
{
  config::ConfigManager configManager;
  if (!configManager.Initialize())
  {
    return 1;
  }

  base::Log logger(&configManager.GetLogConfig());
  if (logger.Initialize())
  {
    return 1;
  }
  auto serverLog = base::Log::GetServerLogger();

  sysfunc::SystemFuncManager sysfuncManager(configManager.GetSysFunctionConfig());
  if (!sysfuncManager.Initialize())
  {
    serverLog->error("System function manager initialized failure.");
    return 1;
  }

  instance::IntanceManager instanceManager(configManager.GetInstanceConfig(), sysfuncManager);
  if (!instanceManager.initialize())
  {
    serverLog->error("Instance manager initialized failure.");
    return 1;
  }

  server::Runtime runtime(configManager.GetServerConfig(), &instanceManager);
  runtime.start();
  return 0;
}
