#include <include/instance.h>

void instance::HttpResponse::set_header(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::set_headers(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::set_cookie(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::set_cookies(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::set_status(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::set_content_type(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpResponse::send(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // Get execution context.
    v8::Local<v8::External> execution_context_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *execution_context = static_cast<instance::ExecuteContext *>(execution_context_data->Value());

    // Get gRPC objects.
    protos::RuntimeResponse *response = execution_context->response();
    grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer = execution_context->writer();

    const int args_length = args.Length();
    if (args_length != 1)
    {
        response->set_status(protos::Common_Status::Common_Status_USER_ERROR);
        response->set_message("Calling 'response.send(foo)' in the user function script should be only had one parameter.");
    }

    response->set_status(protos::Common_Status::Common_Status_OK);
}