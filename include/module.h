#pragma once
#include <include/base.h>

/**
 * This namespace is about to JavaScript
 * modules.
 * 
*/
namespace module
{
    /**
     * JavaScript module.
     * 
    */
    class ScriptModule
    {
    public:
        /**
         * Resolve modules.
         * 
         */
        static v8::MaybeLocal<v8::Module> module_resolve(v8::Local<v8::Context> context,
                                                            v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer);
    };

} // namespace module