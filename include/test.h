#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "grpcpp/grpcpp.h"
#include "grpc/support/log.h"
#include "build/protos/runtime.pb.h"
#include "build/protos/runtime.grpc.pb.h"

namespace test
{
    /**
     * Route context.
     * 
    */
    class RouteContext
    {
    public:
        RouteContext(protos::RuntimeRequest &request, protos::RuntimeResponse &response);

        bool ok();
        void set_ok();

        const std::string &error();
        void set_error(const char *error);
        void set_error(std::string &error);

        protos::RuntimeRequest &request();
        protos::RuntimeResponse &response();

    private:
        bool ok_;
        std::string error_;
        protos::RuntimeRequest &request_;
        protos::RuntimeResponse &response_;
    };

    /**
     * gRPC client for testing.
     * 
    */
    class Client
    {
    public:
        Client(std::shared_ptr<grpc::Channel> channel);

        void route(RouteContext &ctx);

    private:
        std::unique_ptr<protos::RuntimeService::Stub> stub_;
    };

} // namespace test