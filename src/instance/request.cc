#include <include/instance.h>

instance::HttpRequest::HttpRequest(std::string &_requestId, std::string &_functionName, std::string &_functionVersion,
                                   std::string &_cell, std::string &_unit, int &_timeout, std::string &_contentType, std::string &_method,
                                   std::map<std::string, std::string> &_headers, std::map<std::string, std::string> &_parameters,
                                   std::string &_data) : requestId(_requestId), functionName(_functionName), functionVersion(_functionVersion),
                                                         cell(_cell), unit(_unit), timeout(_timeout), contentType(_contentType), method(_method), headers(_headers),
                                                         parameters(_parameters), data(_data)
{
}
instance::HttpRequest::~HttpRequest()
{
}

void instance::HttpRequest::GetSystemInfo(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetContentType(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetMethod(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetHeader(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetHeaders(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetCookie(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetCookies(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetParameter(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetParameters(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

void instance::HttpRequest::GetData(const v8::FunctionCallbackInfo<v8::Value> &args)
{
}

std::string &instance::HttpRequest::GetInnerRequestId()
{
    return this->requestId;
}

std::string &instance::HttpRequest::GetInnerFunctionName()
{
    return this->functionName;
}

std::string &instance::HttpRequest::GetInnerFunctionVersion()
{
    return this->functionVersion;
}

std::string &instance::HttpRequest::GetInnerCell()
{
    return this->cell;
}

std::string &instance::HttpRequest::GetInnerUnit()
{
    return this->unit;
}

int &instance::HttpRequest::GetInnerTimeout()
{
    return this->timeout;
}

std::string &instance::HttpRequest::GetInnerContentType()
{
    return this->contentType;
}

std::string &instance::HttpRequest::GetInnerMethod()
{
    return this->method;
}

std::map<std::string, std::string> &instance::HttpRequest::GetInnerHeaders()
{
    return this->headers;
}

std::map<std::string, std::string> &instance::HttpRequest::GetInnerParameters()
{
    return this->parameters;
}

std::string &instance::HttpRequest::GetInnerData()
{
    return this->data;
}