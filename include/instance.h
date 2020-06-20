#pragma once
#include <include/base.h>
#include <include/sysfunc.h>

/**
 * v8 instance namespace.
 * 
*/
namespace instance
{

    class InstanceConfig : public base::BaseConfig
    {
    public:
        InstanceConfig();
        ~InstanceConfig();

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

        void SetCodeRangeSize(uint64_t codeRangeSize);
        uint64_t GetCodeRangeSize();

        void SetMaxZonePoolSize(uint64_t maxZonePoolSize);
        uint64_t GetMaxZonePoolSize();

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
     * function compile context.
     * 
     */
    class CompileContext
    {
    public:
        CompileContext(std::string &script);
        ~CompileContext();

        void SetScript(std::string &script);
        const std::string &GetScript();

        void SetSuccess(bool success);
        bool IsSuccess();

        void SetError(std::string &error);
        const std::string &GetError();

    private:
        std::string script;
        bool success = false;
        std::string error;
    };

    /**
     * system function bind context.
     * 
     */
    class BindContext
    {
    public:
        BindContext(std::vector<sysfunc::ObjectFunction> &objectFunctopmList, std::vector<sysfunc::PureFunction> &pureFunctionList);
        ~BindContext();

        void SetSuccess(bool success);
        bool IsSuccess();

        void SetError(std::string &error);
        const std::string &GetError();

        std::vector<sysfunc::ObjectFunction> &GetObjectFunctopmList();
        std::vector<sysfunc::PureFunction> &GetPureFunctionList();

    private:
        std::vector<sysfunc::ObjectFunction> &objectFunctopmList;
        std::vector<sysfunc::PureFunction> &pureFunctionList;
        bool success;
        std::string error;
    };

    /**
     * HTTP request parameter in the user main function. User API of 'request' as follow.
     * 1. request.get_system_info();
     * 2. request.get_content_type();
     * 3. request.get_method();
     * 4. request.get_header('foo');
     * 5. request.get_headers();
     * 6. request.get_cookie('foo');
     * 7. request.get_cookies();
     * 8. request.get_parameter('foo');
     * 9. request.get_parameters();
     * 10. request.get_data();
     *  
     */
    class HttpRequest
    {
    public:
        HttpRequest(std::string &requestId, std::string &functionName, std::string &functionVersion,
                    std::string &cell, std::string &unit, int &timeout, std::string &contentType, std::string &method,
                    std::map<std::string, std::string> &headers, std::map<std::string, std::string> &parameters,
                    std::string &data);
        ~HttpRequest();

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
        static void GetSystemInfo(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get content type of this request.
         * 1. application/json;
         * 2. application/html;
         * ...
         * 
         */
        static void GetContentType(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get method of this request.
         * 1. post;
         * 2. get;
         * ...
         *  
         */
        static void GetMethod(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a header of this request.
         *
         */
        static void GetHeader(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all headers of this request.
         * 
         */
        static void GetHeaders(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a cookie of this request.
         * 
         */
        static void GetCookie(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all cookies of this request.
         *  
         */
        static void GetCookies(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get a parameter of this request.
         *  
         */
        static void GetParameter(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get all parameters of this request.
         *
         */
        static void GetParameters(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Get the data of this request.
         *
         */
        static void GetData(const v8::FunctionCallbackInfo<v8::Value> &args);

        std::string &GetInnerRequestId();

        std::string &GetInnerFunctionName();

        std::string &GetInnerFunctionVersion();

        std::string &GetInnerCell();

        std::string &GetInnerUnit();

        int &GetInnerTimeout();

        std::string &GetInnerContentType();

        std::string &GetInnerMethod();

        std::map<std::string, std::string> &GetInnerHeaders();

        std::map<std::string, std::string> &GetInnerParameters();

        std::string &GetInnerData();

    private:
        std::string &requestId;
        std::string &functionName;
        std::string &functionVersion;
        std::string &cell;
        std::string &unit;
        int &timeout;

        std::string &contentType;
        std::string &method;
        std::map<std::string, std::string> &headers;
        std::map<std::string, std::string> &parameters;
        std::string &data;
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
        static void SetHeader(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set many headers for this request.
         *  
         */
        static void SetHeaders(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set a cookie for this request. 
         * 
         */
        static void SetCookie(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set many cookies for this request.
         * 
         */
        static void SetCookies(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP status code for this request.
         * 
         */
        static void SetStatus(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP content type for this request.
         * 
         */
        static void SetContentType(const v8::FunctionCallbackInfo<v8::Value> &args);

        /**
         * User function callback. Set HTTP data and response to HTTP trigger.
         * 
         */
        static void Send(const v8::FunctionCallbackInfo<v8::Value> &args);
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
        ExecuteContext(HttpRequest &request);
        ~ExecuteContext();

        std::time_t &GetStartTime();

        void SetEndTime(std::time_t &time);
        std::time_t &GetEndTime();

        void SetError(std::string &error);
        std::string &GetError();

        HttpRequest &GetRequest();

        void SetStatus(const ExecuteStatus &status);
        ExecuteStatus &GetStatus();

    private:
        std::time_t startTime;
        std::time_t endtTime;
        std::string error;
        HttpRequest &request;
        ExecuteStatus status;
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
        void Compile(CompileContext &context);

        /**
         * bind system functions.
         * 
         */
        void Bind(BindContext &context);

        /**
         * proccess function execution.
         * 
         */
        void Execute(ExecuteContext &context);

    private:
        InstanceConfig config;
        v8::Isolate *isolate;
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
        IntanceManager(InstanceConfig &config, sysfunc::SystemFuncManager &sysFuncManager);
        ~IntanceManager();

        /**
         * initialize many v8 instances with configurations.
         * 
         */
        virtual bool Initialize();

        /**
         * compile functions in all instances.
         *
         */
        void Compile(CompileContext &context);

        /**
         * proccess function execution with an instance.
         * 
         */
        void Execute(ExecuteContext &context);

    private:
        bool inited = false;
        InstanceConfig config;
        sysfunc::SystemFuncManager &sysFuncManager;
        std::vector<instance::Instance *> instances;

        /**
         * queue for working instances.
         * 
        */
        base::BlockingQueue<instance::Instance *> working;

        /**
         * queue for idle instances.
         * 
        */
        base::BlockingQueue<instance::Instance *> idle;
    };

} // namespace instance