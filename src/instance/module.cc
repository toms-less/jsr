#include <include/module.h>

v8::MaybeLocal<v8::Module> module::ScriptModule::module_resolve(v8::Local<v8::Context> context,
                                                                v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer)
{
    return v8::MaybeLocal<v8::Module>();
}