#include <include/server.h>

server::Runtime::Runtime(server::ServerConfig &_config, instance::IntanceManager *instances)
{
    this->config = _config;
    this->instances_ = instances;
}

server::Runtime::~Runtime()
{
}

void server::Runtime::start()
{
    auto serverLog = base::Log::server_logger();
    if (!config.IsInited())
    {
        serverLog->error("current runtime server configuration has not been initialized.");
        return;
    }

    std::string address("127.0.0.1:");
    address.append(std::to_string(config.GetPort()));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    serverLog->info("runtime server started and listening on '{}' port.", config.GetPort());

    handler();
}

void server::Runtime::handler()
{
    unsigned cores = std::thread::hardware_concurrency();
    std::thread workers[cores];
    for (unsigned i = 0; i < cores; i++)
    {
        workers[i] = std::thread(handler_inner, this);
    }
    for (std::thread &worker : workers)
    {
        worker.join();
    }
}

void server::Runtime::handler_inner(Runtime *runtime)
{
    new server::Context(&runtime->service_, runtime->cq_.get(), runtime->instances_);
    void *tag;
    bool ok;
    while (true)
    {
        GPR_ASSERT(runtime->cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        static_cast<server::Context *>(tag)->dispatch();
    }
}
