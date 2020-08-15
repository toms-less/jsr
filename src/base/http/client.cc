#include <include/base.h>
extern "C"
{
#include "curl.h"
}

base::HttpClient::HttpClient()
{
    /**
     * Default 5 milliseconds.
     * 
    */
    min_timeout_ = 5;

    /**
     * Default 5 minutes.
     * 
    */
    max_timeout_ = 5 * 60 * 1000;

    /**
     * Default 10 times.
     * 
    */
    max_retry_ = 10;
}

void base::HttpClient::sync_get(HttpEntry &entry)
{
    if (entry.domain().empty() || entry.uri().empty())
    {
        entry.set_error("Domain or uri is empty.");
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        return;
    }

    /**
     * Build request headers.
     * 
    */
    std::vector<base::Header> &request_headers = entry.request_headers();
    struct curl_slist *header_list = NULL;
    for (base::Header &header : request_headers)
    {
        const std::string &name = header.name();
        const std::string &value = header.value();
        if (name.empty() || value.empty())
        {
            continue;
        }

        std::string current;
        current.append(name).append(":").append(value);
        header_list = curl_slist_append(header_list, current.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

    /**
     * Build request cookies.
     * 
    */
    std::vector<base::Cookie> &request_cookies = entry.request_cookies();
    struct curl_slist *cookie_list = NULL;
    for (size_t i = 0; i < request_cookies.size(); i++)
    {
        base::Cookie &cookie = request_cookies.at(i);
        if (cookie.name().empty() || cookie.value().empty())
        {
            entry.set_error("Invalid request cookies, cookie name or value is empty.");
            curl_slist_free_all(header_list);
            curl_slist_free_all(cookie_list);
            curl_easy_cleanup(curl);
            return;
        }
        std::string current("set-cookie:");
        current.append(cookie.name()).append("=").append(cookie.value()).append(";");
        /**
         * Default value is the domain of this request
         * when there is no specific domain value.
         * 
        */
        current.append("domain=").append(cookie.domain().empty() ? entry.domain() : cookie.domain()).append(";");
        /**
         * Default value is the uri of this request
         * when there is no specific path value.
         * 
        */
        current.append("path=").append(cookie.path().empty() ? entry.uri() : cookie.path()).append(";");
        if (!cookie.expires().empty())
        {
            current.append("expires=").append(cookie.expires()).append(";");
        }
        current.append("max-age=").append(cookie.max_age() <= 0 ? std::to_string(-1) : std::to_string(cookie.max_age())).append(";");
        if (cookie.http_only())
        {
            current.append("HttpOnly;");
        }
        if (cookie.secure())
        {
            current.append("Secure;");
        }
        /**
         * Set cookie in the request header by header name 'set-cookie'.
         * 
        */
        cookie_list = curl_slist_append(cookie_list, current.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, cookie_list);

    /**
     * Build request url.
     * 
    */
    std::string url(entry.https() ? "https://" : "http://");
    url.append(entry.domain()).append(entry.uri());
    if (!entry.parameter_str().empty())
    {
        url.append("?").append(entry.parameter_str());
    }

    /**
     * Send request.
     * 
    */
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    // Get data with 'write_data' function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &entry);
    // Get headers with 'write_header' function.
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &entry);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    CURLcode res = curl_easy_perform(curl);
    switch (res)
    {
    case CURLE_OK:
    {
        entry.set_ok();
        break;
    }
    case CURLE_UNSUPPORTED_PROTOCOL:
    {
        entry.set_error("unsupported protocol.");
        break;
    }
    case CURLE_FAILED_INIT:
    {
        entry.set_error("failed init.");
        break;
    }
    case CURLE_URL_MALFORMAT:
    {
        entry.set_error("url malformat.");
        break;
    }
    case CURLE_COULDNT_RESOLVE_PROXY:
    {
        entry.set_error("can not resolve proxy.");
        break;
    }
    case CURLE_COULDNT_RESOLVE_HOST:
    {
        entry.set_error("can not resolve host.");
        break;
    }
    case CURLE_COULDNT_CONNECT:
    {
        entry.set_error("can not connect.");
        break;
    }
    case CURLE_HTTP_RETURNED_ERROR:
    {
        entry.set_error("http return error.");
        break;
    }
    default:
    {
        std::string error("unkown error, code [");
        error.append(std::to_string(res)).append("].");
        entry.set_error(error.c_str());
    }
    }

    curl_slist_free_all(header_list);
    curl_slist_free_all(cookie_list);
    curl_easy_cleanup(curl);
}

void base::HttpClient::sync_post(HttpEntry &entry)
{
}

size_t base::HttpClient::write_header(void *buffer, size_t size, size_t nmemb, void *entry)
{
    return size;
}

size_t base::HttpClient::write_data(void *buffer, size_t size, size_t nmemb, void *entry)
{
    return size;
}