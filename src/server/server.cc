#include <include/server.h>

server::Runtime::Runtime(server::ServerConfig &_config)
{
    this->config = _config;
}

server::Runtime::~Runtime()
{
}

void server::Runtime::Start()
{
    auto serverLog = base::Log::GetServerLogger();
    if (!config.IsInited())
    {
        serverLog->error("current runtime server configuration has not been initialized.");
        return;
    }

    std::string address("0.0.0.0:");
    address.append(std::to_string(config.GetPort()));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    serverLog->info("runtime server started and listening on '{}' port.", config.GetPort());

    ServerHandle();
}

void server::Runtime::ServerHandle()
{
    new server::Context(&service_, cq_.get());
    void *tag;
    bool ok;
    while (true)
    {
        GPR_ASSERT(cq_->Next(&tag, &ok));
        GPR_ASSERT(ok);
        static_cast<server::Context *>(tag)->Proceed();
    }
}