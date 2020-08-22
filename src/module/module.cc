#include <include/module.h>
#include <include/instance.h>

v8::MaybeLocal<v8::Module> module::ScriptModule::resolve(v8::Local<v8::Context> context,
                                                         v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer)
{
    v8::Isolate *isolate = context->GetIsolate();
    v8::TryCatch try_catch(isolate);
    v8::String::Utf8Value specifier_str(isolate, specifier);
    module::DepsParseContext parse_context(*specifier_str);

    /**
     * Parse the dependencies.
     * 
    */
    module::ScriptModule script_module;
    script_module.deps_parse(parse_context);
    if (!parse_context.ok())
    {
        isolate->ThrowException(instance::Util::error(isolate, "system", parse_context.error().c_str(), parse_context.error().c_str()));
        return v8::MaybeLocal<v8::Module>();
    }

    if (parse_context.type() == module::DepsType::UNKNOWN)
    {
        std::string msg("Unkown specifier in the script, specifier '");
        msg.append(*specifier_str).append("'.");
        isolate->ThrowException(instance::Util::error(isolate, "user", msg.c_str(), msg.c_str()));
        return v8::MaybeLocal<v8::Module>();
    }

    /**
     * Compile the parsed module script.
     * TODO: add module cache.
    */
    if (parse_context.script().empty())
    {
        return v8::Local<v8::Module>();
    }

    v8::ScriptOrigin origin(specifier, v8::Local<v8::Integer>(), v8::Local<v8::Integer>(),
                            v8::Local<v8::Boolean>(), v8::Local<v8::Integer>(), v8::Local<v8::Value>(),
                            v8::Local<v8::Boolean>(), v8::Local<v8::Boolean>(), v8::True(isolate));
    v8::ScriptCompiler::Source source(instance::Util::v8_str(isolate, parse_context.script().c_str()), origin);
    v8::Local<v8::Module> module;
    if (!v8::ScriptCompiler::CompileModule(isolate, &source).ToLocal(&module))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        return v8::MaybeLocal<v8::Module>();
    }
    /**
     * Check module status.
     * In this condition, module status should be 'kUninstantiated'.
     * 
    */
    if (module->GetStatus() != v8::Module::kUninstantiated)
    {
        const char *msg = "Compile error, current module status is not 'kUninstantiated'.";
        isolate->ThrowException(instance::Util::error(isolate, "user", msg, msg));
        return v8::MaybeLocal<v8::Module>();
    }
    module->InstantiateModule(context, module::ScriptModule::resolve);
    // check exception.
    if (try_catch.HasCaught())
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(context).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        return v8::MaybeLocal<v8::Module>();
    }

    /**
     * Check module status.
     * In this condition, module status should be 'kUninstantiated'.
     * 
    */
    if (module->GetStatus() != v8::Module::kInstantiated)
    {
        const char *msg = "Compile error, current module status is not 'kInstantiated'.";
        isolate->ThrowException(instance::Util::error(isolate, "system", msg, msg));
        return v8::MaybeLocal<v8::Module>();
    }
    return module;
}