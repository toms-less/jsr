#include <include/instance.h>

void instance::HttpRequest::system_info(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();

    // Build system info object.
    v8::Local<v8::Object> info = v8::Object::New(isolate);
    info->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "name"),
              instance::Util::v8_str(isolate, request->call().function().c_str()));
    info->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "version"),
              instance::Util::v8_str(isolate, request->call().version().c_str()));
    info->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "group"),
              instance::Util::v8_str(isolate, request->call().group().c_str()));
    info->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "unit"),
              instance::Util::v8_str(isolate, request->call().unit().c_str()));
    info->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "timeout"),
              v8::Integer::New(isolate, request->call().timeout()));
    args.GetReturnValue().Set(info);
}

void instance::HttpRequest::content_type(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::method(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::header(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::headers(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::cookie(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::cookies(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::parameter(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::parameters(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::data(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}