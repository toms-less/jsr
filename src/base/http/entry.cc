#include <include/base.h>

base::http::HttpEntry::HttpEntry(const char *url)
{
    url_.assign(url);
    https_ = false;
    ok_ = false;

    /**
     * Default value is 1 minute. 
     * 
    */
    timeout_ = 60000;

    /**
     * Default value is 1 minute. 
     * 
    */
    connect_timeout_ = 60000;

    /**
     * Without retry when request failure.
     * 
    */
    retry_ = -1;
}

const std::string &base::http::HttpEntry::url()
{
    return url_;
}

void base::http::HttpEntry::set_domain(std::string &domain)
{
    domain_ = domain;
}

const std::string &base::http::HttpEntry::domain()
{
    return domain_;
}

void base::http::HttpEntry::set_uri(std::string &uri)
{
    uri_ = uri;
}

const std::string &base::http::HttpEntry::uri()
{
    return uri_;
}

void base::http::HttpEntry::set_https()
{
    https_ = true;
}

const bool &base::http::HttpEntry::https()
{
    return https_;
}

void base::http::HttpEntry::set_parameter_str(std::string &str)
{
    parameter_str_ = str;
}

const std::string &base::http::HttpEntry::parameter_str()
{
    return parameter_str_;
}

void base::http::HttpEntry::set_request_header(const char *name, const char *value)
{
    base::http::Header header(name, value);
    request_headers_.push_back(header);
}

std::vector<base::http::Header> &base::http::HttpEntry::request_headers()
{
    return request_headers_;
}

void base::http::HttpEntry::set_response_header(const char *name, const char *value)
{
    base::http::Header header(name, value);
    response_headers_.push_back(header);
}

std::vector<base::http::Header> &base::http::HttpEntry::response_headers()
{
    return response_headers_;
}

void base::http::HttpEntry::set_request_cookie(base::http::Cookie &cookie)
{
    request_cookies_.push_back(cookie);
}

std::vector<base::http::Cookie> &base::http::HttpEntry::request_cookies()
{
    return request_cookies_;
}

void base::http::HttpEntry::set_response_cookie(base::http::Cookie &cookie)
{
    response_cookies_.push_back(cookie);
}

std::vector<base::http::Cookie> &base::http::HttpEntry::response_cookies()
{
    return response_cookies_;
}

void base::http::HttpEntry::set_ok()
{
    ok_ = true;
}
const bool &base::http::HttpEntry::ok()
{
    return ok_;
}

void base::http::HttpEntry::set_status(int16_t status)
{
    status_ = status;
}

const int16_t &base::http::HttpEntry::status()
{
    return status_;
}

void base::http::HttpEntry::set_status_message(const char *message)
{
    status_message_.assign(message);
}

const std::string &base::http::HttpEntry::status_message()
{
    return status_message_;
}

void base::http::HttpEntry::set_request_content(const char *content)
{
    request_content_.assign(content);
}

const std::string &base::http::HttpEntry::request_content()
{
    return request_content_;
}

void base::http::HttpEntry::set_response_content(const char *content)
{
    response_content_.assign(content);
}

const std::string &base::http::HttpEntry::response_content()
{
    return response_content_;
}

void base::http::HttpEntry::set_error(const char *content)
{
    error_.assign(content);
}

const std::string &base::http::HttpEntry::error()
{
    return error_;
}

void base::http::HttpEntry::set_timeout(uint timeout)
{
    timeout_ = timeout;
}
const uint &base::http::HttpEntry::timeout()
{
    return timeout_;
}

void base::http::HttpEntry::set_connect_timeout(uint connect_timeout)
{
    connect_timeout_ = connect_timeout;
}
const uint &base::http::HttpEntry::connect_timeout()
{
    return connect_timeout_;
}

void base::http::HttpEntry::set_retry(int retry)
{
    retry_ = retry;
}

const int &base::http::HttpEntry::retry()
{
    return retry_;
}

void base::http::HttpEntry::set_total_time(double total_time)
{
    total_time_ = total_time;
}

const double &base::http::HttpEntry::total_time()
{
    return total_time_;
}

void base::http::HttpEntry::set_download_bytes_size(curl_off_t &download_bytes_size)
{
    download_bytes_size_ = download_bytes_size;
}

const curl_off_t &base::http::HttpEntry::download_bytes_size()
{
    return download_bytes_size_;
}

void base::http::HttpEntry::set_upload_bytes_size(curl_off_t &upload_bytes_size)
{
    upload_bytes_size_ = upload_bytes_size;
}

const curl_off_t &base::http::HttpEntry::upload_bytes_size()
{
    return upload_bytes_size_;
}

void base::http::HttpEntry::set_redirect_count(long &redirect_count)
{
    redirect_count_ = redirect_count;
}

const long &base::http::HttpEntry::redirect_count()
{
    return redirect_count_;
}