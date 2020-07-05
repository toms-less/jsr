#include <include/instance.h>

instance::IntanceManager::IntanceManager(InstanceConfig &_config, sysfunc::SystemFuncManager &_sysFuncManager)
    : sysFuncManager(_sysFuncManager)
{
    this->config = _config;
}

instance::IntanceManager::~IntanceManager()
{
    for (std::vector<instance::Instance *>::iterator it = instances.begin(); it != instances.end(); it++)
    {
        delete (*it);
    }
    std::vector<instance::Instance *>().swap(instances);
    while (!idle.empty())
    {
        idle.pop();
    }
    while (!working.empty())
    {
        idle.pop();
    }
}

bool instance::IntanceManager::initialize()
{
    auto instanceLog = base::Log::GetInstanceLogger();
    if (!config.IsInited())
    {
        instanceLog->error("Instance configuration has not been initialized.");
        return false;
    }

    // initialize v8 globally.
    v8::V8::InitializeExternalStartupData(config.GetStartupData().c_str());
    std::unique_ptr<v8::Platform> platform(v8::platform::NewDefaultPlatform());
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    int instanceCount = config.GetInstanceCount();
    if (instanceCount == 0)
    {
        instanceLog->error("Instance count in the configuration is invalid, current count is 0.");
        return false;
    }

    // initialize v8 instance.
    for (int i = 0; i < instanceCount; i++)
    {
        instance::Instance *current = new instance::Instance(config);
        if (!current->Initialize())
        {
            delete current;
            instanceLog->error("Instance initialize failure.");
            continue;
        }

        // bind system functions.
        instance::BindContext context(sysFuncManager.GetObjectFuncList(), sysFuncManager.GetPureFuncList());
        current->Bind(context);
        if (!context.IsSuccess())
        {
            delete current;
            instanceLog->error("Instance binding system functions failure, detail: {}.", context.GetError());
            continue;
        }
        instances.push_back(current);
        idle.push_back(current);
    }
    if (instances.empty())
    {
        instanceLog->error("Instance initialize failure, current instance list is empty.");
        return false;
    }

    // TODO: compile all function scripts.

    instanceLog->error("Instances initialized sucessfully, current instance list size is {}.", instances.size());
    inited = true;
    return inited;
}

void instance::IntanceManager::compile(CompileContext &context)
{
    if (!config.IsInited())
    {
        std::string error("Instance manager has not been initialized.");
        context.SetSuccess(false);
        context.SetError(error);
        return;
    }

    // compile functions for all instances.
    for (instance::Instance *instance : instances)
    {
        instance->Compile(context);
        if (!context.IsSuccess())
        {
            return;
        }
    }
}

void instance::IntanceManager::execute(ExecuteContext &context)
{
    if (context.status() != ExecuteStatus::INIT)
    {
        std::string error("Invalid execute status.");
        context.set_error(error);
        return;
    }
    instance::Instance *instance = idle.pop();
    if (instance == nullptr)
    {
        context.set_status(ExecuteStatus::BUSY);
        return;
    }
    instance->Execute(context);
}