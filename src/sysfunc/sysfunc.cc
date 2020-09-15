#include <include/sysfunc.h>

void sysfunc::SysFunc::deps(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // Prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());
    v8::TryCatch try_catch(isolate);

    if (args.Length() != 1)
    {
        const char *msg = "'sysfunc.deps()' execution error, parameter size is not 1, it should be like 'sysfunc.deps('libxxx.so')'.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    v8::Local<v8::Value> parameter = args[0];
    if (parameter->IsUndefined())
    {
        const char *msg = "'sysfunc.deps()' execution error, parameter is undefined, it should be like 'sysfunc.deps('libxxx.so')'.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (parameter->IsNull())
    {
        const char *msg = "'sysfunc.deps()' execution error, parameter is null, it should be like 'sysfunc.deps('libxxx.so')'.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }
    if (!parameter->IsString())
    {
        const char *msg = "'sysfunc.deps()' execution error, parameter is not a string, it should be like 'sysfunc.deps('libxxx.so')'.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    /**
     * Make a new object for returning.
     * 
    */
    v8::Local<v8::FunctionTemplate> func_temp = v8::FunctionTemplate::New(isolate, sysfunc::SysFunc::bind, args.Data());
    v8::Local<v8::ObjectTemplate> object_temp = v8::ObjectTemplate::New(isolate);
    object_temp->Set(instance::Util::v8_str(isolate, "bind"), func_temp);
    v8::Local<v8::Object> object_instance;
    if (!object_temp->NewInstance(isolate->GetCurrentContext()).ToLocal(&object_instance))
    {
        v8::Local<v8::Value> stack;
        if (try_catch.StackTrace(isolate->GetCurrentContext()).ToLocal(&stack))
        {
            v8::String::Utf8Value error(isolate, stack);
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        else
        {
            v8::String::Utf8Value error(isolate, try_catch.Exception());
            isolate->ThrowException(instance::Util::error(isolate, "user", *error, *error));
        }
        return;
    }
    /**
     * Set the 'libstr' key in the object 
     * and it will be use to check whether
     * current binding is user level.
     * 
    */
    object_instance->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "libstr"), parameter.As<v8::String>());
    args.GetReturnValue().Set(object_instance);
}

void sysfunc::SysFunc::bind(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // Prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    const int args_length = args.Length();
    if (args_length != 1 && args_length != 2)
    {
        const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' parameter size is invalid.";
        v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
        isolate->ThrowException(error);
        return;
    }

    /**
     * Check whether current binding is user level.
     * 
    */
    bool with_lib = !args.This()->Get(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "libstr")).ToLocalChecked()->IsNullOrUndefined();
    if (args_length == 1)
    {
        /**
         * In this condition, user binds function like below:
         * bind({'httpGet':'Http::get','httpPost':'Http::post'})
         * 
        */
        v8::Local<v8::Value> function_info = args[0];
        if (!function_info->IsObject())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' parameter is not an object type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (with_lib)
        {
            bind_function_with_deps(args);
        }
        else
        {
            bind_function_without_deps(args);
        }
        return;
    }
    if (args_length == 2)
    {
        /**
         * In this condition, user binds function like below:
         * bind('http',{'get':'Http::get','post':'Http::post'})
         * 
        */
        v8::Local<v8::Value> object_name = args[0];
        v8::Local<v8::Value> function_info = args[1];
        if (!object_name->IsString())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' first parameter is not a string type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        v8::String::Utf8Value object_name_str(isolate, object_name);
        if (std::string(*object_name_str).empty())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' first parameter is an empty string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (!function_info->IsObject())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter is not an object type.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (with_lib)
        {
            bind_object_with_deps(args);
        }
        else
        {
            bind_object_without_deps(args);
        }
        return;
    }
}

void sysfunc::SysFunc::bind_object_without_deps(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // Prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    // Get system function maping and instance manager.
    v8::Local<v8::Array> external_arry = v8::Local<v8::Array>::Cast(args.Data());
    v8::Local<v8::Value> external_instance_manager = external_arry->Get(isolate->GetCurrentContext(), 0).ToLocalChecked();
    v8::Local<v8::Value> external_map = external_arry->Get(isolate->GetCurrentContext(), 1).ToLocalChecked();
    instance::IntanceManager *instance_manager = static_cast<instance::IntanceManager *>(external_instance_manager.As<v8::External>()->Value());
    std::map<std::string, base::v8_cb> *map = static_cast<std::map<std::string, base::v8_cb> *>(external_map.As<v8::External>()->Value());

    v8::String::Utf8Value object_name(isolate, args[0]);
    v8::Local<v8::Object> function_info = args[1].As<v8::Object>();
    v8::Local<v8::Array> names;
    if (!function_info->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocal(&names))
    {
        return;
    }
    for (uint32_t i = 0; i < names->Length(); i++)
    {
        v8::Local<v8::Value> name = names->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
        if (!name->IsString())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the key which type is not string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        v8::String::Utf8Value name_str(isolate, name);
        if (std::string(*name_str).empty())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the key which is empty.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }

        v8::Local<v8::Value> value = function_info->Get(isolate->GetCurrentContext(), name).ToLocalChecked();
        if (value->IsNullOrUndefined())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the value of the key is null or undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (!value->IsString())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the value of the key is not string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }

        v8::String::Utf8Value value_v8_str(isolate, value);
        std::string value_str(*value_v8_str);
        if (map->count(value_str) == 0)
        {
            std::string msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, local function is not found, local function is '" + value_str + "'.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg.c_str(), msg.c_str());
            isolate->ThrowException(error);
            return;
        }

        /**
         * Bind for all the instances.
         * 
        */
        for (instance::Instance *instance : instance_manager->instances())
        {
            instance::BindObjectContext bind_ctx(*object_name, *name_str, *(map->at(value_str)), instance_manager, map);
            instance->bind_object(bind_ctx);
            if (!bind_ctx.ok())
            {
                v8::Local<v8::Object> error = instance::Util::error(isolate, "user", bind_ctx.error().c_str(), bind_ctx.error().c_str());
                isolate->ThrowException(error);
                return;
            }
        }
    }
}

void sysfunc::SysFunc::bind_function_without_deps(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // Prepare v8 context.
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    v8::Context::Scope context_scope(isolate->GetCurrentContext());

    // Get system function maping and instance manager.
    v8::Local<v8::Array> external_arry = v8::Local<v8::Array>::Cast(args.Data());
    v8::Local<v8::Value> external_instance_manager = external_arry->Get(isolate->GetCurrentContext(), 0).ToLocalChecked();
    v8::Local<v8::Value> external_map = external_arry->Get(isolate->GetCurrentContext(), 1).ToLocalChecked();
    instance::IntanceManager *instance_manager = static_cast<instance::IntanceManager *>(external_instance_manager.As<v8::External>()->Value());
    std::map<std::string, base::v8_cb> *map = static_cast<std::map<std::string, base::v8_cb> *>(external_map.As<v8::External>()->Value());

    v8::Local<v8::Object> function_info = args[0].As<v8::Object>();
    v8::Local<v8::Array> names;
    if (!function_info->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocal(&names))
    {
        return;
    }
    for (uint32_t i = 0; i < names->Length(); i++)
    {
        v8::Local<v8::Value> name = names->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
        if (!name->IsString())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the key which type is not string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        v8::String::Utf8Value name_str(isolate, name);
        if (std::string(*name_str).empty())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the key which is empty.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }

        v8::Local<v8::Value> value = function_info->Get(isolate->GetCurrentContext(), name).ToLocalChecked();
        if (value->IsNullOrUndefined())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the value of the key is null or undefined.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }
        if (!value->IsString())
        {
            const char *msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, 'bind()' second parameter has the value of the key is not string.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg, msg);
            isolate->ThrowException(error);
            return;
        }

        v8::String::Utf8Value value_v8_str(isolate, value);
        std::string value_str(*value_v8_str);
        if (map->count(value_str) == 0)
        {
            std::string msg = "'sysfunc.bind()' or 'sysfunc.deps('libxxx.so').bind()' execution error, local function is not found, local function is '" + value_str + "'.";
            v8::Local<v8::Object> error = instance::Util::error(isolate, "user", msg.c_str(), msg.c_str());
            isolate->ThrowException(error);
            return;
        }

        /**
         * Bind for all the instances.
         * 
        */
        for (instance::Instance *instance : instance_manager->instances())
        {
            instance::BindFunctionContext bind_ctx(*name_str, map->at(value_str), instance_manager, map);
            instance->bind_function(bind_ctx);
            if (!bind_ctx.ok())
            {
                v8::Local<v8::Object> error = instance::Util::error(isolate, "user", bind_ctx.error().c_str(), bind_ctx.error().c_str());
                isolate->ThrowException(error);
                return;
            }
        }
    }
}

void sysfunc::SysFunc::bind_object_with_deps(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void sysfunc::SysFunc::bind_function_with_deps(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}