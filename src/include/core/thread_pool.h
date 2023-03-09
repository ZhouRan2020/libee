#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>
#include "core/looper.h"
#include "core/utils.h"
#ifndef SRC_INCLUDE_CORE_THREAD_POOL_H
#define SRC_INCLUDE_CORE_THREAD_POOL_H
namespace libee{
    static constexpr int MIN_NUM_THREADS_IN_POOL =2 ;
    /*this threadpool manages the thread resources and acts as the executor for client requests upon submitting a task,
    it gives back a future to be waited for*/
    class ThreadPool{
        public:
        explicit ThreadPool(int size=std::thread::hardware_concurrency()-1);
        ~ThreadPool();
        NON_COPYABLE(ThreadPool);

        template<typename F, typename ... Args>
        decltype(auto) SubmitTask(F&& new_task,Args&&...args);

        void Exit();

        size_t GetSize();

        private:
        std::vector<std::thread> threads_;
        std::queue<std::function<void()>> tasks_;
        std::mutex mtx_;
        std::condition_variable cv_;
        std::atomic<bool> exit_{false};
    };


template <typename F,typename... Args>
auto ThreadPool::SubmitTask(F&& new_task,Args&& ...args)->decltype(auto){
    using return_type = std::invoke_result_t<F, Args...>;
    if(exit_){
        throw std::runtime_error("threadpool:submittask() called while already exit_ be true");
    }

    auto packaged_new_task = std::make_shared<std::packaged_task<return_type()>>
    (std::bind(std::forward<F>(new_task),std::forward<Args>(args)...));

    auto fut = packaged_new_task->get_future();

    {
        std::unique_lock<std::mutex> lock(mtx_);
        tasks_.emplace
        (
            [packaged_new_task](){
                (*packaged_new_task)();
            }
        );
    }

    cv_.notify_one();

    return fut;        

}
}
#endif