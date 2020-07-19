#include <include/test.h>

test::RouteContext::RouteContext(protos::RuntimeRequest &request, protos::RuntimeResponse &response)
    : request_(request), response_(response)
{
    ok_ = false;
}

bool test::RouteContext::ok()
{
    return ok_;
}

void test::RouteContext::set_ok()
{
    ok_ = true;
}

const std::string &test::RouteContext::error()
{
    return error_;
}

void test::RouteContext::set_error(const char *error)
{
    error_.assign(error);
}

void test::RouteContext::set_error(std::string &error)
{
    error_ = error;
}

protos::RuntimeRequest &test::RouteContext::request()
{
    return request_;
}

protos::RuntimeResponse &test::RouteContext::response()
{
    return response_;
}