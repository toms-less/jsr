#include <include/instance.h>

instance::Instance::Instance(InstanceConfig &_config)
{
    this->config = _config;
}
instance::Instance::~Instance()
{
    isolate->Dispose();
    delete allocator_;
}

bool instance::Instance::Initialize()
{
    // v8 isolate parameters.
    v8::Isolate::CreateParams params;
    params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    allocator_ = params.array_buffer_allocator;

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

void instance::Instance::compile(CompileContext &context)
{
    if (!inited)
    {
        context.set_error("Current instance has not been initialized.");
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Context> handle_context = v8::Local<v8::Context>::New(isolate, context_);
    /**
     * Set the repository, namespace and project name to the context for compiling modules.
     * 
    */
    handle_context->SetEmbedderData(0, instance::Util::v8_str(isolate, context.script().repository().c_str()));
    handle_context->SetEmbedderData(1, instance::Util::v8_str(isolate, context.script().namespace_().c_str()));
    handle_context->SetEmbedderData(2, instance::Util::v8_str(isolate, context.script().workspace().name().c_str()));
    v8::Context::Scope context_scope(handle_context);

    // compile function script.
    const protos::JavaScript &script = context.script();
    v8::ScriptOrigin origin(instance::Util::v8_str(isolate, script.name().c_str()), v8::Local<v8::Integer>(),
                            v8::Local<v8::Integer>(), v8::Local<v8::Boolean>(), v8::Local<v8::Integer>(),
                            v8::Local<v8::Value>(), v8::Local<v8::Boolean>(),
                            v8::Local<v8::Boolean>(), v8::True(isolate));
    v8::ScriptCompiler::Source source(instance::Util::v8_str(isolate, context.script_content().c_str()), origin);
    v8::Local<v8::Module> module;
    if (!v8::ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }
    /**
     * Check module status.
     * In this condition, module status should be 'kUninstantiated'.
     * 
    */
    if (module->GetStatus() != v8::Module::kUninstantiated)
    {
        context.set_error("Compile error, current module status is not 'kUninstantiated'.");
        return;
    }
    module->InstantiateModule(handle_context, module::ScriptModule::resolve);
    // check exception.
    if (try_catch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }

    /**
     * Check module status.
     * In this condition, module status should be 'kUninstantiated'.
     * 
    */
    if (module->GetStatus() != v8::Module::kInstantiated)
    {
        context.set_error("Compile error, current module status is not 'kInstantiated'.");
        return;
    }

    v8::Local<v8::Value> names = module->GetModuleNamespace();
    v8::Local<v8::Object> names_object;
    if (!names->ToObject(handle_context).ToLocal(&names_object))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }

    /**
     * Get function of specific in the script, example:
     * ----test.js----
     * import {foo} from https://example/lib.js
     * 
     * export function test(request,response){
     *   response.send('xxxx');
     * }
     * ---------------
     * In this script, 'test' is the entry function
     * and export for outside services such as 'HTTPT'.
     * So the function must have the prefix 'export'.
     * 
    */
    v8::Local<v8::Value> value = names_object->Get(handle_context, instance::Util::v8_str(isolate, script.function().c_str())).ToLocalChecked();
    if (!value->IsFunction())
    {
        std::string error("Target function does not exist in the script, function: '");
        error.append(script.function()).append("'.");
        context.set_error(error);
        return;
    }

    /**
     * Set the function as global function for the other requesting.
     * 
    */
    handle_context->Global()->Set(handle_context, instance::Util::v8_str(isolate, script.function().c_str()), value);
    context.set_ok();
}

void instance::Instance::uncompile(UncompileContext &context)
{
    if (!inited)
    {
        context.set_error("Current instance has not been initialized.");
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Context> handle_context = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope context_scope(handle_context);

    const protos::JavaScript &script = context.script();
    v8::Local<v8::Value> value = handle_context->Global()->Get(handle_context, instance::Util::v8_str(isolate, script.function().c_str())).ToLocalChecked();
    if (!value->IsFunction())
    {
        /**
         * In this condition, the target function does not exist
         * in the global object, so it should do nothing and return.
         * 
        */
        context.set_ok();
        return;
    }
    if (!handle_context->Global()->Delete(handle_context, instance::Util::v8_str(isolate, script.function().c_str())).FromJust())
    {
        context.set_error("Delete function error.");
        return;
    }
    context.set_ok();
}

void instance::Instance::bind_function(BindFunctionContext &context)
{
    if (context.function().empty())
    {
        context.set_error("Bind error, function name is empty.");
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Context> handle_context = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope context_scope(handle_context);

    /**
     * Set the initialized map of system function for user binding.
     * 
    */
    v8::Local<v8::Array> data = v8::Array::New(isolate, 2);
    data->Set(handle_context, v8::Integer::New(isolate, 0), v8::External::New(isolate, context.instance_manager()));
    data->Set(handle_context, v8::Integer::New(isolate, 1), v8::External::New(isolate, context.sysfunc_map()));
    v8::Local<v8::FunctionTemplate> funcTemp = v8::FunctionTemplate::New(isolate, context.pfunc(), data);
    v8::Local<v8::Function> func;
    if (!funcTemp->GetFunction(handle_context).ToLocal(&func))
    {
        std::string error = "Bind error, get function from template error, function name '" + context.function() + "'.";
        context.set_error(error.c_str());
        return;
    }
    if (!handle_context->Global()->Set(handle_context, instance::Util::v8_str(isolate, context.function().c_str()), func).FromJust())
    {
        std::string error = "Set global function error, function name '" + context.function() + "'.";
        context.set_error(error.c_str());
        return;
    }

    // check exception.
    if (try_catch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }
    context.set_ok();
}

void instance::Instance::bind_object(BindObjectContext &context)
{
    if (context.object().empty())
    {
        context.set_error("Bind error, object name is empty.");
        return;
    }
    if (context.function().empty())
    {
        context.set_error("Bind error, function name is empty.");
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Context> handle_context = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope context_scope(handle_context);

    /**
     * Set the initialized map of system function for user binding.
     * 
    */
    v8::Local<v8::Array> data = v8::Array::New(isolate, 2);
    data->Set(handle_context, v8::Integer::New(isolate, 0), v8::External::New(isolate, context.instance_manager()));
    data->Set(handle_context, v8::Integer::New(isolate, 1), v8::External::New(isolate, context.sysfunc_map()));
    v8::Local<v8::FunctionTemplate> func_temp = v8::FunctionTemplate::New(isolate, context.pfunc(), data);
    v8::Local<v8::String> v8_object_name = instance::Util::v8_str(isolate, context.object().c_str());
    if (handle_context->Global()->Has(handle_context, v8_object_name).FromJust())
    {
        // current object has been added, add current function to this object.
        v8::Local<v8::Function> func;
        if (!func_temp->GetFunction(handle_context).ToLocal(&func))
        {
            std::string error = "Bind error, get function from template error, object name '" + context.object() + "', function name '" + context.function() + "'.";
            context.set_error(error.c_str());
            return;
        }

        v8::Local<v8::Value> value;
        if (!handle_context->Global()->Get(handle_context, v8_object_name).ToLocal(&value))
        {
            std::string error = "Bind error, get v8 object error, object name '" + context.object() + "', function name '" + context.function() + "'.";
            context.set_error(error.c_str());
            return;
        }
        v8::Local<v8::Object> object_instance = value.As<v8::Object>();
        object_instance->Set(handle_context, instance::Util::v8_str(isolate, context.function().c_str()), func);
        if (!handle_context->Global()->Set(handle_context, v8_object_name, object_instance).FromJust())
        {
            std::string error = "Bind error, set global object function error, object name '" + context.object() + "', function name '" + context.function() + "'.";
            context.set_error(error.c_str());
            return;
        }
    }
    else
    {
        // first time to add object.
        v8::Local<v8::ObjectTemplate> object_temp = v8::ObjectTemplate::New(isolate);
        object_temp->Set(instance::Util::v8_str(isolate, context.function().c_str()), func_temp);

        v8::Local<v8::Object> object_instance;
        if (!object_temp->NewInstance(handle_context).ToLocal(&object_instance))
        {
            v8::Local<v8::Value> stack;
            if (try_catch.StackTrace(handle_context).ToLocal(&stack))
            {
                v8::String::Utf8Value error_stack(isolate, stack);
                context.set_error(*error_stack);
            }
            else
            {
                v8::String::Utf8Value error_stack(isolate, try_catch.Exception());
                context.set_error(*error_stack);
            }
            return;
        }
        if (!handle_context->Global()->Set(handle_context, v8_object_name, object_instance).FromJust())
        {
            std::string error = "Bind error, set global object function error, object name '" + context.object() + "', function name '" + context.function() + "'.";
            context.set_error(error.c_str());
            return;
        }
    }

    // check exception.
    if (try_catch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }
    context.set_ok();
}

void instance::Instance::load_binding(LoadBindingContext &context)
{
    std::string &script = context.script();
    if (script.empty())
    {
        context.set_error("Load binding script error, script content is empty.");
        return;
    }
    if (script.length() > v8::String::kMaxLength)
    {
        std::string error = "Load binding script error, script is too long, max script length is '" + std::to_string(v8::String::kMaxLength) + "'.";
        context.set_error(error.c_str());
        return;
    }

    // As v8 limited, this place should set stack limit for multi-thread.
    v8::Locker locker(isolate);
    isolate->SetStackLimit(config.GetStackLimit());

    v8::HandleScope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Context> handle_context = v8::Local<v8::Context>::New(isolate, context_);
    v8::Context::Scope context_scope(handle_context);

    // compile function script.
    v8::Local<v8::String> source = instance::Util::v8_str(isolate, script.c_str());
    v8::Local<v8::Script> compiled_script;
    if (!v8::Script::Compile(handle_context, source).ToLocal(&compiled_script))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }

    // run compiled function script for calling later.
    v8::Local<v8::Value> result;
    if (!compiled_script->Run(handle_context).ToLocal(&result))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }

    // check exception.
    if (try_catch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(handle_context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            context.set_error(*error);
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            context.set_error(*error);
        }
        return;
    }
    context.set_ok();
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

    v8::Local<v8::Value> functionValue;
    if (!handleContext->Global()->Get(handleContext, instance::Util::v8_str(isolate, context.request()->call().function().c_str())).ToLocal(&functionValue))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.set_error(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.set_error(error);
        }
        context.set_status(ExecuteStatus::ERROR);
        return;
    }
    if (!functionValue->IsFunction())
    {
        std::string error = "Invalid function name, function '" + context.request()->call().function() + "' does not exist.";
        context.set_error(error);
        context.set_status(ExecuteStatus::ERROR);
        return;
    }
    v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(functionValue);

    // build request object.
    v8::Local<v8::ObjectTemplate> requestTemplate = v8::ObjectTemplate::New(isolate);

    // "request.system_info()"
    v8::Local<v8::FunctionTemplate> getSystemInfoTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::system_info, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "system_info"), getSystemInfoTemplate);

    // "request.content_type()"
    v8::Local<v8::FunctionTemplate> getContentTypeTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::content_type, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "content_type"), getContentTypeTemplate);

    // "request.method()"
    v8::Local<v8::FunctionTemplate> getMethodTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::method, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "method"), getMethodTemplate);

    // "request.header('foo')"
    v8::Local<v8::FunctionTemplate> getHeaderTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::header, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "header"), getHeaderTemplate);

    // "request.headers()"
    v8::Local<v8::FunctionTemplate> getHeadersTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::headers, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "headers"), getHeadersTemplate);

    // "request.cookie('foo')"
    v8::Local<v8::FunctionTemplate> getCookieTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::cookie, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "cookie"), getCookieTemplate);

    // "request.cookies()"
    v8::Local<v8::FunctionTemplate> getCookiesTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::cookies, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "cookies"), getCookiesTemplate);

    // "request.parameter('foo')"
    v8::Local<v8::FunctionTemplate> getParameterTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::parameter, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "parameter"), getParameterTemplate);

    // "request.parameters()"
    v8::Local<v8::FunctionTemplate> getParametersTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::parameters, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "parameters"), getParametersTemplate);

    // "request.data()"
    v8::Local<v8::FunctionTemplate> getDataTemplate = v8::FunctionTemplate::New(isolate, HttpRequest::data, v8::External::New(isolate, &context));
    requestTemplate->Set(instance::Util::v8_str(isolate, "data"), getDataTemplate);

    v8::Local<v8::Object> request;
    if (!requestTemplate->NewInstance(handleContext).ToLocal(&request))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.set_error(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.set_error(error);
        }
        context.set_status(ExecuteStatus::ERROR);
        return;
    }

    // build response object.
    v8::Local<v8::ObjectTemplate> responseTemplate = v8::ObjectTemplate::New(isolate);

    // "response.set_header('k','v')"
    v8::Local<v8::FunctionTemplate> setHeaderTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::set_header, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "set_header"), setHeaderTemplate);

    // "response.set_headers({'k1':'v','k2':'v2'})"
    v8::Local<v8::FunctionTemplate> setHeadersTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::set_headers, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "set_headers"), setHeadersTemplate);

    // "response.set_cookie({'name':'k1','value':'v1','maxAge':60})"
    v8::Local<v8::FunctionTemplate> setCookieTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::set_cookie, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "set_cookie"), setCookieTemplate);

    // "response.set_content_type('application/json')"
    v8::Local<v8::FunctionTemplate> setContentTypeTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::set_content_type, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "set_content_type"), setContentTypeTemplate);

    // "response.set_status(200)"
    v8::Local<v8::FunctionTemplate> setStatusTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::set_status, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "set_status"), setStatusTemplate);

    // "response.send({'k1':'v','k2':'v2'})"
    v8::Local<v8::FunctionTemplate> sendTemplate = v8::FunctionTemplate::New(isolate, HttpResponse::send, v8::External::New(isolate, &context));
    responseTemplate->Set(instance::Util::v8_str(isolate, "send"), sendTemplate);

    v8::Local<v8::Object> response;
    if (!responseTemplate->NewInstance(handleContext).ToLocal(&response))
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.set_error(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.set_error(error);
        }
        context.set_status(ExecuteStatus::ERROR);
        return;
    }

    // call user function.
    v8::Local<v8::Value> args[] = {request, response};
    function->Call(handleContext, handleContext->Global(), 2, args);

    if (tryCatch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (tryCatch.StackTrace(handleContext).ToLocal(&stack))
        {
            v8::String::Utf8Value errorStack(isolate, stack);
            std::string error(*errorStack);
            context.set_error(error);
        }
        else
        {
            v8::String::Utf8Value errorStack(isolate, tryCatch.Exception());
            std::string error(*errorStack);
            context.set_error(error);
        }
        context.set_status(ExecuteStatus::ERROR);
        return;
    }
    context.set_status(ExecuteStatus::FINISH);
}