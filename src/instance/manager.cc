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
    auto instance_log = base::Log::instance_logger();
    if (!config.IsInited())
    {
        instance_log->error("Instance configuration has not been initialized.");
        return false;
    }

    int instance_count = config.GetInstanceCount();
    if (instance_count == 0)
    {
        instance_log->error("Instance count in the configuration is invalid, current count is 0.");
        return false;
    }

    // initialize v8 instance.
    for (int i = 0; i < instance_count; i++)
    {
        instance::Instance *current = new instance::Instance(config);
        if (!current->Initialize())
        {
            delete current;
            instance_log->error("Instance initialize failure.\n");
            continue;
        }

        // bind system functions.
        instance::BindContext context(sysFuncManager.GetObjectFuncList(), sysFuncManager.GetPureFuncList());
        current->Bind(context);
        if (!context.IsSuccess())
        {
            delete current;
            instance_log->error("Instance binding system functions failure, detail: {}.", context.GetError());
            continue;
        }
        instances.push_back(current);
        idle.push_back(current);
    }
    if (instances.empty())
    {
        instance_log->error("Instance initialize failure, current instance list is empty.");
        return false;
    }

    // TODO: compile all function scripts, example for testing below.
    instance::CompileContext compile_ctx("function home(request, response) {try {let headers = request.headers();response.send(JSON.stringify(headers));} catch (error) {}}");
    for (int i = 0; i < instance_count; i++)
    {
        instance::Instance *instance = instances[i];
        instance->Compile(compile_ctx);
        if (!compile_ctx.ok())
        {
            instance_log->error("Compile script error when initialized instance, error {}.", compile_ctx.error());
            continue;
        }
        instance_log->info("Compile script sucessfully when initialized instance, script {}.", compile_ctx.script());
    }

    instance_log->info("Instances initialized sucessfully, current instance list size is {}.", instances.size());
    inited = true;
    return inited;
}

void instance::IntanceManager::compile(CompileContext &context)
{
    if (!config.IsInited())
    {
        context.set_error("Instance manager has not been initialized.");
        return;
    }

    // compile functions for all instances.
    for (instance::Instance *instance : instances)
    {
        instance->Compile(context);
        if (!context.ok())
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