#include <include/base.h>

base::ThreadPool::ThreadPool(size_t size)
{
    for (size_t i = 0; i < size; ++i)
        workers.emplace_back(
            [this] {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return !this->tasks.empty(); });
                        if (this->tasks.empty())
                        {
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
}

base::ThreadPool::~ThreadPool()
{
    condition.notify_all();
    for (std::thread &worker : workers)
    {
        worker.join();
    }
}

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> base::ThreadPool::Submit(F &&f, Args &&... args)
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

size_t base::ThreadPool::WokingSize()
{
    return 0;
}