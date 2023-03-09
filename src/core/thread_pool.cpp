#include "core/thread_pool.h"
#include "core/looper.h"
#include <mutex>

namespace libee{

    ThreadPool::ThreadPool(int size){
        size=std::max(size,MIN_NUM_THREADS_IN_POOL);
        for(auto i=0;i<size;++i){

            threads_.emplace_back([this](){

                while(1){

                    std::function<void()> next_task;

                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock,[this](){return exit_||!tasks_.empty();});
                        if(exit_&&tasks_.empty())return;
                        next_task=tasks_.front();
                        tasks_.pop();
                    }

                    next_task();
                }

            });

        }
    }

    ThreadPool::~ThreadPool(){
        Exit();
        for(auto& worker:threads_) if(worker.joinable()) worker.join();
    }

    void ThreadPool::Exit(){
        exit_ = true;
        cv_.notify_all();
    }

    size_t ThreadPool::GetSize() {return threads_.size();}
}