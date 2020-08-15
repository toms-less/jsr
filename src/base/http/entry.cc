#include <include/base.h>

base::HttpEntry::HttpEntry(const char *domain, const char *uri, bool https)
{
    domain_.assign(domain);
    uri_.assign(uri);
    https_ = https;
    ok_ = false;

    /**
     * Default value is 1 minute. 
     * 
    */
    timeout_ = 60000;

    /**
     * Without retry when request failure.
     * 
    */
    retry_ = -1;
}

const std::string &base::HttpEntry::domain()
{
    return domain_;
}

const std::string &base::HttpEntry::uri()
{
    return uri_;
}

const bool &base::HttpEntry::https()
{
    return https_;
}

void base::HttpEntry::set_parameter_str(const char *str)
{
    parameter_str_.assign(str);
}

const std::string &base::HttpEntry::parameter_str()
{
    return parameter_str_;
}

void base::HttpEntry::set_request_header(const char *name, const char *value)
{
    base::Header header(name, value);
    request_headers_.push_back(header);
}

std::vector<base::Header> &base::HttpEntry::request_headers()
{
    return request_headers_;
}

void base::HttpEntry::set_response_header(const char *name, const char *value)
{
    base::Header header(name, value);
    response_headers_.push_back(header);
}

std::vector<base::Header> &base::HttpEntry::response_headers()
{
    return response_headers_;
}

void base::HttpEntry::set_request_cookie(base::Cookie &cookie)
{
    request_cookies_.push_back(cookie);
}

std::vector<base::Cookie> &base::HttpEntry::request_cookies()
{
    return request_cookies_;
}

void base::HttpEntry::set_response_cookie(base::Cookie &cookie)
{
    response_cookies_.push_back(cookie);
}

std::vector<base::Cookie> &base::HttpEntry::response_cookies()
{
    return response_cookies_;
}

void base::HttpEntry::set_ok()
{
    ok_ = true;
}
const bool &base::HttpEntry::ok()
{
    return ok_;
}

void base::HttpEntry::set_status(int16_t status)
{
    status_ = status;
}

const int16_t &base::HttpEntry::status()
{
    return status_;
}

void base::HttpEntry::set_request_content(const char *content)
{
    request_content_.assign(content);
}

const std::string &base::HttpEntry::request_content()
{
    return request_content_;
}

void base::HttpEntry::set_response_content(const char *content)
{
    response_content_.assign(content);
}

const std::string &base::HttpEntry::response_content()
{
    return response_content_;
}

void base::HttpEntry::set_error(const char *content)
{
    error_.assign(content);
}

const std::string &base::HttpEntry::error()
{
    return error_;
}

void base::HttpEntry::set_timeout(uint timeout)
{
    timeout_ = timeout;
}
const uint &base::HttpEntry::timeout()
{
    return timeout_;
}

void base::HttpEntry::set_retry(int retry)
{
    retry_ = retry;
}

const int &base::HttpEntry::retry()
{
    return retry_;
}