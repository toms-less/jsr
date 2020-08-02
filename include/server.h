#pragma once
#include <thread>
#include <memory>
#include <iostream>
#include <include/base.h>
#include <include/instance.h>

namespace server
{

    /**
     * runtime server configuration.
     *  
     */
    class ServerConfig : public base::BaseConfig
    {
    public:
        /**
         * set runtime service port.
         * 
         */
        void SetPort(int port);

        /**
         * get runtime service port.
         * 
         */
        int GetPort();

    private:
        /**
         * runtime service port.
         * 
         */
        int port;
    };

    /**
     * runtime.
     * 
     */
    class Runtime
    {
    public:
        Runtime(server::ServerConfig &config, instance::IntanceManager *instances);
        ~Runtime();

        void start();

        server::ServerConfig config;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        protos::RuntimeService::AsyncService service_;
        std::unique_ptr<grpc::Server> server_;
        instance::IntanceManager *instances_;

        /**
         * Handle Runtime requesting.
         * In this method, runtime start an asynchronous service
         * with multi-thread. Thread count is equals with CPU cores.
         *
         */
        void handler();

    private:
        /**
         * Handle runtime requesting in the selected thread.
         * 
        */
        static void handler_inner(Runtime *runtime);
    };

    /**
     * Server context.
     * 
     * Runtime deals with some type requesting with asynchronous model,
     * such as function calling, script updating,
     * configuration updating and some more in the future.
     * 
     * This context represents one of above requesting at a time.
     * In the context, it deals with requesting with 'dispatch' method,
     * and executes the requesting with specified method like 'call_handler',
     * 'script_handler'.
     *
     */
    class Context
    {
    public:
        Context(protos::RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instances);

        /**
         * Context status.
         * 
        */
        enum Status
        {
            CREATE,
            PROCESS,
            FINISH
        };

        /**
         * Dispatch requesting by requesting type.
         * 
         */
        void dispatch();

        /**
         * Set context status.
         * 
        */
        void set_status(const server::Context::Status &status_);

    private:
        /**
         * Entry of dealing with calling requesting.
         * 
        */
        void call_handler();

        /**
         * Entry of deal with script action requesting.
         * 
        */
        void script_handler();

        /**
         * Deal with requesting of function script updateing.
         * 
        */
        void update_script(const protos::JavaScript &script);

        /**
         * Deal with requesting of function script adding.
         * 
        */
        void add_script(const protos::JavaScript &script);

        /**
         * Deal with requesting of function script deleting.
         * 
        */
        void delete_script(const protos::JavaScript &script);

        protos::RuntimeService::AsyncService *service_;
        grpc::ServerCompletionQueue *cq_;
        grpc::ServerContext ctx_;
        protos::RuntimeRequest request_;
        protos::RuntimeResponse response_;
        grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> writer_;
        instance::IntanceManager *instances_;
        Status status_;
    };
} // namespace server