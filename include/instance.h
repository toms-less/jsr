#pragma once
#include <include/base.h>
#include <include/module.h>

/**
 * v8 instance namespace.
 * 
*/
namespace instance
{
    /**
     * Declare classes.
     * 
    */
    class InstanceConfig;
    class CompileContext;
    class BindFunctionContext;
    class BindObjectContext;
    class LoadBindingContext;
    class HttpRequest;
    class HttpResponse;
    class ExecuteContext;
    class Instance;
    class IntanceManager;
    class Util;

    class InstanceConfig : public base::BaseConfig
    {
    public:
        InstanceConfig();

        void SetStartupData(const std::string &startupData);
        std::string &GetStartupData();

        void SetStackLimit(uint32_t stackLimit);
        uint32_t GetStackLimit();

        void SetPhysicalMemory(uint64_t physicalMemory);
        uint64_t GetPhysicalMemory();

        void SetVirtualMemoryLimit(uint64_t virtualMemoryLimit);
        uint64_t GetVirtualMemoryLimit();

        void SetMaxOldeGenerationSize(uint64_t maxOldeGenerationSize);
        uint64_t GetMaxOldeGenerationSize();

        void SetInitialOldGenerationSize(uint64_t initialOldGenerationSize);
        uint64_t GetInitialOldGenerationSize();

        void SetMaxYounGenerationSize(uint64_t maxYounGenerationSize);
        uint64_t GetMaxYounGenerationSize();

        void SetInitialYoungGenerationSize(uint64_t initialYoungGenerationSize);
        uint64_t GetInitialYoungGenerationSize();

        void SetInstanceCount(uint32_t instanceCount);
        uint32_t GetInstanceCount();

    private:
        /**
         *  path of startup cache.
         *  
         */
        std::string startupData;

        /**
         * stack limited size, deep of stack, default 1000.
         * 
         */
        uint32_t stackLimit = 1000;

        /**
         * total amount of physical memory in MB on the current device, default 2GB.
         * 
         */
        uint64_t physicalMemory = 2048;

        /**
         * the amount of virtual memory on the current device in bytes. if there is no limit, default value is 0.
         * 
         */
        uint64_t virtualMemoryLimit = 0;

        /**
         * max old generation size in MB, default value is 500.
         * 
         */
        uint64_t maxOldeGenerationSize = 500;

        /**
         * initial old generation size in MB, default value is 200.
         * 
         */
        uint64_t initialOldGenerationSize = 200;

        /**
         * max young generation size in MB, default value is 500.
         * 
         */
        uint64_t maxYounGenerationSize = 500;

        /**
         * initial young generation size in MB, default value is 200.
         * 
         */
        uint64_t initialYoungGenerationSize = 200;

        /**
         * code range size in MB, default value is 500.
         * 
         */
        uint64_t codeRangeSize = 500;

        /**
         * max zone pool size in MB, default value is 500.
         * 
         */
        uint64_t maxZonePoolSize = 500;

        /**
         * intance count, default value is 50.
         * 
         */
        uint32_t instanceCount = 50;
    };

    /**
     * Function compile context.
     * 
     */
    class CompileContext
    {
    public:
        CompileContext(const protos::JavaScript &script);
        const protos::JavaScript &script();

        void set_ok();
        bool ok();

        void set_error(std::string &error);
        void set_error(const char *error);
        const std::string &error();

        void set_script_content(std::string &content);
        std::string &script_content();

    private:
        bool ok_;
        std::string error_;
        const protos::JavaScript &script_;
        std::string script_content_;
    };

    /**
     * Function uncompile context.
     * 
     */
    class UncompileContext
    {
    public:
        UncompileContext(const protos::JavaScript &script);
        const protos::JavaScript &script();

        void set_ok();
        bool ok();

        void set_error(std::string &error);
        void set_error(const char *error);
        const std::string &error();

    private:
        bool ok_;
        std::string error_;
        const protos::JavaScript &script_;
    };

    /**
     * System function bind context.
     * 
     */
    class BindFunctionContext
    {
    public:
        BindFunctionContext(const char *function, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &), instance::IntanceManager *instance_manager, std::map<std::string, base::v8_cb> *sysfunc_map);

        instance::IntanceManager *instance_manager();
        std::map<std::string, base::v8_cb> *sysfunc_map();

        std::string &function();
        void (*pfunc())(const v8::FunctionCallbackInfo<v8::Value> &);

        void set_ok();
        bool &ok();

        void set_error(const char *error);
        const std::string &error();

    private:
        bool ok_;
        std::string function_;
        std::string error_;
        void (*pfunc_)(const v8::FunctionCallbackInfo<v8::Value> &);
        instance::IntanceManager *instance_manager_;
        std::map<std::string, base::v8_cb> *map_;
    };

    /**
     * System object bind context.
     * 
     */
    class BindObjectContext
    {
    public:
        BindObjectContext(const char *object, const char *function, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &), instance::IntanceManager *instance_manager, std::map<std::string, base::v8_cb> *sysfunc_map);

        instance::IntanceManager *instance_manager();
        std::map<std::string, base::v8_cb> *sysfunc_map();

        std::string &object();
        std::string &function();
        void (*pfunc())(const v8::FunctionCallbackInfo<v8::Value> &);

        void set_ok();
        bool &ok();

        void set_error(const char *error);
        const std::string &error();

    private:
        bool ok_;
        std::string object_;
        std::string function_;
        std::string error_;
        void (*pfunc_)(const v8::FunctionCallbackInfo<v8::Value> &);
        instance::IntanceManager *instance_manager_;
        std::map<std::string, base::v8_cb> *map_;
    };

    /**
     * Load binding context.
     * 
    */
    class LoadBindingContext
    {
    public:
        LoadBindingContext(std::string &script);
        std::string &script();

        void set_ok();
        bool &ok();

        void set_error(const char *error);
        const std::string &error();

    private:
        bool ok_;
        std::string error_;
        std::string &script_;
    };

    /**
     * HTTP request parameter in the user main function. User API of 'request' as follow.
     * 1. request.system_info();
     * 2. request.content_type();
     * 3. request.method();
     * 4. request.header('foo');
     * 5. request.headers();
     * 6. request.cookie('foo');
     * 7. request.cookies();
     * 8. request.parameter('foo');
     * 9. request.parameters();
     * 10. request.data();
     *  
     */
    class HttpRequest
    {
    public:
        /**
         * User function callback. Get system info in the user function, return an object with follow properties:
         *  1. requestId;
         * 2. functionName;
         * 3. functionVersion;
         * 4. cell;
         * 5. unit;
         * 6. timeout;
         * 
         */
        static void system_info(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get content type of this request.
         * 1. application/json;
         * 2. application/html;
         * ...
         * 
         */
        static void content_type(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get method of this request.
         * 1. post;
         * 2. get;
         * ...
         *  
         */
        static void method(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a header of this request.
         *
         */
        static void header(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all headers of this request.
         * 
         */
        static void headers(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a cookie of this request.
         * 
         */
        static void cookie(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all cookies of this request.
         *  
         */
        static void cookies(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a parameter of this request.
         *  
         */
        static void parameter(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all parameters of this request.
         *
         */
        static void parameters(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get the data of this request.
         *
         */
        static void data(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

    /**
     * HTTP response parameter int the user main function. User API os 'response' as follow:
     * 1. response.set_header('k','v');
     * 2. response.set_headers({'k1':'v1','k2':'v2'});
     * 3. response.set_cookie('k','v');
     * 4. response.set_cookie({'name':'k1','value':'v1','maxAge':200});
     * 5. response.set_cookies([{'name':'k1','value':'v1','maxAge':200},{'name':'k2','value':'v2','maxAge':200}]);
     * 6. response.set_status(200);
     * 7. response.set_content_type('application/html');
     * 8. response.send('<html><header><title>foo</title></header><body>foo</body></html>');
     *  
     */
    class HttpResponse
    {
    public:
        /**
         * User function callback. Set a header for this request.
         * 
         */
        static void set_header(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set many headers for this request.
         *  
         */
        static void set_headers(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set a cookie for this request. 
         * 
         */
        static void set_cookie(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP status code for this request.
         * 
         */
        static void set_status(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP content type for this request.
         * 
         */
        static void set_content_type(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP data and response to HTTP trigger.
         * 
         */
        static void send(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

    enum ExecuteStatus
    {
        /**
         * current request has been initialized.
         */
        INIT,

        /**
         * there is no idle instance for this request.
         */
        BUSY,

        /**
         * there are some error happend. 
         */
        ERROR,

        /**
         * current request has been executed successfully.
         */
        FINISH
    };

    /**
     * function execution context.
     * 
     */
    class ExecuteContext
    {
    public:
        ExecuteContext(protos::RuntimeRequest *request, protos::RuntimeResponse *response,
                       grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer,
                       base::BlockingQueue<instance::Instance *> *queue, intptr_t ctx_ptr);

        std::time_t &start_time();

        void set_end_time(std::time_t &time);
        std::time_t &end_time();

        void set_error(std::string &error);
        std::string &error();

        protos::RuntimeRequest *request();
        protos::RuntimeResponse *response();
        grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer();
        /**
         * 'server::Context' class pointer value
         * for using in 'HttpResponse:send'.
         * 
        */
        const intptr_t &ctx_ptr();

        void set_status(const ExecuteStatus &status);
        ExecuteStatus &status();

        /**
         * V8 instance queue.
         * 
        */
        base::BlockingQueue<instance::Instance *> *queue();

        /**
         * Current working v8 instance.
         * 
        */
        instance::Instance *working_instance();
        void set_working_instance(instance::Instance *instance);

    private:
        std::time_t start_time_;
        std::time_t end_time_;
        std::string error_;
        protos::RuntimeRequest *request_;
        protos::RuntimeResponse *response_;
        grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer_;
        const intptr_t ctx_ptr_;
        ExecuteStatus status_;
        base::BlockingQueue<instance::Instance *> *queue_;
        instance::Instance *working_instance_;
    };

    /**
     * V8 instance.
     *
     */
    class Instance
    {
    public:
        Instance(InstanceConfig &config);
        ~Instance();

        /**
         * initialize the v8 instance with configurations.
         *
         */
        bool Initialize();

        /**
         * compile functions.
         *
         */
        void compile(CompileContext &context);

        /**
         * Remove the compiled functions.
         *
         */
        void uncompile(UncompileContext &context);

        /**
         * Bind system function.
         * 
         */
        void bind_function(BindFunctionContext &context);

        /**
         * Bind system object.
         * 
         */
        void bind_object(BindObjectContext &context);

        /**
         * Load binding files.
         * 
        */
        void load_binding(LoadBindingContext &context);

        /**
         * proccess function execution.
         * 
         */
        void Execute(ExecuteContext &context);

    private:
        InstanceConfig config;
        v8::Isolate *isolate;
        v8::ArrayBuffer::Allocator *allocator_;
        v8::Persistent<v8::Context> context_;
        bool inited = false;
    };

    /**
     * instance manager.
     *  
     */
    class IntanceManager : public base::Module
    {
    public:
        IntanceManager(InstanceConfig &config);
        ~IntanceManager();

        /**
         * initialize many v8 instances with configurations.
         * 
         */
        virtual bool initialize();

        /**
         * compile functions in all instances.
         *
         */
        void compile(CompileContext &context);

        /**
         * Remove the compiled functions.
         *
         */
        void uncompile(UncompileContext &context);

        /**
         * proccess function execution with an instance.
         * 
         */
        void execute(ExecuteContext &context);

        /**
         * Get queue of v8 instances.
         * 
        */
        base::BlockingQueue<instance::Instance *> *queue();

        /**
         * Get the instance list.
         * 
        */
        std::vector<instance::Instance *> &instances();

    private:
        bool inited = false;
        InstanceConfig config;
        std::vector<instance::Instance *> instances_;

        /**
         * queue for v8 instances.
         * 
        */
        base::BlockingQueue<instance::Instance *> queue_;
    };

    /**
     * instance utils.
     * 
    */
    class Util
    {
    public:
        /**
         * Convert 'c string' to 'v8::String'.
         * 
         */
        static v8::Local<v8::String> v8_str(v8::Isolate *isolate, const char *str);

        /**
         * Build v8 error object.
         * 
        */
        static v8::Local<v8::Object> error(v8::Isolate *isolate, const char *name, const char *message, const char *stack);
    };

} // namespace instance