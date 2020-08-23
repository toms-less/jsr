#include <include/base.h>

base::http::HttpClient::HttpClient()
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
     * Default 5 milliseconds.
     * 
    */
    min_connect_timeout_ = 5;

    /**
     * Default 2 minutes.
     * 
    */
    max_connect_timeout_ = 2 * 60 * 1000;

    /**
     * Default 10 times.
     * 
    */
    max_retry_ = 10;
}

void base::http::HttpClient::sync_get(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    sync_request(curl, entry);
}

void base::http::HttpClient::sync_post(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, entry.request_content().empty() ? "" : entry.request_content().c_str());
    sync_request(curl, entry);
}

void base::http::HttpClient::sync_options(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
    sync_request(curl, entry);
}

void base::http::HttpClient::sync_patch(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    sync_request(curl, entry);
}

void base::http::HttpClient::sync_put(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    sync_request(curl, entry);
}

void base::http::HttpClient::sync_delete(HttpEntry &entry)
{
    if (!url_parse(entry))
    {
        return;
    }

    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        entry.set_error("'libcurl' initialized error.");
        curl_easy_cleanup(curl);
        return;
    }
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 0L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    sync_request(curl, entry);
}

bool base::http::HttpClient::url_parse(HttpEntry &entry)
{
    /**
     * Parse domain, https, uri and parameter string.
     * User valid url may be like below:
     * 1. https://example.com?p=xxx
     * 2. http://example.com?p=xxx
     * 3. https://example.com/key1=value1&key2=value2?p=xxx
     * 4. https://example.com/key1=value1&key2=value2
     * 5. https://example.com/
    */
    if (entry.url().empty())
    {
        entry.set_error("Url is empty.");
        return false;
    }

    const std::string http = "http://";
    const std::string https = "https://";
    bool normal_http = entry.url().find(http) == 0 ? true : false;
    bool normal_https = entry.url().find(https) == 0 ? true : false;
    if (!normal_http && !normal_https)
    {
        entry.set_error("HTTP request url is invalid, it should be start with 'http://' or 'https://'.");
        return false;
    }
    std::string::size_type domain_start = normal_https ? https.length() : http.length();
    std::string::size_type domain_end = entry.url().find("/", domain_start);
    if (domain_end != std::string::npos)
    {
        std::string domain = entry.url().substr(domain_start, domain_end - domain_start);
        entry.set_domain(domain);

        std::string::size_type uri_end = entry.url().find("?", domain_end);
        if (uri_end != std::string::npos)
        {
            std::string uri = entry.url().substr(domain_end + 1, uri_end - domain_end);
            entry.set_uri(uri);
            std::string parameter_str = entry.url().substr(uri_end + 1);
            entry.set_parameter_str(parameter_str);
        }
        else
        {
            std::string uri = entry.url().substr(domain_end + 1);
            entry.set_uri(uri);
        }
    }
    else
    {
        std::string domain = entry.url().substr(domain_start);
        entry.set_domain(domain);
        std::string uri = "/";
        entry.set_uri(uri);
    }
    if (normal_https)
    {
        entry.set_https();
    }
    return true;
}

void base::http::HttpClient::sync_request(CURL *curl, HttpEntry &entry)
{
    /**
     * Set timeout and connection timeout values.
     * 
    */
    if (entry.timeout() < min_timeout_)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, min_timeout_);
    }
    else if (entry.timeout() > max_timeout_)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, max_timeout_);
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, entry.timeout());
    }

    if (entry.connect_timeout() < min_connect_timeout_)
    {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, min_connect_timeout_);
    }
    else if (entry.connect_timeout() > max_connect_timeout_)
    {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, max_connect_timeout_);
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, entry.connect_timeout());
    }

    /**
     * Build request headers.
     * 
    */
    std::vector<base::http::Header> &request_headers = entry.request_headers();
    struct curl_slist *header_list = NULL;
    for (base::http::Header &header : request_headers)
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
    std::vector<base::http::Cookie> &request_cookies = entry.request_cookies();
    struct curl_slist *cookie_list = NULL;
    for (size_t i = 0; i < request_cookies.size(); i++)
    {
        base::http::Cookie &cookie = request_cookies.at(i);
        if (cookie.name().empty() || cookie.value().empty())
        {
            entry.set_error("Invalid request cookies, cookie name or value is empty.");
            curl_slist_free_all(header_list);
            curl_slist_free_all(cookie_list);
            curl_easy_cleanup(curl);
            return;
        }
        std::string current("Set-Cookie:");
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
     * Send request.
     * 
    */
    std::string header_string;
    std::string body_string;
    curl_easy_setopt(curl, CURLOPT_URL, entry.url().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    CURLcode res = curl_easy_perform(curl);
    switch (res)
    {
    case CURLE_OK:
    {
        parse(curl, header_string, body_string, entry);
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

    // TODO: retry when current request is failure.

    curl_slist_free_all(header_list);
    curl_slist_free_all(cookie_list);
    curl_easy_cleanup(curl);
}

size_t base::http::HttpClient::write_data(void *buffer, size_t size, size_t nmemb, std::string *data)
{
    data->append(static_cast<char *>(buffer), size * nmemb);
    return size * nmemb;
}

void base::http::HttpClient::parse(CURL *curl, const std::string &header_string, const std::string &body_string, base::http::HttpEntry &entry)
{
    std::vector<std::string> lines;
    std::istringstream stream(header_string);
    {
        std::string line;
        while (std::getline(stream, line, '\n'))
        {
            lines.push_back(line);
        }
    }
    for (auto &line : lines)
    {
        if (line.length() == 0)
        {
            continue;
        }
        /**
         * Parse status message.
         * 
        */
        if (line.substr(0, 5) == "HTTP/")
        {
            line.resize(std::min<size_t>(line.size(), line.find_last_not_of("\t\n\r ") + 1));
            size_t pos1 = line.find_first_of("\t ");
            size_t pos2 = std::string::npos;
            if (pos1 != std::string::npos)
            {
                pos2 = line.find_first_of("\t ", pos1 + 1);
            }
            if (pos2 != std::string::npos)
            {
                line.erase(0, pos2 + 1);
                entry.set_status_message(line.c_str());
            }
            continue;
        }

        /**
         * Parse headers.
         * 
        */
        size_t header_tag = line.find(":");
        if (header_tag != std::string::npos)
        {
            std::string value = line.substr(header_tag + 1);
            value.erase(0, value.find_first_not_of("\t "));
            value.resize(std::min<size_t>(value.size(), value.find_last_not_of("\t\n\r ") + 1));
            entry.set_response_header(line.substr(0, header_tag).c_str(), value.c_str());
        }
    }

    /**
     * Parse cookies
     * 
    */
    curl_slist *raw_cookies;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &raw_cookies);
    for (curl_slist *nc = raw_cookies; nc; nc = nc->next)
    {
        std::vector<std::string> tokens;
        std::stringstream stream(nc->data);
        std::string item;
        while (std::getline(stream, item, '\t'))
        {
            tokens.push_back(item);
        }

        std::string &value = tokens.back();
        tokens.pop_back();
        std::string &key = tokens.back();
        base::http::Cookie cookie;
        if (strcasecmp(key.c_str(), "domain") == 0)
        {
            cookie.set_domain(value.c_str());
        }
        else if (strcasecmp(key.c_str(), "path") == 0)
        {
            cookie.set_path(value.c_str());
        }
        else if (strcasecmp(key.c_str(), "expires") == 0)
        {
            cookie.set_expires(value.c_str());
        }
        else
        {
            cookie.set_name(key.c_str());
            cookie.set_value(value.c_str());
        }
        entry.set_request_cookie(cookie);
        curl_slist_free_all(raw_cookies);

        /**
         * Parse status.
         * 
        */
        int16_t status_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
        entry.set_status(status_code);

        /**
         * Set body.
         * 
        */
        entry.set_response_content(body_string.c_str());

        /**
         * Parse total time.
         * 
        */
        double total_time;
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        entry.set_total_time(total_time);

        /**
         * Parse downloaded bytes size.
         * 
        */
        curl_off_t download_bytes_size;
        curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_bytes_size);
        entry.set_download_bytes_size(download_bytes_size);

        /**
         * Parse uploaded bytes size.
         * 
        */
        curl_off_t upload_bytes_size;
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_bytes_size);
        entry.set_upload_bytes_size(upload_bytes_size);

        /**
         * Parse redirect count.
         * 
        */
        long redirect_count;
        curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect_count);
        entry.set_redirect_count(redirect_count);
    }
}