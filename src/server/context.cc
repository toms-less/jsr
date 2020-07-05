#include <include/server.h>

server::Context::Context(protos::RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instances)
    : service_(service), cq_(cq), writer_(&ctx_), status_(CREATE)
{
    this->instances_ = instances;
    dispatch();
}

void server::Context::dispatch()
{
    switch (status_)
    {
    case CREATE:
    {
        status_ = PROCESS;
        service_->RequestRoute(&ctx_, &request_, &writer_, cq_, cq_, this);
        break;
    }
    case PROCESS:
    {
        new server::Context(service_, cq_, instances);
        switch (request_.type())
        {
        case protos::Common_Type::Common_Type_CALL:
        {
            response_.set_type(protos::Common_Type::Common_Type_CALL);
            response_.set_id(request_.id());
            call_handler();
            break;
        }
        case protos::Common_Type::Common_Type_SCRIPT:
        {
            response_.set_type(protos::Common_Type::Common_Type_SCRIPT);
            response_.set_id(request_.id());
            script_handler();
            break;
        }
        default:
        {
            response_.set_type(protos::Common_Type::Common_Type_SCRIPT);
            response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
            response_.set_message("Invalid request type.");
            response_.set_id(request_.id());
            status_ = FINISH;
            writer_.Finish(response_, grpc::Status::OK, this);
        }
        };
        break;
    }
    case FINISH:
    default:
    {
        GPR_ASSERT(status_ = FINISH);
        delete this;
    }
    };
}

void server::Context::call_handler()
{
    // build execution context of instance.
    instance::ExecuteContext context(&request_);
    instances->execute(context);
    switch (context.status())
    {
    case ExecuteStatus::FINISH:
    {
        response_.set_status(protos::Common_Status::Common_Status_OK);
        break;
    }
    case ExecuteStatus::INIT:
    case ExecuteStatus::BUSY:
    {
        response_.set_status(protos::Common_Status::Common_Status_SYSTEM_ERROR);
        response_.set_message(context.error());
        status_ = FINISH;
        writer_.Finish(response_, grpc::Status::OK, this);
        break;
    }
    case ExecuteStatus::ERROR:
    default:
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(context.error());
        status_ = FINISH;
        writer_.Finish(response_, grpc::Status::OK, this);
        break;
    }
    };
}

void server::Context::script_handler()
{
}