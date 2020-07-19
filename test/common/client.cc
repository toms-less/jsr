#include <include/test.h>

test::Client::Client(std::shared_ptr<grpc::Channel> channel)
    : stub_(protos::RuntimeService::NewStub(channel))
{
}

void test::Client::route(RouteContext &ctx)
{
    grpc::ClientContext context;
    grpc::Status status = stub_->Route(&context, ctx.request(), &(ctx.response()));

    if (!status.ok())
    {
        std::string error("Function call failed, status code '");
        error.append(std::to_string(status.error_code())).append("', status message '").append(status.error_message()).append(".");
        ctx.set_error(error);
        return;
    }
    ctx.set_ok();
}