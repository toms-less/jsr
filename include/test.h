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
     * gRPC client for testing.
     * 
    */
    class Client
    {
    public:
        Client(std::shared_ptr<grpc::Channel> channel);

        /**
         * Function calling.
         * 
        */
        void call(protos::RuntimeRequest &request, protos::RuntimeResponse &response);

        /**
         * Updating script.
         * 
        */
        void script_update(protos::RuntimeRequest &request, protos::RuntimeResponse &response);

    private:
        std::unique_ptr<protos::RuntimeService::Stub> stub_;
    };

} // namespace test