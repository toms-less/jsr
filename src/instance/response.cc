#include <include/server.h>

void instance::HttpResponse::set_header(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeResponse *response = ctx->response();

    const int args_length = args.Length();
    if (args_length != 2)
    {
        const char *msg = "Parameters count of 'set_header' function should be only 2.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> name = args[0];
    v8::Local<v8::Value> value = args[1];
    if (!name->IsString() || !name->IsStringObject() || !value->IsString() || !value->IsStringObject())
    {
        const char *msg = "Parameters of 'set_header' function should be 2 string, 'name' and 'value'.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // convert name.
    std::string cname;
    if (name->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, name);
        cname.append(*utf8_str);
    }
    if (name->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, name.As<v8::StringObject>()->ValueOf());
        cname.append(*utf8_str);
    }

    // convert value.
    std::string cvalue;
    if (value->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, value);
        cvalue.append(*utf8_str);
    }
    if (value->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, value.As<v8::StringObject>()->ValueOf());
        cvalue.append(*utf8_str);
    }

    google::protobuf::Map<std::string, std::string> *headers = response->mutable_call()->mutable_headers();
    (*headers)[cname] = cvalue;
}

void instance::HttpResponse::set_headers(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeResponse *response = ctx->response();

    const int args_length = args.Length();
    if (args_length != 1)
    {
        const char *msg = "Parameters count of 'set_headers' function should be only 1.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> parameter = args[0];
    if (parameter->IsNull())
    {
        return;
    }
    if (parameter->IsUndefined())
    {
        const char *msg = "Parameters of 'set_headers' function is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!parameter->IsObject())
    {
        const char *msg = "Parameters of 'set_headers' function is not an object type.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    v8::Local<v8::Object> v8_param = parameter.As<v8::Object>();
    v8::MaybeLocal<v8::Array> maybe = v8_param->GetOwnPropertyNames(isolate->GetCurrentContext());
    if (maybe.IsEmpty())
    {
        return;
    }

    google::protobuf::Map<std::string, std::string> *headers = response->mutable_call()->mutable_headers();
    v8::Local<v8::Array> propertyNames = maybe.ToLocalChecked();
    for (uint32_t i = 0; i < propertyNames->Length(); i++)
    {
        v8::Local<v8::Value> name = propertyNames->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
        v8::Local<v8::Value> value = v8_param->Get(isolate->GetCurrentContext(), name).ToLocalChecked();
        if (!name->IsString() || !name->IsStringObject() || !value->IsString() || !value->IsStringObject())
        {
            /**
             * Clear all the headers that have been put into the map
             * before throw an exception to the user function script.
             * 
            */
            headers->clear();
            const char *msg = "Invalid parameters of 'set_headers' function, header's name or value should be a string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }

        std::string cname;
        if (name->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, name);
            cname.append(*utf8_str);
        }
        if (name->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, name.As<v8::StringObject>()->ValueOf());
            cname.append(*utf8_str);
        }

        std::string cvalue;
        if (value->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, value);
            cvalue.append(*utf8_str);
        }
        if (value->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, value.As<v8::StringObject>()->ValueOf());
            cvalue.append(*utf8_str);
        }
        (*headers)[cname] = cvalue;
    }
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
    v8::TryCatch try_catch(isolate);

    // Get execution context.
    v8::Local<v8::External> ctx_data = v8::Local<v8::External>::Cast(args.Data());
    instance::ExecuteContext *ctx = static_cast<instance::ExecuteContext *>(ctx_data->Value());

    // Get gRPC objects.
    protos::RuntimeRequest *request = ctx->request();
    protos::RuntimeResponse *response = ctx->response();
    grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer = ctx->writer();

    // prepare response.
    protos::CallResponse *call = new protos::CallResponse();
    call->set_function(request->call().function());
    call->set_version(request->call().version());
    call->set_group(request->call().group());
    call->set_unit(request->call().unit());
    call->set_runtime("JavaScript");

    const int args_length = args.Length();
    switch (args_length)
    {
    case 0:
    {
        /**
         * In this condition, user use this function
         * like 'response.send()' in their scripts.
         * So it will return an empty data to the client.
         *
        */
        // empty data.
        call->set_data("");
        break;
    }
    case 1:
    {
        v8::Local<v8::Value> parameter = args[0];
        if (parameter->IsUndefined())
        {
            /**
             * In this condition, user put an undefined
             * parameter as the parameter. It will
             * throw an exception to user script.
             * 
            */
            call->set_data("");
            const char *msg = "Parameters of 'send' is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            break;
        }
        if (parameter->IsNull())
        {
            /**
             * In this condition, user put an 'null'
             * parameter as the parameter. So it will 
             * return an empty data to the client.
             * 
            */
            // empty data.
            call->set_data("");
            break;
        }
        if (parameter->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, parameter);
            call->set_data(*utf8_str);
            break;
        }
        if (parameter->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, parameter.As<v8::StringObject>()->ValueOf());
            call->set_data(*utf8_str);
            break;
        }

        v8::Local<v8::String> v8_str;
        if (!v8::JSON::Stringify(isolate->GetCurrentContext(), parameter).ToLocal(&v8_str))
        {
            call->set_data("");
            v8::String::Utf8Value utf8_error(isolate, try_catch.Exception());
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", *utf8_error, *utf8_error);
            isolate->ThrowException(error);
            break;
        }
        v8::String::Utf8Value utf8_str(isolate, v8_str);
        call->set_data(*utf8_str);
        break;
    }
    default:
    {
        /**
         * In the user script, user parameters count of 'response.send'
         * must be less than 2. If user set more than 1 parameters, it will
         * throw an exception to user script.
         * 
        */
        call->set_data("");
        const char *msg = "Parameters of 'send' function should be less than 2 or empty.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
    }
    };

    response->set_allocated_call(call);
    response->set_status(protos::Common_Status::Common_Status_OK);
    writer->Finish(*response, grpc::Status::OK, reinterpret_cast<server::Context *>(ctx->ctx_ptr()));
}