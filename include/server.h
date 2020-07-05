#pragma once
#include <thread>
#include <memory>
#include <iostream>
#include <include/base.h>
#include <include/instance.h>
#include "grpcpp/grpcpp.h"
#include "grpc/support/log.h"
#include "build/protos/runtime.pb.h"
#include "build/protos/runtime.grpc.pb.h"

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
        Runtime(server::ServerConfig &config, instance::IntanceManager *instanceManager);
        ~Runtime();

        void start();

        server::ServerConfig config;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        protos::RuntimeService::AsyncService service_;
        std::unique_ptr<grpc::Server> server_;
        instance::IntanceManager *instanceManager;

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
        Context(protos::RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instanceManager);

        /**
         * dispatch requesting by requesting type.
         * 
         */
        void dispatch();

    private:
        /**
         * deal with requesting of function calling.
         * 
        */
        void call_handler();

        /**
         * deal with requesting of function script updateing.
         * 
        */
        void script_handler();
        protos::RuntimeService::AsyncService *service_;
        grpc::ServerCompletionQueue *cq_;
        grpc::ServerContext ctx_;
        protos::RuntimeRequest request_;
        protos::RuntimeResponse response_;
        grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> writer_;
        instance::IntanceManager *instanceManager;

        enum Status
        {
            CREATE,
            PROCESS,
            FINISH
        };
        Status status_;
    };
} // namespace server