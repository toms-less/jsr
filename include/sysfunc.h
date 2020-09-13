#pragma once
#include <include/instance.h>

namespace sysfunc
{
    /**
     * system functions about 'console'.
     * 1. 'console.log';
     * 2. 'console.info';
     * 3. 'console.warn';
     * 4. 'console.debug';
     * 5. 'console.error';
     * 6. 'console.time';
     * 7. 'console.timeEnd';
     *
     */
    class Console
    {
    public:
        /**
         * "console.log('foo')"
         * 
         */
        static void log(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.info('foo')"
         *  
         */
        static void info(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.warn('foo')"
         *  
         */
        static void warn(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.debug('foo')"
         * 
         */
        static void debug(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.error('foo')"
         * 
         */
        static void error(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.time()"
         * 
         */
        static void time(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * "console.timeEnd()"
         *
         */
        static void timeEnd(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

    /**
     * system functions about 'http'.
     * 1. 'http.get';
     * 2. 'http.post';
     * 3. 'http.options()';
     * 4. 'http.patch()';
     * 5. 'http.put()';
     * 6. 'http.delete()';
     *
     */
    class Http
    {
    public:
        /**
         * 'http.get()'
         * 
         */
        static void get(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * 'http.post()'
         * 
         */
        static void post(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * 'http.options()'
         * 
         */
        static void options(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * 'http.patch()'
         * 
         */
        static void patch(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * 'http.put()'
         * 
         */
        static void put(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * 'http.delete()'
         * 
         */
        static void del(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

    /**
     * system function manager.
     * 1. initialize all system functions.
     * 2. manager all system functions.
     * 
     */
    class SystemFuncManager : public base::Module
    {
    public:
        SystemFuncManager(instance::IntanceManager &instance_manager);

        virtual bool initialize();

    private:
        instance::IntanceManager &instance_manager_;

        std::map<std::string, base::v8_cb> map_;
    };

    /**
     * This class is used in the basic modules to load
     * the local and third party dependencies. For example:
     * 
     * 1. sysfunc.deps('libxxx.so').bind('kafka',{'send':'Kafka::send','comsumer':'Kafka::comsumer'});
     * 2. sysfunc.deps('libxxx.so').bind({'kafkaSend':'Kafka::send','kafkaComsumer':'Kafka::comsumer'});
     * 3. sysfunc.bind('http',{'get':'Http::get','post':'Http::post'});
     * 4. sysfunc.bind({'httpGet':'Http::get','httpPost':'Http::post'});
     * 
    */
    class SysFunc
    {
    public:
        /**
         * This function is used to load library or package of system function dependencies.
         * 
        */
        static void deps(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * This function is used to bind the outer functions in the dependencies into
         * the sysem function of jsr runtime.
         * 
        */
        static void bind(const v8::FunctionCallbackInfo<v8::Value> &args);

    private:
        /**
         * Bind object without dependency. For example:
         * sysfunc.bind('http',{'get':'Http::get','post':'Http::post'})
         * 
        */
        static void bind_object_without_deps(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * Bind function without dependency. For example:
         * sysfunc.bind({'httpGet':'Http::get','httpPost':'Http::post'});
         * 
        */
        static void bind_function_without_deps(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * Bind object with dependency. For example:
         * sysfunc.deps('libxxx.so').bind('kafka',{'send':'Kafka::send','comsumer':'Kafka::comsumer'})
         * 
        */
        static void bind_object_with_deps(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * Bind function with function. For example:
         * sysfunc.bind('http',{'get':'Http::get','post':'Http::post'})
         * 
        */
        static void bind_function_with_deps(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

} // namespace sysfunc