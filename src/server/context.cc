#include <include/server.h>

server::Context::Context(RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instanceManager)
    : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
{
    this->instanceManager = instanceManager;
    Proceed();
}

void server::Context::Proceed()
{
    if (status_ == CREATE)
    {
        status_ = PROCESS;
        service_->RequestCall(&ctx_, &request_, &responder_, cq_, cq_, this);
    }
    else if (status_ == PROCESS)
    {
        new server::Context(service_, cq_, instanceManager);
        status_ = FINISH;
    }
    else
    {
        GPR_ASSERT(status_ = FINISH);
        delete this;
    }
}