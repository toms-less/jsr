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
        virtual bool Initialize() = 0;

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
        Log(base::LogConfig *logConfig);
        ~Log();

        /**
         * initialize logger.
         *  
         */
        virtual bool Initialize();

        /**
         * get debug logger for logging.
         * 
         */
        static std::shared_ptr<spdlog::logger> GetDebugLogger();

        /**
         * get server logger for logging.
         * 
         */
        static std::shared_ptr<spdlog::logger> GetServerLogger();

        /**
         * get instance logger for logging.
         *
         */
        static std::shared_ptr<spdlog::logger> GetInstanceLogger();

        static base::LogConfig *logConfig;
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
        explicit BlockingQueue(const int maxCapacity = -1) : maxCapacity(maxCapacity)
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

        const int maxCapacity;
        typedef std::unique_lock<std::mutex> TLocker;
        std::mutex mutex;
        std::condition_variable notEmpty;
        std::condition_variable notFull;
    };

    /**
     * project utils.
     * 
     */
    class Utils
    {
    public:
        /**
         * get current work directory.
         * 
         */
        static std::string GetCurrentWorkDir();

        /**
         * get current timeStamp
         * 
         */
        static std::time_t GetTimeStamp();

        /**
         * convert 'c string' to 'v8::String'
         * 
         */
        static v8::Local<v8::String> ToV8String(v8::Isolate *isolate, const char *str);
    };
} // namespace base
