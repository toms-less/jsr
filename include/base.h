#pragma once
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <queue>
#include <deque>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <string>
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "grpcpp/grpcpp.h"
#include "grpc/support/log.h"
#include "build/protos/runtime.pb.h"
#include "build/protos/runtime.grpc.pb.h"

namespace base
{

    /**
     * base module.
     * 
     */
    class Module
    {
    public:
        Module();
        ~Module();

        /**
         * initialize module.
         * 
         */
        virtual bool initialize() = 0;

        /**
         * set the status of initialization.
         *
         */
        void SetInited(bool inited);

        /**
         * get the status of initialization.
         * 
         */
        bool IsInited();

    protected:
        bool inited;
    };

    /**
     * base configuation.
     * 
     */
    class BaseConfig
    {
    public:
        BaseConfig();
        ~BaseConfig();

        /**
         * set the status of initialization.
         * 
         */
        void SetInited(bool inited);

        /**
         * get the status of initialization.
         * 
         */
        bool IsInited();

    protected:
        bool inited;
    };

    /**
     * logger configuration.
     * 
     */
    class LogConfig : public base::BaseConfig
    {
    public:
        LogConfig();
        ~LogConfig();

        void SetDebugLog(const std::string &debugLog);
        std::string &GetDebugLog();

        void SetServerLog(const std::string &serverLog);
        std::string &GetServerLog();

        void SetInstanceLog(const std::string &instanceLog);
        std::string &GetInstanceLog();

        void SetMaxSize(int size);
        int GetMaxSize();

        void SetRotate(int rotate);
        int GetRotate();

    private:
        /**
         * path of debug log.
         *
         */
        std::string debugLog;

        /**
         * path of server log.
         *  
         */
        std::string serverLog;

        /**
         * path of instance log.
         * 
         */
        std::string instanceLog;

        /**
         * max size of log in MB.
         * 
         */
        int maxSize;

        /**
         * rotated files.
         * 
         */
        int rotate;
    };

    /**
     * basic log for logging.
     *
     */
    class Log : public base::Module
    {
    public:
        Log(base::LogConfig *config);

        /**
         * initialize logger.
         *  
         */
        virtual bool initialize();

        /**
         * get debug logger for logging.
         * 
         */
        static std::shared_ptr<spdlog::logger> debug_logger();

        /**
         * get server logger for logging.
         * 
         */
        static std::shared_ptr<spdlog::logger> server_logger();

        /**
         * get instance logger for logging.
         *
         */
        static std::shared_ptr<spdlog::logger> instance_logger();

    private:
        static base::LogConfig *config_;
    };

    /**
     * thread pool.
     * 
     */
    class ThreadPool
    {
    public:
        /**
         * initialize a thread pool with specific thread size.
         *  
         */
        ThreadPool(size_t size);

        /**
         * clean.
         *  
         */
        ~ThreadPool();

        /**
         * submit a task with function and arguments, then return a future.
         * 
         */
        template <class F, class... Args>
        std::future<typename std::result_of<F(Args...)>::type> Submit(F &&f, Args &&... args);

        /**
         * get the current woking size of thread pool.
         * 
         */
        size_t WokingSize();

    private:
        /**
         *  workers of thread pool.
         * 
         */
        std::vector<std::thread> workers;

        /**
         * task queue of thread pool.
         * 
         */
        std::queue<std::function<void()>> tasks;

        /**
         *  mutex of thread pool.
         * 
         */
        std::mutex queueMutex;

        /**
         *  condition of thread pool.
         *
         */
        std::condition_variable condition;
    };

    /**
     * blocking queue.
     * 
     */
    template <class T>
    class BlockingQueue
    {
    public:
        /**
         * if 'maxCapacity' less than 0, the capacity of this blocking queue is no limited.
         * 
        */
        explicit BlockingQueue(const size_t maxCapacity = -1) : maxCapacity(maxCapacity)
        {
        }

        /**
         * get size of current blocking queue.
         * 
        */
        size_t size()
        {
            TLocker lock(mutex);
            return list.size();
        }

        /**
         * push item to the back of current blockinig queue.
         * 
        */
        void push_back(const T &item)
        {
            TLocker lock(mutex);
            if (maxCapacity > 0)
            {
                while (list.size() >= maxCapacity)
                {
                    notFull.wait(lock);
                }
            }
            list.push_back(item);
            notEmpty.notify_all();
        }

        /**
         * Pop an item from the queue.
         * 
        */
        T pop()
        {
            TLocker lock(mutex);
            if (list.empty())
            {
                return nullptr;
            }

            T value = *list.begin();
            list.pop_front();
            notFull.notify_all();

            lock.unlock();
            return value;
        }

        /**
         * check whether it is empty.
         * 
        */
        bool empty()
        {
            TLocker lock(mutex);
            return list.empty();
        }

        /**
         * check whether it is full.
         * 
        */
        bool full()
        {
            if (maxCapacity < 0)
            {
                return false;
            }
            TLocker lock(mutex);
            return list.size() >= maxCapacity;
        }

    private:
        typedef std::deque<T> TList;
        TList list;

        const size_t maxCapacity;
        typedef std::unique_lock<std::mutex> TLocker;
        std::mutex mutex;
        std::condition_variable notEmpty;
        std::condition_variable notFull;
    };

    /**
     * HTTP Header.
     * 
    */
    class Header
    {
    public:
        Header(const char *name, const char *value);

        const std::string &name();
        const std::string &value();

    private:
        std::string name_;
        std::string value_;
    };

    /**
     * HTTP cookie.
     * 
    */
    class Cookie
    {
    public:
        Cookie(const char *name, const char *value);

        const std::string &name();
        const std::string &value();

        void set_domain(const char *domain);
        const std::string &domain();

        void set_path(const char *path);
        const std::string &path();

        void set_expires(int32_t expires);
        const int32_t &expires();

        void set_max_age(int32_t max_age);
        const int32_t &max_age();

        void set_secure(bool secure);
        const bool &secure();

        void set_http_only(bool http_only);
        bool &http_only();

    private:
        std::string name_;
        std::string value_;
        std::string domain_;
        std::string path_;
        int32_t expires_;
        int32_t max_age_;
        bool secure_;
        bool http_only_;
    };

    /**
     * Context of HTTP client.
     * 
    */
    class HttpEntry
    {
    public:
        HttpEntry(const char *url);
        const std::string &url();

        void set_request_header(const char *name, const char *value);
        const std::vector<Header> &request_headers();

        void set_response_header(const char *name, const char *value);
        const std::vector<Header> &response_headers();

        void set_request_cookie(Cookie &cookie);
        const std::vector<Cookie> &request_cookies();

        void set_response_cookie(Cookie &cookie);
        const std::vector<Cookie> &response_cookies();

        void set_ok();
        const bool &ok();

        void set_status(int16_t status);
        const int16_t &status();

        void set_request_content(const char *content);
        const std::string &request_content();

        void set_response_content(const char *content);
        const std::string &response_content();

        void set_error(const char *content);
        const std::string &error();

        void set_timeout(uint timeout);
        const uint &timeout();

        void set_retry(int retry);
        const int &retry();

    private:
        /**
         * Request url, such as 'https://example.com?p=xx'.
         * 
        */
        std::string url_;

        /**
         * Request headers.
         * 
        */
        std::vector<Header> request_headers_;

        /**
         * Response headers.
         * 
        */
        std::vector<Header> response_headers_;

        /**
         * Request cookies.
         * 
        */
        std::vector<Cookie> request_cookies_;

        /**
         * Response cookies.
         * 
        */
        std::vector<Cookie> response_cookies_;

        /**
         * Requesting status. If status is 'false',
         * error message is ofen not empty.
         * 
        */
        bool ok_;

        /**
         * HTTP code, such as 404.
         * 
        */
        int16_t status_;

        /**
         * Request content.
         * 
        */
        std::string request_content_;

        /**
         * Response content.
         * 
        */
        std::string response_content_;

        /**
         * Error message which was occured in requesting.
         * 
        */
        std::string error_;

        /**
         * Request timeout value in millisecond.
         * 
        */
        uint timeout_;

        /**
         * Retry times when request failure.
         * If this value less than 0, it will
         * return error right now when error occured.
         * 
        */
        int retry_;
    };

    /**
     * HTTP client which is wrapped by libcurl.
     * 
    */
    class HttpClient
    {
    public:
        HttpClient();
        ~HttpClient();
        /**
         * Request HTTP server with 'GET' method synchronously.
         * 
        */
        void sync_get(HttpEntry &entry);

        /**
         * Request HTTP server with 'POST' method synchronously.
         * 
        */
        void sync_post(HttpEntry &entry);

    private:
        /**
         * Minimum timeout value in millisecond.
         * If user set timeout value less then
         * this value, it will set the timeout
         * to be this value for protecting system.
         * 
        */
        uint min_timeout_;

        /**
         * maximum timeout value in millisecond.
         * If user set timeout value more then
         * this value, it will set the timeout
         * to be this value for protecting system.
         * 
        */
        uint max_timeout_;

        /**
         * maximum retry times.
         * If user set retry times more then
         * this value, it will set the retry times
         * to be this value for protecting system.
         * 
        */
        int max_retry_;
    };

    /**
     * project utils.
     * 
     */
    class Util
    {
    public:
        /**
         * Get current work directory.
         * 
         */
        static std::string cwd();

        /**
         * Get current timeStamp
         * 
         */
        static std::time_t timestamp();
    };
} // namespace base
