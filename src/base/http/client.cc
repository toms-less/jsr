#include <include/base.h>

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

base::HttpClient::~HttpClient()
{
}

void base::HttpClient::sync_get(HttpEntry &entry)
{
}

void base::HttpClient::sync_post(HttpEntry &entry)
{
}