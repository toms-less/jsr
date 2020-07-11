#include <include/server.h>

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
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    // Get execution context.
    v8::Local<v8::External> execution_context_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *execution_context = static_cast<instance::ExecuteContext *>(execution_context_data->Value());

    // Get gRPC objects.
    protos::RuntimeResponse *response = execution_context->response();
    grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer = execution_context->writer();
    server::Context *ctx = reinterpret_cast<server::Context *>(execution_context->ctx_ptr());

    const int args_length = args.Length();
    if (args_length > 1)
    {
        const char *msg = "Parameters of 'send' function should be less than 1 or empty.";
        v8::Local<v8::Object> error = base::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    response->set_status(protos::Common_Status::Common_Status_OK);
    writer->Finish(*response, grpc::Status::OK, ctx);
}