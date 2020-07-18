#include <include/test.h>

test::Client::Client(std::shared_ptr<grpc::Channel> channel)
    : stub_(protos::RuntimeService::NewStub(channel))
{
}

void test::Client::call(protos::RuntimeRequest &request, protos::RuntimeResponse &response)
{
    grpc::ClientContext context;
    grpc::Status status = stub_->Route(&context, request, &response);

    if (!status.ok())
    {
        std::cout << "Function call failed, status code '" << status.error_code()
                  << "', status message '" << status.error_message() << "'." << std::endl;
    }
}

void test::Client::script_update(protos::RuntimeRequest &request, protos::RuntimeResponse &response)
{
    grpc::ClientContext context;
    grpc::Status status = stub_->Route(&context, request, &response);

    if (!status.ok())
    {
        std::cout << "Script updating failed, status code '" << status.error_code()
                  << "', status message '" << status.error_message() << "'." << std::endl;
    }
}