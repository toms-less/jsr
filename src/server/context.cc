#include <include/server.h>

server::Context::Context(RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq)
    : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
{
    Proceed();
}

void server::Context::Proceed()
{
    if (status_ == CREATE)
    {
        status_ = PROCESS;
    }
    else if (status_ == PROCESS)
    {
        new server::Context(service_, cq_);
        status_ = FINISH;
    }
    else
    {
        GPR_ASSERT(status_ = FINISH);
        delete this;
    }
}