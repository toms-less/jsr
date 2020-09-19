#include <include/instance.h>

instance::IntanceManager::IntanceManager(InstanceConfig &_config)
{
    this->config = _config;
}

instance::IntanceManager::~IntanceManager()
{
    for (std::vector<instance::Instance *>::iterator it = instances_.begin(); it != instances_.end(); it++)
    {
        delete (*it);
    }
    std::vector<instance::Instance *>().swap(instances_);
    while (!queue_.empty())
    {
        queue_.pop();
    }
}

bool instance::IntanceManager::initialize()
{
    auto instance_log = base::Log::instance_logger();
    int instance_count = config.instance_count();
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
        instances_.push_back(current);
        queue_.push_back(current);
    }
    if (instances_.empty())
    {
        instance_log->error("Instance initialize failure, current instance list is empty.");
        return false;
    }

    instance_log->info("Instances initialized sucessfully, current instance list size is {}.", instances_.size());
    inited = true;
    return inited;
}

void instance::IntanceManager::compile(CompileContext &context)
{
    // compile functions for all instances.
    for (instance::Instance *instance : instances_)
    {
        instance->compile(context);
        if (!context.ok())
        {
            return;
        }
    }
}

void instance::IntanceManager::uncompile(UncompileContext &context)
{
    // Remove the compiled functions for all instances.
    for (instance::Instance *instance : instances_)
    {
        instance->uncompile(context);
        if (!context.ok())
        {
            return;
        }
    }
}

void instance::IntanceManager::execute(ExecuteContext &context)
{
    if (context.working_instance() != nullptr)
    {
        std::string error("Invalid execute working instance.");
        context.set_error(error);
        return;
    }
    if (context.status() != ExecuteStatus::INIT)
    {
        std::string error("Invalid execute status.");
        context.set_error(error);
        return;
    }
    instance::Instance *instance = queue_.pop();
    if (instance == nullptr)
    {
        context.set_status(ExecuteStatus::BUSY);
        return;
    }

    /**
     * Keep current v8 instance in context
     * for Recycling it and push back to instance queue
     * when current requesting is finished.
     * 
    */
    context.set_working_instance(instance);
    instance->Execute(context);
}

base::BlockingQueue<instance::Instance *> *instance::IntanceManager::queue()
{
    return &queue_;
}

std::vector<instance::Instance *> &instance::IntanceManager::instances()
{
    return instances_;
}