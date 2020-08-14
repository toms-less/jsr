#include <include/base.h>

void base::HttpClient::sync_get(HttpEntry &entry)
{
}

void base::HttpClient::sync_post(HttpEntry &entry)
{
}

base::Header::Header(std::string &name, std::string &value)
{
    name_ = name;
    value_ = value;
}

const std::string &base::Header::name()
{
    return name_;
}

const std::string &base::Header::value()
{
    return value_;
}

base::Cookie::Cookie(std::string &name, std::string &value)
{
    name_ = name;
    value_ = value;
}

const std::string &base::Cookie::name()
{
    return name_;
}

const std::string &base::Cookie::value()
{
    return value_;
}

void base::Cookie::set_domain(std::string &domain)
{
    domain_ = domain;
}

const std::string &base::Cookie::domain()
{
    return domain_;
}

void base::Cookie::set_path(std::string &path)
{
    path_ = path;
}

const std::string &base::Cookie::path()
{
    return path_;
}

void base::Cookie::set_expires(int32_t expires)
{
    expires_ = expires;
}

const int32_t &base::Cookie::expires()
{
    return expires_;
}

void base::Cookie::set_max_age(int32_t max_age)
{
    max_age_ = max_age;
}

const int32_t &base::Cookie::max_age()
{
    return max_age_;
}

void base::Cookie::set_secure(bool secure)
{
    secure_ = secure;
}

const bool &base::Cookie::secure()
{
    return secure_;
}

void base::Cookie::set_http_only(bool http_only)
{
    http_only_ = http_only;
}

bool &base::Cookie::http_only()
{
    return http_only_;
}

base::HttpEntry::HttpEntry()
{
    ok_ = false;
}

void base::HttpEntry::set_url(std::string &url)
{
    url_ = url;
}
const std::string &base::HttpEntry::url()
{
    return url_;
}

void base::HttpEntry::set_header(std::string &name, std::string &value)
{
    base::Header header(name, value);
    headers_.push_back(header);
}

const std::vector<base::Header> &base::HttpEntry::headers()
{
    return headers_;
}

void base::HttpEntry::set_cookie(base::Cookie &cookie)
{
    cookies_.push_back(cookie);
}

const std::vector<base::Cookie> &base::HttpEntry::cookies()
{
    return cookies_;
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

void base::HttpEntry::set_content(const char *content)
{
    content_.assign(content);
}

const std::string &base::HttpEntry::content()
{
    return content_;
}

void base::HttpEntry::set_error(std::string &error)
{
    error_ = error;
}

const std::string &base::HttpEntry::error()
{
    return error_;
}