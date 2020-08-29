#include <include/server.h>
#include <include/module.h>

server::Context::Context(protos::RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instances)
    : service_(service), cq_(cq), writer_(&ctx_), instances_(instances), status_(CREATE)
{
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
        new server::Context(service_, cq_, instances_);
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
    instance::ExecuteContext context(&request_, &response_, &writer_, instances_->queue(), (intptr_t)this);
    instances_->execute(context);
    switch (context.status())
    {
    case instance::ExecuteStatus::FINISH:
    {
        /**
         * This condition often happend when user did not call 'resonse.send(xxx)'.
         * As the response status is only setted when the 'HttpResponse::send' callback
         * are invoked, so it should be check in this place.
         * 
        */
        if (response_.status() == protos::Common_Status::Common_Status_INIT)
        {
            response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
            response_.set_message("User should user 'response.send(foo)' in the functions.");
            writer_.Finish(response_, grpc::Status::OK, this);
            status_ = FINISH;
        }
        break;
    }
    case instance::ExecuteStatus::INIT:
    case instance::ExecuteStatus::BUSY:
    {
        response_.set_status(protos::Common_Status::Common_Status_SYSTEM_ERROR);
        response_.set_message(context.error());
        writer_.Finish(response_, grpc::Status::OK, this);
        status_ = FINISH;
    }
    case instance::ExecuteStatus::ERROR:
    default:
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(context.error());
        writer_.Finish(response_, grpc::Status::OK, this);
        status_ = FINISH;
    }
    };
    /**
     * Recycle current v8 instance and push it into instances queue.
     * 
    */
    if (context.working_instance() != nullptr)
    {
        context.queue()->push_back(context.working_instance());
    }
}

void server::Context::script_handler()
{
    const google::protobuf::RepeatedPtrField<protos::ScriptRequest> &script_requests = request_.script();
    if (script_requests.size() <= 0)
    {
        response_.set_status(protos::Common_Status::Common_Status_OK);
        response_.set_message("No scripts action.");
        writer_.Finish(response_, grpc::Status::OK, this);
        status_ = FINISH;
        return;
    }

    for (int i = 0; i < script_requests.size(); i++)
    {
        const protos::ScriptRequest &script_request = script_requests.Get(i);
        switch (script_request.action())
        {
        case protos::ScriptRequest_ActionType_UPDATE:
        {
            if (!update_script(script_request.javascript()))
            {
                break;
            }
            continue;
        }
        case protos::ScriptRequest_ActionType_ADD:
        {
            if (!add_script(script_request.javascript()))
            {
                break;
            }
            continue;
        }
        case protos::ScriptRequest_ActionType_DELETE:
        {
            if (!delete_script(script_request.javascript()))
            {
                break;
            }
            continue;
        }
        default:
            continue;
        }
    }
    writer_.Finish(response_, grpc::Status::OK, this);
    status_ = FINISH;
}

bool server::Context::update_script(const protos::JavaScript &script)
{
    /**
     * TODO:
     * 1. add parsing cache;
     * 2. parse script asynchronously.
     * 
    */
    module::DepsParseContext parse_context(script.workspace().repository().c_str(), script.url().c_str());
    module::ScriptModule script_module;
    script_module.deps_parse(parse_context);
    if (!parse_context.ok())
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(parse_context.error());
        return false;
    }

    instance::CompileContext context(script);
    context.set_script_content(parse_context.script());
    instances_->compile(context);
    if (!context.ok())
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(context.error());
        return false;
    }
    response_.set_status(protos::Common_Status::Common_Status_OK);
    // TODO set compiled script info.
    return true;
}

bool server::Context::add_script(const protos::JavaScript &script)
{
    /**
     * TODO:
     * 1. add parsing cache;
     * 2. parse script asynchronously.
     * 
    */
    module::DepsParseContext parse_context(script.workspace().repository().c_str(), script.url().c_str());
    module::ScriptModule script_module;
    script_module.deps_parse(parse_context);
    if (!parse_context.ok())
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(parse_context.error());
        return false;
    }

    instance::CompileContext context(script);
    context.set_script_content(parse_context.script());
    instances_->compile(context);
    if (!context.ok())
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(context.error());
        return false;
    }
    response_.set_status(protos::Common_Status::Common_Status_OK);
    // TODO set compiled script info.
    return true;
}

bool server::Context::delete_script(const protos::JavaScript &script)
{
    instance::UncompileContext context(script);
    instances_->uncompile(context);
    if (!context.ok())
    {
        response_.set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response_.set_message(context.error());
        return false;
    }
    response_.set_status(protos::Common_Status::Common_Status_OK);
    // TODO set uncompiled script info.
    return true;
}

void server::Context::set_status(const server::Context::Status &status)
{
    status_ = status;
}