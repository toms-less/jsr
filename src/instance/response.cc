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
    if (name->IsNull() || value->IsNull())
    {
        return;
    }
    if (name->IsUndefined() || value->IsUndefined())
    {
        const char *msg = "Parameters of 'set_header' are invalid, header name or value is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    if (!name->IsString() && !name->IsStringObject())
    {
        const char *msg = "Parameters of 'set_header' are invalid, header name should be a string value.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!value->IsString() && !value->IsStringObject())
    {
        const char *msg = "Parameters of 'set_header' are invalid, header value should be a string value.";
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
        const char *msg = "Parameters of 'set_headers' are invalid, only one parameter is permitted.";
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
        const char *msg = "Parameters of 'set_headers' are invalid, parameter value is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!parameter->IsObject())
    {
        const char *msg = "Parameters of 'set_headers' are invalid, parameter value is not an object type.";
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
        if (name->IsNull() || value->IsNull())
        {
            continue;
        }
        if (name->IsUndefined() || value->IsUndefined())
        {
            /**
             * Clear all the headers that have been put into the map
             * before throw an exception to the user function script.
             * 
            */
            headers->clear();
            const char *msg = "Invalid parameters of 'set_headers' are invalid, header name or value in the parameter is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (!name->IsString() && !name->IsStringObject())
        {
            /**
             * Clear all the headers that have been put into the map
             * before throw an exception to the user function script.
             * 
            */
            headers->clear();
            const char *msg = "Invalid parameters of 'set_headers' are invalid, header name should be a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (!value->IsString() && !value->IsStringObject())
        {
            /**
             * Clear all the headers that have been put into the map
             * before throw an exception to the user function script.
             * 
            */
            headers->clear();
            const char *msg = "Invalid parameters of 'set_headers' are invalid, header value should be a string type.";
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
        const char *msg = "Parameters count of 'set_cookie' are invalid, only one parameter is permitted.";
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
        const char *msg = "Parameters of 'set_cookie' function is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!parameter->IsObject())
    {
        const char *msg = "Parameters of 'set_cookie' function is not an object type.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    v8::Local<v8::Object> v8_param = parameter.As<v8::Object>();
    google::protobuf::RepeatedPtrField<protos::Cookie> *cookies = response->mutable_call()->mutable_cookies();
    protos::Cookie *cookie = new protos::Cookie();

    /**
     * Cookie name.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_name = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "name"));
    if (!maybe_name.IsEmpty())
    {
        v8::Local<v8::Value> name = maybe_name.ToLocalChecked();
        if (name->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie name is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (name->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie name is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!name->IsString() && !name->IsStringObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie name should be a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (name->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, name);
            cookie->set_name(*utf8_str);
        }
        if (name->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, name.As<v8::StringObject>()->ValueOf());
            cookie->set_name(*utf8_str);
        }
    }
    else
    {
        const char *msg = "Invalid parameters of 'set_cookie' are invalid, there is no cookie name.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        delete cookie;
        return;
    }

    /**
     * Cookie value.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_value = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "value"));
    if (!maybe_value.IsEmpty())
    {
        v8::Local<v8::Value> value = maybe_value.ToLocalChecked();
        if (value->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie value is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (value->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie value is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!value->IsString() && !value->IsStringObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie value should be a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (value->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, value);
            cookie->set_value(*utf8_str);
        }
        if (value->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, value.As<v8::StringObject>()->ValueOf());
            cookie->set_value(*utf8_str);
        }
    }
    else
    {
        const char *msg = "Invalid parameters of 'set_cookie' are invalid, there is no cookie value.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        delete cookie;
        return;
    }

    /**
     * Cookie domain.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_domain = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "domain"));
    if (!maybe_domain.IsEmpty())
    {
        v8::Local<v8::Value> domain = maybe_domain.ToLocalChecked();
        if (domain->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie domain is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (domain->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie domain is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!domain->IsString() && !domain->IsStringObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie domain should be a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (domain->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, domain);
            cookie->set_domain(*utf8_str);
        }
        if (domain->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, domain.As<v8::StringObject>()->ValueOf());
            cookie->set_domain(*utf8_str);
        }
    }

    /**
     * Cookie path.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_path = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "path"));
    if (!maybe_path.IsEmpty())
    {
        v8::Local<v8::Value> path = maybe_path.ToLocalChecked();
        if (path->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie path is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (path->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie path is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!path->IsString() && !path->IsStringObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie path should be a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (path->IsString())
        {
            v8::String::Utf8Value utf8_str(isolate, path);
            cookie->set_path(*utf8_str);
        }
        if (path->IsStringObject())
        {
            v8::String::Utf8Value utf8_str(isolate, path.As<v8::StringObject>()->ValueOf());
            cookie->set_path(*utf8_str);
        }
    }

    /**
     * Cookie expires.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_expires = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "expires"));
    if (!maybe_expires.IsEmpty())
    {
        v8::Local<v8::Value> expires = maybe_expires.ToLocalChecked();
        if (expires->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie expires is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (expires->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie expires is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!expires->IsInt32())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie expires should be a int type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        cookie->set_expires(expires.As<v8::Int32>()->Value());
    }

    /**
     * Cookie maxAge.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_age = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "maxAge"));
    if (!maybe_age.IsEmpty())
    {
        v8::Local<v8::Value> age = maybe_age.ToLocalChecked();
        if (age->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie maxAge is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (age->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie maxAge is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!age->IsInt32())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie maxAge should be a int type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        cookie->set_maxage(age.As<v8::Int32>()->Value());
    }

    /**
     * Cookie secure.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_secure = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "secure"));
    if (!maybe_secure.IsEmpty())
    {
        v8::Local<v8::Value> secure = maybe_secure.ToLocalChecked();
        if (secure->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie secure is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (secure->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie secure is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!secure->IsBoolean() && !secure->IsBooleanObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie secure should be a boolean type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (secure->IsBoolean())
        {
            cookie->set_secure(secure.As<v8::Boolean>()->Value());
        }
        if (secure->IsBooleanObject())
        {
            cookie->set_secure(secure.As<v8::BooleanObject>()->ValueOf());
        }
    }

    /**
     * Cookie httpOnly.
     * 
    */
    v8::MaybeLocal<v8::Value> maybe_http_only = v8_param->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "httpOnly"));
    if (!maybe_http_only.IsEmpty())
    {
        v8::Local<v8::Value> http_only = maybe_http_only.ToLocalChecked();
        if (http_only->IsNull())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie httpOnly is null.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (http_only->IsUndefined())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie httpOnly is undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (!http_only->IsBoolean() && !http_only->IsBooleanObject())
        {
            const char *msg = "Invalid parameters of 'set_cookie' are invalid, cookie httpOnly should be a boolean type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            delete cookie;
            return;
        }
        if (http_only->IsBoolean())
        {
            cookie->set_httponly(http_only.As<v8::Boolean>()->Value());
        }
        if (http_only->IsBooleanObject())
        {
            cookie->set_httponly(http_only.As<v8::BooleanObject>()->ValueOf());
        }
    }
    cookies->AddAllocated(cookie);
}

void instance::HttpResponse::set_status(const v8::FunctionCallbackInfo<v8::Value> &args)
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
        const char *msg = "Parameters count of 'set_status' function should be only one.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> status = args[0];
    if (status->IsNull())
    {
        const char *msg = "Parameters of 'set_status' are invalid, http status value is null.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (status->IsUndefined())
    {
        const char *msg = "Parameters of 'set_status' are invalid, http status value is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!status->IsInt32())
    {
        const char *msg = "Parameters of 'set_status' are invalid, http status value is not an int type.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // todo: check valid status code.
    response->mutable_call()->set_status(status.As<v8::Int32>()->Value());
}

void instance::HttpResponse::set_content_type(const v8::FunctionCallbackInfo<v8::Value> &args)
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
        const char *msg = "Parameters count of 'set_content_type' function should be only one.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    v8::Local<v8::Value> content_type = args[0];
    if (content_type->IsNull())
    {
        const char *msg = "Parameters of 'set_content_type' are invalid, content type value is null.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (content_type->IsUndefined())
    {
        const char *msg = "Parameters of 'set_content_type' are invalid, content type value is undefined.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!content_type->IsString() && !content_type->IsStringObject())
    {
        const char *msg = "Parameters of 'set_content_type' are invalid, content type value is not a string.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    // todo: check valid content type.
    if (content_type->IsString())
    {
        v8::String::Utf8Value utf8_str(isolate, content_type);
        response->mutable_call()->set_contenttype(*utf8_str);
    }
    if (content_type->IsStringObject())
    {
        v8::String::Utf8Value utf8_str(isolate, content_type.As<v8::StringObject>()->ValueOf());
        response->mutable_call()->set_contenttype(*utf8_str);
    }
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