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

        void Start();

    private:
        server::ServerConfig config;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        RuntimeService::AsyncService service_;
        std::unique_ptr<grpc::Server> server_;
        instance::IntanceManager *instanceManager;

        /**
         * handle function calling.
         *
         */
        void ServerHandle();
    };

    /**
     *  server context.
     *
     */
    class Context
    {
    public:
        Context(RuntimeService::AsyncService *service, grpc::ServerCompletionQueue *cq, instance::IntanceManager *instanceManager);

        /**
         * deal with the RPC request.
         * 
         */
        void Proceed();

    private:
        RuntimeService::AsyncService *service_;
        grpc::ServerCompletionQueue *cq_;
        grpc::ServerContext ctx_;
        RuntimeRequest request_;
        RuntimeResponse response_;
        grpc::ServerAsyncResponseWriter<RuntimeResponse> responder_;
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