#include <include/instance.h>

void instance::HttpRequest::system_info(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'system_info' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

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
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'content_type' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    args.GetReturnValue().Set(instance::Util::v8_str(isolate, request->call().contenttype().c_str()));
}

void instance::HttpRequest::method(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'method' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    args.GetReturnValue().Set(instance::Util::v8_str(isolate, request->call().method().c_str()));
}

void instance::HttpRequest::header(const v8::FunctionCallbackInfo<v8::Value> &args)
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

    const int args_length = args.Length();
    if (args_length != 1)
    {
        const char *msg = "Parameters count of 'header' function should be only 1.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> parameter = args[0];
    if (!parameter->IsString() || !parameter->IsStringObject())
    {
        const char *msg = "Parameters of 'header' function should be a string.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    const google::protobuf::Map<std::string, std::string> &headers = request->call().headers();
    if (parameter->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter);
        std::string name(*utf8_str);
        if (!headers.contains(name))
        {
            args.GetReturnValue().SetNull();
            return;
        }
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, headers.at(name).c_str()).ToLocalChecked());
        return;
    }
    if (parameter->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter.As<v8::StringObject>()->ValueOf());
        std::string name(*utf8_str);
        if (!headers.contains(name))
        {
            args.GetReturnValue().SetNull();
            return;
        }
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, headers.at(name).c_str()).ToLocalChecked());
        return;
    }
}

void instance::HttpRequest::headers(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'headers' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    const google::protobuf::Map<std::string, std::string> &headers = request->call().headers();
    v8::Local<v8::Object> v8_headers = v8::Object::New(isolate);
    for (auto &pair : headers)
    {
        v8_headers->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, pair.first.c_str()),
                        instance::Util::v8_str(isolate, pair.second.c_str()));
    }
    args.GetReturnValue().Set(v8_headers);
}

void instance::HttpRequest::cookie(const v8::FunctionCallbackInfo<v8::Value> &args)
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

    const int args_length = args.Length();
    if (args_length != 1)
    {
        const char *msg = "Parameters count of 'cookie' function should be only 1.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> parameter = args[0];
    if (!parameter->IsString() || !parameter->IsStringObject())
    {
        const char *msg = "Parameters of 'cookie' function should be a string.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    std::string name;
    if (parameter->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter);
        name.append(*utf8_str);
    }
    if (parameter->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter.As<v8::StringObject>()->ValueOf());
        name.append(*utf8_str);
    }

    const google::protobuf::RepeatedPtrField<protos::Cookie> &cookies = request->call().cookies();
    for (int i = 0; i < cookies.size(); i++)
    {
        const protos::Cookie &cookie = cookies[i];
        if (name == cookie.name())
        {
            // Build cookie object.
            v8::Local<v8::Object> v8_cookie = v8::Object::New(isolate);
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "name"),
                           instance::Util::v8_str(isolate, cookie.name().c_str()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "value"),
                           instance::Util::v8_str(isolate, cookie.value().c_str()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "domain"),
                           instance::Util::v8_str(isolate, cookie.domain().c_str()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "path"),
                           instance::Util::v8_str(isolate, cookie.path().c_str()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "expires"),
                           v8::Integer::New(isolate, cookie.expires()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "maxAge"),
                           v8::Integer::New(isolate, cookie.maxage()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "secure"),
                           v8::Boolean::New(isolate, cookie.secure()));
            v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "httpOnly"),
                           v8::Boolean::New(isolate, cookie.httponly()));
            args.GetReturnValue().Set(v8_cookie);
            return;
        }
    }
    args.GetReturnValue().SetNull();
}

void instance::HttpRequest::cookies(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'cookies' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    const google::protobuf::RepeatedPtrField<protos::Cookie> &cookies = request->call().cookies();
    v8::Local<v8::Array> v8_cookies = v8::Array::New(isolate);
    for (int i = 0; i < cookies.size(); i++)
    {
        const protos::Cookie &cookie = cookies[i];
        // Build cookie object.
        v8::Local<v8::Object> v8_cookie = v8::Object::New(isolate);
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "name"),
                       instance::Util::v8_str(isolate, cookie.name().c_str()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "value"),
                       instance::Util::v8_str(isolate, cookie.value().c_str()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "domain"),
                       instance::Util::v8_str(isolate, cookie.domain().c_str()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "path"),
                       instance::Util::v8_str(isolate, cookie.path().c_str()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "expires"),
                       v8::Integer::New(isolate, cookie.expires()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "maxAge"),
                       v8::Integer::New(isolate, cookie.maxage()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "secure"),
                       v8::Boolean::New(isolate, cookie.secure()));
        v8_cookie->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "httpOnly"),
                       v8::Boolean::New(isolate, cookie.httponly()));

        v8_cookies->Set(isolate->GetCurrentContext(), i, v8_cookie);
    }
    args.GetReturnValue().Set(v8_cookies);
}

void instance::HttpRequest::parameter(const v8::FunctionCallbackInfo<v8::Value> &args)
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

    const int args_length = args.Length();
    if (args_length != 1)
    {
        const char *msg = "Parameters count of 'parameter' function should be only 1.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> parameter = args[0];
    if (!parameter->IsString() || !parameter->IsStringObject())
    {
        const char *msg = "Parameters of 'parameter' function should be a string.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    const google::protobuf::Map<std::string, std::string> &parameters = request->call().parameters();
    if (parameter->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter);
        std::string name(*utf8_str);
        if (!parameters.contains(name))
        {
            args.GetReturnValue().SetNull();
            return;
        }
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, parameters.at(name).c_str()).ToLocalChecked());
        return;
    }
    if (parameter->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, parameter.As<v8::StringObject>()->ValueOf());
        std::string name(*utf8_str);
        if (!parameters.contains(name))
        {
            args.GetReturnValue().SetNull();
            return;
        }
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, parameters.at(name).c_str()).ToLocalChecked());
        return;
    }
}

void instance::HttpRequest::parameters(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'parameters' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    const google::protobuf::Map<std::string, std::string> &parameters = request->call().parameters();
    v8::Local<v8::Object> v8_parameters = v8::Object::New(isolate);
    for (auto &pair : parameters)
    {
        v8_parameters->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, pair.first.c_str()),
                           instance::Util::v8_str(isolate, pair.second.c_str()));
    }
    args.GetReturnValue().Set(v8_parameters);
}

void instance::HttpRequest::data(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    const int args_length = args.Length();
    if (args_length != 0)
    {
        const char *msg = "'data' function should have no parameter.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    args.GetReturnValue().Set(instance::Util::v8_str(isolate, request->call().data().c_str()));
}