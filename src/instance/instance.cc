#include <include/instance.h>

instance::Instance::Instance(InstanceConfig &_config)
{
    this->config = _config;
}
instance::Instance::~Instance()
{
}

bool instance::Instance::Initialize()
{
    // v8 isolate parameters.
    v8::Isolate::CreateParams params;

    uint32_t stackLimit = config.GetStackLimit();
    // bytes.
    uint64_t physicalMemory = config.GetPhysicalMemory() * 1024 * 1024;
    // bytes.
    uint64_t virtualMemoryLimit = config.GetVirtualMemoryLimit() * 1024 * 1024;
    // bytes.
    uint64_t maxOldeGenerationSize = config.GetMaxOldeGenerationSize() * 1024 * 1024;
    // bytes.
    uint64_t initialOldGenerationSize = config.GetInitialOldGenerationSize() * 1024 * 1024;
    // bytes.
    uint64_t maxYounGenerationSize = config.GetMaxYounGenerationSize() * 1024 * 1024;
    // bytes.
    uint64_t initialYoungGenerationSize = config.GetInitialYoungGenerationSize() * 1024 * 1024;
    // bytes.
    uint64_t codeRangeSize = config.GetCodeRangeSize() * 1024 * 1024;
    // bytes.
    uint64_t maxZonePoolSize = config.GetMaxZonePoolSize() * 1024 * 1024;

    if (physicalMemory > 0 && virtualMemoryLimit >= 0)
    {
        params.constraints.ConfigureDefaults(physicalMemory, virtualMemoryLimit);
    }
    if (maxOldeGenerationSize > 0)
    {
        params.constraints.set_max_old_generation_size_in_bytes(maxOldeGenerationSize);
    }
    if (initialOldGenerationSize > 0)
    {
        params.constraints.set_initial_old_generation_size_in_bytes(initialOldGenerationSize);
    }
    if (maxYounGenerationSize > 0)
    {
        params.constraints.set_max_young_generation_size_in_bytes(maxYounGenerationSize);
    }
    if (initialYoungGenerationSize > 0)
    {
        params.constraints.set_initial_young_generation_size_in_bytes(initialYoungGenerationSize);
    }
    if (stackLimit > 0)
    {
        params.constraints.set_stack_limit(&stackLimit);
    }
    if (codeRangeSize > 0)
    {
        params.constraints.set_code_range_size_in_bytes(codeRangeSize);
    }
    if (maxZonePoolSize > 0)
    {
        params.constraints.set_max_zone_pool_size(maxZonePoolSize);
    }

    // create v8 isolate.
    isolate = v8::Isolate::New(params);
    v8::Locker locker(isolate);
    v8::HandleScope handleScope(isolate);

    // reset global context.
    context_.Reset(isolate, v8::Context::New(isolate));
    isolate->SetStackLimit(stackLimit);
    inited = true;

    return true;
}

void instance::Instance::Compile(CompileContext &context)
{
    if (!inited)
    {
        std::string error("Current instance has not been initialized.");
        context.SetError(error);
        return;
    }
    const std::string &script = context.GetScript();
    if (script.empty())
    {
        std::string error("Current function script is empty.");
        context.SetError(error);
        return;
    }
    if (script.length() > v8::String::kMaxLength)
    {
        std::string error("Current function script is too long, max script length is '");
        error.append(std::to_string(v8::String::kMaxLength)).append("'.");
        context.SetError(error);
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolateScope(isolate);
    v8::TryCatch tryCatch(isolate);

    v8::Local<v8::Context> handleContext = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope contextScope(handleContext);

    // compile function script.
    v8::Local<v8::String> source = base::Utils::ToV8String(isolate, script.c_str());
    v8::Local<v8::Script> compiledScript;
    if (!v8::Script::Compile(handleContext, source).ToLocal(&compiledScript))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }

    // run compiled function script for calling later.
    v8::Local<v8::Value> result;
    if (!compiledScript->Run(handleContext).ToLocal(&result))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }

    // check exception.
    if (tryCatch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }
    context.SetSuccess(true);
}

void instance::Instance::Bind(BindContext &context)
{
    if (!inited)
    {
        std::string error("Current instance has not been initialized.");
        context.SetError(error);
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolateScope(isolate);
    v8::TryCatch tryCatch(isolate);

    v8::Local<v8::Context> handleContext = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope contextScope(handleContext);

    // bind pure functions.
    std::vector<sysfunc::PureFunction> &pureFunctionList = context.GetPureFunctionList();
    for (std::size_t i = 0; i < pureFunctionList.size(); i++)
    {
        sysfunc::PureFunction &pureFunc = pureFunctionList.at(i);
        std::string &functionName = pureFunc.GetFunctionName();
        if (functionName.empty())
        {
            continue;
        }
        v8::Local<v8::FunctionTemplate> funcTemp = v8::FunctionTemplate::New(isolate, pureFunc.GetFuncPointer());
        v8::Local<v8::Function> func;
        if (!funcTemp->GetFunction(handleContext).ToLocal(&func))
        {
            std::string error("Get function from template error, function name '");
            error.append(functionName).append("'.");
            context.SetError(error);
            return;
        }
        if (!handleContext->Global()->Set(handleContext, base::Utils::ToV8String(isolate, functionName.c_str()), func).FromJust())
        {
            std::string error("Set global function error, function name '");
            error.append(functionName).append("'.");
            context.SetError(error);
            return;
        }
    }

    // bind object function.
    std::vector<sysfunc::ObjectFunction> &objectFunctionList = context.GetObjectFunctopmList();
    for (std::size_t i = 0; i < objectFunctionList.size(); i++)
    {
        sysfunc::ObjectFunction &objectFunc = objectFunctionList.at(i);
        std::string &objectName = objectFunc.GetObjectName();
        std::string &functionName = objectFunc.GetFunctionName();
        if (objectName.empty() || functionName.empty())
        {
            continue;
        }
        v8::Local<v8::FunctionTemplate> funcTemp = v8::FunctionTemplate::New(isolate, objectFunc.GetFuncPointer());

        v8::Local<v8::String> v8ObjectName = base::Utils::ToV8String(isolate, objectName.c_str());
        if (handleContext->Global()->Has(handleContext, v8ObjectName).FromJust())
        {
            // current object has been added, add current function to this object.
            v8::Local<v8::Function> func;
            if (!funcTemp->GetFunction(handleContext).ToLocal(&func))
            {
                std::string error("Get function from template error, object name '");
                error.append(objectName).append("' function name '").append(functionName).append("'.");
                context.SetError(error);
                return;
            }

            v8::Local<v8::Value> value;
            if (!handleContext->Global()->Get(handleContext, v8ObjectName).ToLocal(&value))
            {
                std::string error("Get v8 object error, object name '");
                error.append(objectName).append("' function name '").append(functionName).append("'.");
                context.SetError(error);
                return;
            }
            v8::Local<v8::Object> objectInstance = value.As<v8::Object>();
            objectInstance->Set(handleContext, base::Utils::ToV8String(isolate, functionName.c_str()), func);
            if (!handleContext->Global()->Set(handleContext, v8ObjectName, objectInstance).FromJust())
            {
                std::string error("Set global object function error, object name '");
                error.append(objectName).append("' function name '").append(functionName).append("'.");
                context.SetError(error);
                return;
            }
        }
        else
        {
            // first time to add object.
            v8::Local<v8::ObjectTemplate> objectTemp = v8::ObjectTemplate::New(isolate);
            objectTemp->Set(base::Utils::ToV8String(isolate, functionName.c_str()), funcTemp);

            v8::Local<v8::Object> objectInstance;
            if (!objectTemp->NewInstance(handleContext).ToLocal(&objectInstance))
            {
                v8::Local<v8::Value> stack;
                if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
                {
                    v8::String::Utf8Value errorStack(isolate, stack);
                    std::string error(*errorStack);
                    context.SetError(error);
                }
                else
                {
                    v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
                    std::string error(*errorStack);
                    context.SetError(error);
                }
                return;
            }
            if (!handleContext->Global()->Set(handleContext, v8ObjectName, objectInstance).FromJust())
            {
                std::string error("Set global object function error, object name '");
                error.append(objectName).append("' function name '").append(functionName).append("'.");
                context.SetError(error);
                return;
            }
        }
    }
    context.SetSuccess(true);
}

void instance::Instance::Execute(ExecuteContext &context)
{
    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker loker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolateScope(isolate);
    v8::TryCatch tryCatch(isolate);

    v8::Local<v8::Context> handleContext = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope contextScope(handleContext);

    // build request object.
    v8::Local<v8::ObjectTemplate> requestTemplate = v8::ObjectTemplate::New(isolate);

    // "request.getSystemInfo()"
    v8::Local<v8::FunctionTemplate> getSystemInfoTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetSystemInfo, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getSystemInfo"), getSystemInfoTemplate);

    // "request.getContentType()"
    v8::Local<v8::FunctionTemplate> getContentTypeTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetContentType, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getContentType"), getContentTypeTemplate);

    // "request.getMethod()"
    v8::Local<v8::FunctionTemplate> getMethodTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetMethod, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getMethod"), getMethodTemplate);

    // "request.getHeader('foo')"
    v8::Local<v8::FunctionTemplate> getHeaderTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetHeader, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getHeader"), getHeaderTemplate);

    // "request.getHeaders()"
    v8::Local<v8::FunctionTemplate> getHeadersTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetHeaders, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getHeaders"), getHeadersTemplate);

    // "request.getCookie('foo')"
    v8::Local<v8::FunctionTemplate> getCookieTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetCookie, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getCookie"), getCookieTemplate);

    // "request.getCookies()"
    v8::Local<v8::FunctionTemplate> getCookiesTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetCookies, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getCookies"), getCookiesTemplate);

    // "request.getParameter('foo')"
    v8::Local<v8::FunctionTemplate> getParameterTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetParameter, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getParameter"), getParameterTemplate);

    // "request.getParameters()"
    v8::Local<v8::FunctionTemplate> getParametersTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetParameters, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getParameters"), getParametersTemplate);

    // "request.getData()"
    v8::Local<v8::FunctionTemplate> getDataTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::GetData, v8::External::New(isolate, &context));
    requestTemplate->Set(base::Utils::ToV8String(isolate, "getData"), getDataTemplate);

    v8::Local<v8::Object> request;
    if (!requestTemplate->NewInstance(handleContext).ToLocal(&request))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }

    // build response object.
    v8::Local<v8::ObjectTemplate> responseTemplate = v8::ObjectTemplate::New(isolate);

    // "response.setHeader('k','v')"
    v8::Local<v8::FunctionTemplate> setHeaderTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetHeader, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setHeader"), setHeaderTemplate);

    // "response.setHeaders({'k1':'v','k2':'v2'})"
    v8::Local<v8::FunctionTemplate> setHeadersTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetHeaders, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setHeaders"), setHeadersTemplate);

    // "response.setCookie({'name':'k1','value':'v1','maxAge':60})"
    v8::Local<v8::FunctionTemplate> setCookieTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetCookie, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setCookie"), setCookieTemplate);

    // "response.setCookies([{'name':'k1','value':'v1','maxAge':60},{'name':'k2','value':'v2','maxAge':60}])"
    v8::Local<v8::FunctionTemplate> setCookiesTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetCookies, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setCookies"), setCookiesTemplate);

    // "response.setContentType('application/json')"
    v8::Local<v8::FunctionTemplate> setContentTypeTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetContentType, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setContentType"), setContentTypeTemplate);

    // "response.setStatus(200)"
    v8::Local<v8::FunctionTemplate> setStatusTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::SetStatus, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "setStatus"), setStatusTemplate);

    // "response.send({'k1':'v','k2':'v2'})"
    v8::Local<v8::FunctionTemplate> sendTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::Send, v8::External::New(isolate, &context));
    responseTemplate->Set(base::Utils::ToV8String(isolate, "send"), sendTemplate);

    v8::Local<v8::Object> response;
    if (!responseTemplate->NewInstance(handleContext).ToLocal(&response))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }

    v8::Local<v8::Value> functionValue;
    if (!handleContext->Global()->Get(handleContext, base::Utils::ToV8String(isolate, context.GetRequest().GetInnerFunctionName().c_str())).ToLocal(&functionValue))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }
    v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(functionValue);

    // call user function.
    std::unique_ptr<v8::Local<v8::Value>> pargs(new v8::Local<v8::Value>[2]);
    pargs.get()[0] = request;
    pargs.get()[1] = response;

    v8::Local<v8::Value> result;
    if (!function->Call(handleContext, handleContext->Global(), 2, pargs.get()).ToLocal(&result))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }

    if (tryCatch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.SetError(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.SetError(error);
        }
        return;
    }
    context.SetStatus(ExecuteStatus::FINISH);
}